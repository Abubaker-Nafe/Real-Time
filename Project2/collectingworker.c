#include "header.h"
#include "message_queues.h"
#include "shared_memories.h"

int main(int argc, char** argv){

//    int sleep1 = 1;
//    while(sleep1); //for debugging

    if ( argc < 2){
        perror("Argc less than 2\n");
        exit(-1);
    }

    QueueMessage MQ_Struct; // MQ_Struct.Dosmth == 1 for replacement ||  MQ_Struct.Dosmth = 2 for receiving container
    ContainerInfo ContainerInfoQueue;
    pid_t openglPID = atoi(argv[1]);

    while(readTimer() < readSimulationTime() && readNumOfCollectors() > 0 && readNumOfCollectorsShot() < readMaxCollectingWorkersShot() &&
          readNumOfDistributersShot() < readMaxDistributionWorkersShot()
          && readNumOfFamiliesDead() < readMaxFamiliesDead() && readNumOfContainersShot() < readMaxNumOfShotContainers()){
        for(int k = 0; k < readNumOfCollectors() ; k++){
            if(readCollectorState(k) == 0 && readCollectorDead(k) != 1){
                if(readCollectorEnergy(k) < 25){
                    inc_collectors_shot(); // for info
                    decNumOfCollectorsEdited(); // for opengl
                    printf("\033[31mCollecter(%d) has been shot استشهد and need replacement\033[0m\n", k);
                    if(kill(openglPID, SIGUSR1) == -1){
                        perror("signal from collector to opengl failed\n");
                        exit(EXIT_FAILURE);
                    }
                    sleep(2);
                    setCollectedDead(k,1);
                    setCollectedHide(k,1);
                    //we need to send a message to splitting workers so one of them comes for replacement.
                    MQ_Struct.Dosmth = 1; // 1 is for replacement
                    MQ_Struct.CollectorIndex = k;
                    if (msgsnd(get_queue(CollectorToSplittingQueue), &MQ_Struct, sizeof(MQ_Struct), 0) == -1) {
                        perror("Queue Send to collecting worker for replacement Error\n");
                        exit(-1);
                    }
                    exit(0);
                }

                if(!check_queue_empty(get_queue(ContainerToCollecterQueue),2)) {
                    if (msgrcv(get_queue(ContainerToCollecterQueue), &ContainerInfoQueue, sizeof(ContainerInfoQueue),2,0) == -1) {
                        perror("Queue received from container error\n");
                        exit(-1);
                    }
                    if (read_dropped(ContainerInfoQueue.ContainerIndex) == 1 &&
                        read_height(ContainerInfoQueue.ContainerIndex) == 0 &&
                        read_content(ContainerInfoQueue.ContainerIndex) > 0 &&
                        read_collected(ContainerInfoQueue.ContainerIndex) == 0){

                        set_collected(ContainerInfoQueue.ContainerIndex, 1); // container i has been collected
                        setCollectorState(k, 1); // busy
                        decCollectorEnergy(k); // -20
                        sleep(4); // time for collector to collect the container
                        printf("\033[34mCollector(%d) has collected container(%d) from plane:%d, it's content:%d and going to the safe area, energy = %d\033[0m\n",
                               k, ContainerInfoQueue.ContainerBasedOnPlaneIndex, ContainerInfoQueue.PlaneIndex,
                               read_content(ContainerInfoQueue.ContainerIndex), readCollectorEnergy(k));
                        ContainerInfoQueue.type = 4;
                        ContainerInfoQueue.CollectorIndex = k;
                        //To inform the opengl that the container is in safe area
                        if (msgsnd(get_queue(CollectorToOpengl), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 0) == -1) {
                            perror("Queue Send to Opengl from collector Error\n");
                            exit(-1);
                        }
                        sleep(4); //time for collector to move the container back to safe area
                        //inform splitting worker & opengl that a container has been to safe area
                        ContainerInfoQueue.type = 3;
                        ContainerInfoQueue.CollectorIndex = k;
                        //To inform the splitter that the container is in safe area
                        if (msgsnd(get_queue(CollectorToSplittingQueue), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 0) == -1) {
                            perror("Queue Send to splitting worker for receiving wheat Error\n");
                            exit(-1);
                        }
                        setCollectorState(k, 0); // reset state, collector is free now
                    }
                }
            }
        }

    }
    printf("Collecting worker %d terminated..\n",getpid());
    return 0;
}