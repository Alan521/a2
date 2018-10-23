/*********************
CIS3210 assignment 2
Haiqing Gong
********************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <extraFunctions.h>

//Create a transfer list and initilize its mutex and variable
TransferList* createTransferList()
{
    TransferList* list = (TransferList*)malloc(sizeof(TransferList));
    list->head = list->tail = NULL;
    pthread_mutex_init(&list->mutex, NULL);
    //Initialize the condition variable
    pthread_cond_init(&list->cond, NULL);
    return list;
}

//Add transfer to the transfer list
void addTransfer(TransferList* list, int id, char file[], char* data, long long int size, long long int chunks){
	//state variables
    TransferNode* node = (TransferNode*)malloc(sizeof(TransferNode));
    node->id = id;
    strcpy(node->filename, file);
	node->data = data;
    node->fileSize = size;
    node->chunkSize = chunks; 
    node->next = NULL;
	
    pthread_mutex_lock(&list->mutex);
    if (list->tail != NULL) {
        list->tail->next = node;       // append after tail
        list->tail = node;
    } else {
        list->tail = list->head = node;   // first node
    }
    //Signal the consumer thread woiting on this condition variable
    pthread_cond_signal(&list->cond);
    pthread_mutex_unlock(&list->mutex);
    sleep(3);

}
//Remove transfers from the transfer list
TransferNode* removeTransfer(TransferList* list)
{
    pthread_mutex_lock(&list->mutex);
    
    //Wait for a signal if there's something on the list. if list is null, go back to sleep
    while(list->head == NULL){ 
        pthread_cond_wait(&list->cond, &list->mutex);
    }
    TransferNode* oldHead = list->head;
    list->head = oldHead->next;
    if (list->head == NULL) {
		// remove last node 
        list->tail = NULL;         
    }
    pthread_mutex_unlock(&list->mutex);
    return oldHead;

}

//display the transfer list
void displayTransferList(TransferList* list){
    pthread_mutex_lock(&list->mutex);
    //Wait for a signal if there's something on the list. if list is null, go back to sleep
    while(list->head == NULL){
        pthread_cond_wait(&list->cond, &list->mutex);
    }
    TransferNode* printOut = list->head;
    //iterate through the list and print all of active transfers 
    while(printOut != NULL){

        fprintf(stderr, "%d %s %s %lld %lld\n", printOut->id, printOut->filename, printOut->data, printOut->fileSize, printOut->chunkSize);
        printOut = printOut->next;
    }
    pthread_mutex_unlock(&list->mutex);
    return;
}
