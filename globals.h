#ifndef __locks_
#define __locks_
#include "Blockchain.h"
#define MQ_MAX_SIZE        			20
#define MQ_MAX_MSG_SIZE    			sizeof(BLOCK_T) 		//Some big value(in bytes)
#define MQ_CONNECTIONS_NAME         "/connectionsRequests_mq"
#define MQ_MISSION_BLOCKS_Q     	"/mission_mq"



typedef struct connection_msg_data{
	int MinerId;
    char MqName [100];
}CONNECTION_MSG_DATA_T;



// Blockchain *g_Blockchain;

void initGlobals(Blockchain*);
void addMissionBlock(BLOCK_T*);
BLOCK_T* getLastBlock();
bool isBlockValid(BLOCK_T*);
bool isHashDifficulityOK(unsigned int);
bool isBlockLegal(BLOCK_T*);
void copy_block(BLOCK_T* src, BLOCK_T** dest);
unsigned int generateBlockHash(BLOCK_T *);
int stringlen(char *);
void openNewMQ(char* MQname);


#endif