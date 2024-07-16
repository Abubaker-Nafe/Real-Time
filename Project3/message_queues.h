#ifndef QUEUES
#define QUEUES

#include "header.h"

typedef struct queueMessage{
    long type; // 0 --> create , 1 --> pause
}PillCreateQueue;

typedef struct queueMessage1{
    long type; // 0 --> create , 1 --> pause
}PillPauseQueue;

typedef struct queueMessage2{
    long type; // 0 --> create , 1 --> pause
}LiquidCreateQueue;

typedef struct queueMessage3{
    long type; // 0 --> create , 1 --> pause
}LiquidPauseQueue;

#define PillCreate 0x1234
#define PillPause 0x1644
#define LiquidCreate 0x1164
#define LiquidPause 0x1217


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