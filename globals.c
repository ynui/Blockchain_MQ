#ifndef __globals_
#define __globals_

#include "globals.h"


// BLOCK_T* getLastBlock()
// {
// 	BLOCK_T* ret;
// 	if(isBlockchainEmpty(g_Blockchain))
// 		ret = NULL;
// 	else
// 		ret = g_Blockchain->head->data;
// 	printBlock(ret);
// 	return ret;
// }


// void initGlobals(Blockchain* blockchain)
// {
// 	g_Blockchain = blockchain;
// }


bool isBlockValid(BLOCK_T *minedBlock)
{
	unsigned int calculatedCrc32Hash = generateBlockHash(minedBlock);
	return (calculatedCrc32Hash == minedBlock->hash) &&
		   (isBlockLegal(minedBlock));
}
bool isHashDifficulityOK(unsigned int hash)
{
	if(DIFFICULITY == 0) return true;
	unsigned int mask = 0xFFFFFFFF;
	mask <<= (32 - DIFFICULITY);
	return ((hash & mask) == 0);
}

bool isBlockLegal(BLOCK_T* block)// check hash and height
{
	return isHashDifficulityOK(block->hash);// &&
			//block->height - 1 == g_Blockchain->head->data->height;
}

void copy_block(BLOCK_T* src, BLOCK_T** dest)
{
	*dest = (BLOCK_T*)malloc(sizeof(BLOCK_T));
	(*dest)->height = src->height;
	(*dest)->timestamp = src->timestamp;
	(*dest)->hash = src->hash;
	(*dest)->prev_hash = src->prev_hash;
	(*dest)->difficulty = src->difficulty;
	(*dest)->nonce = src->nonce;
	(*dest)->relayed_by = src->relayed_by;
}

unsigned int generateBlockHash(BLOCK_T *block)
{
	char toHash[250];
	snprintf(toHash, sizeof(toHash), "%u%d%d%d%d%d",
			 block->prev_hash, block->height,
			 block->difficulty, block->relayed_by,
			 block->timestamp, block->nonce);
	return crc32(0, toHash, stringlen(toHash));
}

int stringlen(char *str)
{
    int len = 0;
    int i;
    for (i=0; str[i] != 0; i++) 
    {
        len++;
    }
    return(len);
}

void openNewMQ(char* MQname)
{
	
	mqd_t mq;
    struct mq_attr attr = {0};

    attr.mq_maxmsg = MQ_MAX_SIZE;
    attr.mq_msgsize = MQ_MAX_MSG_SIZE;

	mq_unlink(MQname); // delete first if already exists, this requires sudo privilege
    mq = mq_open(MQname, O_CREAT, S_IRWXU | S_IRWXG, &attr);
	mq_close(mq);
}
#endif //!__globals_