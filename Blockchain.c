#include "Blockchain.h"


Node * createnode(BLOCK_T* data) {
	Node * newNode = (Node*)malloc(sizeof(Node));
	if (!newNode) {
		return NULL;
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

Blockchain* makeBlockchain()
{
	Blockchain* fBlockchain = (Blockchain*)malloc(sizeof(Blockchain));
	if (!fBlockchain) {
		printf("Error initielizing blockchain...");
		return NULL;
	}
	fBlockchain->head = fBlockchain->tail = NULL;
	return fBlockchain;
}



void blockchainAdd(BLOCK_T* block, Blockchain * Blockchain) {
	Node* newNode = createnode(block);
	if (Blockchain->head == NULL) {
		Blockchain->head = Blockchain->tail = newNode;
	}
	else {
		newNode->next = Blockchain->head;
		Blockchain->head = newNode;
	}
}

void addToTail(Node* newNode, Blockchain * Blockchain) 
{
	if (Blockchain->head == NULL) {
		Blockchain->head = Blockchain->tail = newNode;
	}
	else {
		newNode->next = NULL;
		Blockchain->tail->next = newNode;
		Blockchain->tail = newNode;
	}
}

void destroy(Blockchain * Blockchain) {
	Node * temp = Blockchain->head;
	Node * next = temp;
	while (temp != NULL) {
		next = temp->next;
		free(temp->data);
		free(temp);
		temp = next;
	}
	Blockchain->head = Blockchain->tail = NULL;
}

bool isBlockchainEmpty(Blockchain* blockChain)
{
	return blockChain->head == NULL ;
}

void printBlock(BLOCK_T* block)
{
	printf("==========\nHeight: %d\nTimestamp: %d\nHash: %x\nPrev Hash: %x\nDifficulity: %d\nNonce: %d\nRelayed By: %d\n==========\n", block->height, block->timestamp, block->hash, block->prev_hash, block->difficulty, block->nonce, block->relayed_by);
}