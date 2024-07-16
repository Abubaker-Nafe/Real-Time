#ifndef QUEUES
#define QUEUES

#include "header.h"

typedef struct queueMessage{
    long Dosmth;
    int CollectorIndex;
    int content;
    int PlaneIndex;
    int ContainerIndex;
}QueueMessage;

typedef struct ContainerInfo{
    long type;
    int ContainerIndex;
    int ContainerBasedOnPlaneIndex;
    int PlaneIndex;
    int content;
    int CollectorIndex;
    int DistributerIndex;
    int SplitterIndex;
    int FamilyIndex;
}ContainerInfo;

typedef struct PlaneRefilling{
    long type;
    int PlaneIndex;
}PlaneRefilling;



#define PlaneToCollecterQueue 0x1234
#define CollectorToSplittingQueue 0x1235
#define SplittingToDistributerQueue 0x1236

#define PlaneToOpenglQueue 0x5555
#define ContainerToOpenglQueue 0x7777
#define ContainerToCollecterQueue 0x6666
#define CollectorToOpengl 0x5551

#define SplitterToDistibuter 0x4444
#define SplitterToOpengl 0x3333

#define DistributerToOpengl 0x1111

/* For access the Queue (Creation)*/
int get_queue(int key){
    int queue;
    if ( (queue = msgget(key, IPC_CREAT | 0777)) == -1 ) {
        perror("Queue Access Error\n");
        exit(-1);
    }
    return queue;
}

int check_queue_empty(int queue, long queue_type){
    if(msgrcv(queue, NULL, 0, queue_type, IPC_NOWAIT) == -1){
        if(errno == E2BIG)
            return 0; // There is data
    }
    return 1; // Empty queue
}


#endif