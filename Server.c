
#include "Server.h"
mqd_t mq_connection;
mqd_t mq_reader;
CONNECTION_MSG_DATA_T *connectionMsg;
MQ_Array mq_senders;

void main() //server
{
	pthread_t blocklistener;
	printf("Server starting...\n");
	initServer();
	pthread_create(&blocklistener, NULL, serverWork, NULL);
	serve();
}

void serve()
{
	while (true)
	{
		mqd_t newConnectionQ = ConnectionRequset(); //get queue name and miner id
		cleanConnectionMsg();
		BLOCK_T *BlockToSend = g_Blockchain->head->data;
		mq_send(newConnectionQ, (char *)BlockToSend, MQ_MAX_MSG_SIZE, 0);
	}
}

void *serverWork()
{
	BLOCK_T *newBlock;
	while (true)
	{
		newBlock = getBlockFromMiner();
		if (isBlockValid(newBlock))
		{
			addToBlockchain(newBlock);
			checkAndPrintServerActionToLog(newBlock, true);
			sendBlockToMiners();
		}
		else
		{
			//height or hash wrong
			checkAndPrintServerActionToLog(newBlock, false);
			free(newBlock);
		}
	}
}

BLOCK_T *getBlockFromMiner()
{
	BLOCK_T *newBlock = (BLOCK_T*)malloc(sizeof(BLOCK_T));
	mq_receive(mq_reader, (char *)newBlock, MQ_MAX_MSG_SIZE, NULL);
	return newBlock;
}

void sendBlockToMiners()
{
	BLOCK_T *blockToSend = g_Blockchain->head->data;
	for (int i = 0; i < (&mq_senders)->numOfMiners; i++)
		mq_send((&mq_senders)->array[i], (char *)blockToSend, MQ_MAX_MSG_SIZE, 0);
}

mqd_t ConnectionRequset()
{
	mq_receive(mq_connection, (char *)connectionMsg, MQ_MAX_MSG_SIZE, NULL);
	mqd_t newConnectionMQ = mq_open(connectionMsg->MqName, O_WRONLY);
	insertMqArray(newConnectionMQ);
	printf("Server received connection request from miner #%d, queue name: %s\n",
		   connectionMsg->MinerId, connectionMsg->MqName);

	return newConnectionMQ;
}

void initBlockchain()
{
	g_Blockchain = makeBlockchain();
}

BLOCK_T *initGenesisBlock()
{
	BLOCK_T *genesisBlock = (BLOCK_T *)malloc(sizeof(BLOCK_T));
	genesisBlock->timestamp = (int)time(NULL);
	genesisBlock->height = 0;
	genesisBlock->prev_hash = 0;
	genesisBlock->difficulty = DIFFICULITY;
	genesisBlock->relayed_by = 0;
	genesisBlock->nonce = 0;
	genesisBlock->hash = generateBlockHash(genesisBlock);
	return genesisBlock;
}

void cleanConnectionMsg()
{
	connectionMsg->MinerId = -1;
	strcpy(connectionMsg->MqName, "/0");
}

void initServer()
{
	initMqArray();
	openNewMQ(MQ_CONNECTIONS_NAME);
	openNewMQ(MQ_MISSION_BLOCKS_Q);
	mq_reader = mq_open(MQ_MISSION_BLOCKS_Q, O_RDONLY);
	mq_connection = mq_open(MQ_CONNECTIONS_NAME, O_RDONLY);
	connectionMsg = (CONNECTION_MSG_DATA_T *)malloc(sizeof(CONNECTION_MSG_DATA_T));
	initBlockchain();
	BLOCK_T *genesisBlock = initGenesisBlock();
	addToBlockchain(genesisBlock);
}
void addToBlockchain(BLOCK_T *newBlock)
{
	blockchainAdd(newBlock, g_Blockchain);
}

void checkAndPrintServerActionToLog(BLOCK_T *block, bool isAddedToBlockchain)
{
	unsigned int calculatedHash;
	if (isAddedToBlockchain)
	{
		printf("Server: block added by %d, attributes: height(%d), timestamp(%d), hash(%08x), prev_hash(%08x), difficulty(%d), nonce(%d)\n",
				block->relayed_by, block->height,
				block->timestamp, block->hash,
			   	block->prev_hash, block->difficulty,
			   	block->nonce);
	}
	else
	{
		calculatedHash = generateBlockHash(block);
		if (block->hash != calculatedHash)
		{
			printf("Server: wrong hash for block # %d by miner # %d, received %08x but calculated %08x\n",
				   block->height, block->relayed_by,
				   block->hash, calculatedHash);
		}
		else if (isHashDifficulityOK(block->hash) == false)
		{
			printf("Server: wrong difficulity for block # %d by miner # %d, received %08x but calculated %08x\n",
				   block->height, block->relayed_by,
				   block->hash, calculatedHash);
		}
		else if (block->height - 1 != g_Blockchain->head->data->height)
		{
			printf("Server: wrong height for block # %d by miner # %d\n",
				   block->height, block->relayed_by);
		}
		else
		{
			printf("Error eding block:\n");
			printBlock(block);
		}
		
	}
}

void initMqArray()
{
	int MqArrInitSize = 10;
	(&mq_senders)->array = (mqd_t *)malloc(MqArrInitSize * sizeof(mqd_t));
	(&mq_senders)->numOfMiners = 0;
	(&mq_senders)->PhysicalSize = 1;
}

void insertMqArray(mqd_t element)
{
	if ((&mq_senders)->numOfMiners == (&mq_senders)->PhysicalSize)
	{
		(&mq_senders)->PhysicalSize *= 2;
		(&mq_senders)->array = (mqd_t *)realloc((&mq_senders)->array, (&mq_senders)->PhysicalSize * sizeof(mqd_t));
	}
	(&mq_senders)->array[(&mq_senders)->numOfMiners++] = element;
}