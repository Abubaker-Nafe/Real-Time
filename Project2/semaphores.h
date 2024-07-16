#ifndef SEMAPHORES
#define SEMAPHORES

#include "header.h"
#include "message_queues.h"
#include "shared_memories.h"

//container
const char* Container_SEMAPHORE = "1984";
//information about whole project
const char* INFO_SEMAPHORE = "1255";
//Storage
const char* Storage_SEMAPHORE = "1540";
//Family
const char* Family_SEMAPHORE = "1747";
//Planes
const char* Plane_SEMAPHORE = "1474";
//collector
const char* Collector_SEMAPHORE = "1425";
//distributer
const char* Distributer_SEMAPHORE = "1644";
//Splitter
const char* Splitter_SEMAPHORE = "1599";
//Timer
const char* Timer_SEMAPHORE = "1502";
//PlaneNumOfContainers
const char* PlaneNumOfContainers_SEMAPHORE = "1002";
//Soldier
const char* Soldier_SEMAPHORE = "1010";

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