/*********************
CIS3210 assignment 1
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

//Transfer Node
typedef struct transfer_node {
    int id;
    char filename[21]; 
    long long int fileSize;
    long long int chunkSize;
    struct transfer_node* next;
} TransferNode;
//Transfer list - a singly linked list
//Remove from head, add to tail
typedef struct {
    TransferNode* head;
    TransferNode* tail;
    pthread_mutex_t mutex;
    
    //Add a condition variable
    pthread_cond_t cond;
} TransferList;

//Create a transfer list and initilize its mutex and condition variable
TransferList* createTransferList()
{
    TransferList* q = (TransferList*)malloc(sizeof(TransferList));
    q->head = q->tail = NULL;
    pthread_mutex_init(&q->mutex, NULL);
    
    //Initialize the condition variable
    pthread_cond_init(&q->cond, NULL);
    return q;
}

//Add transfer to transfer list
void addTransfer(TransferList* q, int id, char file[], long long int size, long long int chunks){
    TransferNode* node = (TransferNode*)malloc(sizeof(TransferNode));
    node->id = id;
    strcpy(node->filename, file);
    node->fileSize = size;
    node->chunkSize = chunks;
    node->next = NULL;

    // critical section
    pthread_mutex_lock(&q->mutex);
    if (q->tail != NULL) {
        q->tail->next = node;       // append after tail
        q->tail = node;
    } else {
        q->tail = q->head = node;   // first node
    }
    //Signal the consumer thread woiting on this condition variable
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
    //fprintf(stderr, "Worker %d enqueues the message, signals cond variable, unlocks mutex, and goes to sleep\n", sender);
    sleep(3);

}
//Remove transfer from transfer list
void removeTransfer(TransferList* q)
{
    // critical section
    pthread_mutex_lock(&q->mutex);
    
    //Wait for a signal that there's something on the list
    //If the list is still null, go back to sleep
    while(q->head == NULL){
        //fprintf(stderr,"Transfer list is empty and removeTransfer goes to sleep (pthread_cond_wait)\n");
        pthread_cond_wait(&q->cond, &q->mutex);
        //fprintf(stderr,"removeTransfer is woken up - someone signalled the condition variable\n");
    }
    
    
    TransferNode* oldHead = q->head;
    q->head = oldHead->next;
    if (q->head == NULL) {
        q->tail = NULL;         // last node removed
    }
    free(oldHead);
      
    //Release lock
    pthread_mutex_unlock(&q->mutex);


}