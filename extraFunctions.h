#ifndef __threadedFunc__
#define __threadedFunc__
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
TransferList* createTransferList();
//Add transfer to transfer list
void addTransfer(TransferList* q, int id, char file[], long long int size, long long int chunks);
//Remove transfer from transfer list
void removeTransfer(TransferList* q);

void displayTransferList(TransferList* q);

#endif