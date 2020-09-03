#ifndef __miner_
#define __miner_
#include "globals.h"
#include "lib.h"


#define NUM_OF_MINERS 4

void mine();
void initMiner();
mqd_t sendConnectionRequests();
BLOCK_T* getBlockFromServer ();
void getBlockValues(int minerID, BLOCK_T* lastBlockFromChain, BLOCK_T *res);
void mineBlockFromServer();
bool isMQempty();
void PrintMinerActionToLog(BLOCK_T*);
void printReceivedBlockDetails(BLOCK_T*);
void printMinerDetails();

#endif // !#define __miner_
