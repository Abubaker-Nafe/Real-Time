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
    QueueMessage MQ_Struct;
    ContainerInfo ContainerInfoQueue;
    int openglPID = atoi(argv[1]);
    while (readTimer() < readSimulationTime() && readNumOfSplitters() > 0 && readNumOfCollectorsShot() < readMaxCollectingWorkersShot()
           && readNumOfDistributersShot() < readMaxDistributionWorkersShot()
           && readNumOfFamiliesDead() < readMaxFamiliesDead() && readNumOfContainersShot() < readMaxNumOfShotContainers()) {
        for(int k =0 ; k < readNumOfSplitters() ; k++){
            if(readSplitterState(k) == 0 && readSplitterHide(k) == 0){
                if(!check_queue_empty(get_queue(CollectorToSplittingQueue),1)) {
                    if (msgrcv(get_queue(CollectorToSplittingQueue), &MQ_Struct, sizeof(MQ_Struct), 1, 0) == -1) {
                        perror("Queue received from collector for replacement error\n");
                        exit(-1);
                    }
                    // replace collector
                    setSplitterHide(k,1);
                    decNumOfSplittersEdited();
                    incNumOfCollectorsEdited(); // for opengl
                    inc_Collecting_Workers(); // for collectors
                    printf("\033[32mSplitter(%d) is going to replace collecting worker(%d)\033[0m\n", k,MQ_Struct.CollectorIndex);
                    execlp("./collectingworker", "collectingworker",openglPID, NULL);
                    exit(0);
                }
                if(!check_queue_empty(get_queue(CollectorToSplittingQueue),3)) {
                    // Received a container, split into 1kg bags
                    if (msgrcv(get_queue(CollectorToSplittingQueue), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 3, 0) == -1) {
                        perror("Queue received from collecter for receiving wheat error\n");
                        exit(-1);
                    }
                    setSplitterState(k,1);  // for opengl

                    // to opengl
                    ContainerInfoQueue.type = 5;
                    ContainerInfoQueue.SplitterIndex = k;
                    if(msgsnd(get_queue(SplitterToOpengl), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 0) == -1) {
                        perror("Queue Send to Opengl from splitter Error\n");
                        exit(-1);
                    }

                    sleep(4); // so that the splitting workers are splitting the containers into bags
                    inc_Storage(ContainerInfoQueue.content); // add the KG's from container to the safe area (Storage)
                    printf("\033[34mSplitting collector(%d) has splitted the container into 1kg bags, Storage content:%d\033[0m\n"
                           ,k, read_Storage());
                    decrement_content(ContainerInfoQueue.ContainerIndex,1);
                    setSplitterState(k,0); // free now
                }
                if(ReadNumOfDistributersEdited() < readReplaceDistributersWhenNumberLessThan()){
                    // replace Distributer
                    setSplitterHide(k,1);
                    decNumOfSplittersEdited();
                    incNumOfDistributersEdited();
                    inc_Distributing_Workers();
                    printf("\033[32mSplitting worker(%d) with pid=%d is going to replace Distributing worker\033[0m\n",k,getpid());
                    execlp("./distributionworker", "distributionWorker",openglPID, NULL);
                    exit(0);
                }
            }
        }
    }
    printf("Splitting worker %d terminated..\n",getpid());
    return 0;
}