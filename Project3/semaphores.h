#ifndef SEMAPHORES
#define SEMAPHORES

#include "header.h"
#include "message_queues.h"
#include "shared_memories.h"

//container
const char* Employee_Semaphore = "1984";
const char* Pill_Semaphore = "1457";
const char* Liquid_Semaphore = "9754";
const char* Line_Semaphore = "8744";
const char* LiquidLine_Semaphore = "8883";
const char* Info_Semaphore = "8711";
const char* LiquidPackages_Semaphore = "7787";
const char* PillPackages_Semaphore = "9511";
const char* Timer_SEMAPHORE = "4151";

//creation | access
sem_t* get_semaphore(const char* key){
    sem_t* sem = sem_open(key, O_CREAT, 0777, 0);
    if(sem == SEM_FAILED){
        perror("Semaphore Open Error\n");
        exit(-1);
    }
    return sem;
}


void attach_semaphore(sem_t* sem){
    // When return 0 for successful access
    if (sem_wait(sem) < 0){
        perror("Semaphore Wait Error\n");
        exit(-1);
    }
}


void detach_semaphore(sem_t* sem){
    // When return -1 then wrong issue happened
    if (sem_post(sem) < 0){
        perror("Semaphore Release Error\n");
        exit(-1);
    }
}

void close_semaphore(sem_t* sem){
    // Close the Semaphore
    sem_close(sem);
}

#endif