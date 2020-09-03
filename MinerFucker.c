#include "Miner.h"

unsigned int generateBlockBadHash(BLOCK_T *block);

mqd_t mq_reader;
mqd_t mq_sender;
char blockQName[100];
int id;

void main(int argc, char **argv) //Writer
{
    initMiner();
    printMinerDetails();
    mine();
}

void initMiner()
{
    char num_as_char[100];
    id = getpid();
    snprintf(num_as_char, sizeof(num_as_char), "%d", id);
    strcpy(blockQName, "/MINER_Q_");
    strcat(blockQName, num_as_char);
    openNewMQ(blockQName);
    mq_reader = mq_open(blockQName, O_RDONLY);
    mq_sender = mq_open(MQ_MISSION_BLOCKS_Q, O_WRONLY);
}

void mine()
{

    sendConnectionRequests();
    mineBlockFromServer();
}

void sendMinedBlockToServer(BLOCK_T *newBlock)
{
    mq_send(mq_sender, (char *)newBlock, MQ_MAX_MSG_SIZE, 0);
}

void mineBlockFromServer()
{
    while (true)
    {
        BLOCK_T minedBlock;
        BLOCK_T *receivedBlock = getBlockFromServer();
        printReceivedBlockDetails(receivedBlock);
        getBlockValues(id, receivedBlock, &minedBlock);
        free(receivedBlock);
        do
        {
            minedBlock.nonce++;
            minedBlock.timestamp = (int)time(NULL);
            minedBlock.hash = generateBlockBadHash(&minedBlock);
            if (isBlockLegal(&minedBlock))
            {
                sendMinedBlockToServer(&minedBlock);
                PrintMinerActionToLog(&minedBlock);
            }
        } while (isMQempty());
    }
}

bool isMQempty()
{
    struct mq_attr attr;
    mq_getattr(mq_reader, &attr);
    return attr.mq_curmsgs == 0;
}

void getBlockValues(int minerID, BLOCK_T *lastBlockFromChain, BLOCK_T *res)
{
    res->height = lastBlockFromChain->height + 1;
    res->prev_hash = lastBlockFromChain->hash;
    res->difficulty = DIFFICULITY;
    res->relayed_by = minerID;
    res->nonce = -1;
    res->timestamp = 0;
    res->hash = 0;
}

BLOCK_T *getBlockFromServer()
{
    BLOCK_T *receivedBlock = (BLOCK_T *)malloc(sizeof(BLOCK_T));
    mq_receive(mq_reader, (char *)receivedBlock, MQ_MAX_MSG_SIZE, NULL);
    return receivedBlock;
}

mqd_t sendConnectionRequests()
{
    mqd_t mq = mq_open(MQ_CONNECTIONS_NAME, O_WRONLY);
    CONNECTION_MSG_DATA_T *connectionMsg = (CONNECTION_MSG_DATA_T *)malloc(sizeof(CONNECTION_MSG_DATA_T));
    connectionMsg->MinerId = id;
    strcpy(connectionMsg->MqName, blockQName);
    mq_send(mq, (char *)connectionMsg, MQ_MAX_MSG_SIZE, 0);
    printf("Miner Fucker # %d sent connection request on %s\n", connectionMsg->MinerId, connectionMsg->MqName);
    free(connectionMsg);
    
    return mq;
}

void PrintMinerActionToLog(BLOCK_T* block)
{
	printf("Miner Fucker # %d : Mined a new block # %d, with the hash %08x\n", 
    block->relayed_by, block->height, block->hash);
}

void printMinerDetails()
{
    printf("miner fucker id = %d, queue name = %s\n",id,blockQName);
}

void printReceivedBlockDetails(BLOCK_T* block)
{
    	printf("Miner fucker #%d received block: relayed_by(%d), height(%d), timestamp(%d), hash(%08x), prev_hash(%08x), difficulty(%d), nonce(%d)\n", 
            id ,block->relayed_by, block->height,
			block->timestamp, block->hash,
			block->prev_hash, block->difficulty,
			block->nonce);
}

unsigned int generateBlockBadHash(BLOCK_T *block)
{
	char toHash[250];
	snprintf(toHash, sizeof(toHash), "%d%d%d%d%d%u",
			 block->height, block->difficulty,
			 block->relayed_by, block->timestamp,
			 block->nonce, block->prev_hash);
	return crc32(0, toHash, stringlen(toHash));
}