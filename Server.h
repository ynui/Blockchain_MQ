#ifndef __server_
#define __server_
#include "globals.h"

#include "Blockchain.h"
Blockchain *g_Blockchain;

typedef struct {
  mqd_t *array;
  size_t numOfMiners;
  size_t PhysicalSize;
} MQ_Array;


void serve();
void* serverWork();
void sendBlockToMiners();
mqd_t ConnectionRequset();
void initServer();
void initBlockchain();
BLOCK_T *initGenesisBlock();
void addToBlockchain(BLOCK_T *newBlock);
void checkAndPrintServerActionToLog(BLOCK_T* , bool);
BLOCK_T* getBlockFromMiner();
void addToMqArr(mqd_t mqToAdd);
void initMqArray();
void insertMqArray(mqd_t element);
void cleanConnectionMsg();

 #endif // !__serverrvoid serve()
