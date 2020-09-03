#ifndef __Blockchain__
#define __Blockchain__
#include "Block.h"
#include "lib.h"
#define DIFFICULITY 16

typedef struct node {
	BLOCK_T *data;
	struct node * next;
}Node;

typedef struct blockchain {
	Node* head;
	Node* tail;
}Blockchain;


void addToTail(Node* , Blockchain * );
Blockchain * makeBlockchain();
void blockchainAdd(BLOCK_T* block, Blockchain * Blockchain);
bool isBlockchainEmpty(Blockchain*);
void initBlockchain();
Blockchain* getBlockchain();
Node* createnode(BLOCK_T*);
void printBlock(BLOCK_T*);
void addMissionBlock(BLOCK_T*);
void initMutexes();
void destroy(Blockchain*);

#endif // !__Blockchain__
