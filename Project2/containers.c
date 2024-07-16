#include "header.h"
#include "shared_memories.h"

int main(int argc, char** argv){

    if ( argc < 4){
        perror("Argc less than 3\n");
        exit(-1);
    }

    int index = atoi(argv[1]); // get the value of the index as integer
    int PlaneIndex = atoi(argv[2]); // index of plane which dropped the container
    int ContainerBasedOnPlaneIndex = atoi(argv[3]); // index of the container based on the plane
    ContainerInfo ContainerInfoQueue;

    while(read_height(index)>0){
        if(readTimer() >= readSimulationTime() || readNumOfCollectorsShot() >= readMaxCollectingWorkersShot() || readNumOfDistributersShot() >= readMaxDistributionWorkersShot()
           || readNumOfFamiliesDead() >= readMaxFamiliesDead() || readNumOfContainersShot() >= readMaxNumOfShotContainers()){
            printf("Container process %d terminated..\n",getpid());
            exit(1);
        }
        decrement_height(20,index);
//        printf("\033[31mContainer(%d)  from plane(%d) is going down 20 meters, current height:%d\033[0m\n", ContainerBasedOnPlaneIndex,PlaneIndex,read_height(index));
        sleep(2); // each second container goes down 10 meters
    }
    printf("\033[31mContainer(%d) from plane(%d) has reached the ground with content = %d\033[0m\n", ContainerBasedOnPlaneIndex, PlaneIndex, read_content(index));
    ContainerInfoQueue.type = 1;
    ContainerInfoQueue.ContainerIndex = index;
    ContainerInfoQueue.ContainerBasedOnPlaneIndex = ContainerBasedOnPlaneIndex;
    ContainerInfoQueue.PlaneIndex = PlaneIndex;
    ContainerInfoQueue.content = read_content(index);
    decNumOfContainersDummy(PlaneIndex); // decrement number for opengl
    if (msgsnd(get_queue(ContainerToOpenglQueue), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 0) == -1) {
        perror("Queue send to opengl from container Error\n");
        exit(-1);
    }
    sleep(5);
    ContainerInfoQueue.type = 2;
    ContainerInfoQueue.ContainerIndex = index;
    ContainerInfoQueue.ContainerBasedOnPlaneIndex = ContainerBasedOnPlaneIndex;
    ContainerInfoQueue.PlaneIndex = PlaneIndex;
    ContainerInfoQueue.content = read_content(index);
    if (msgsnd(get_queue(ContainerToCollecterQueue), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 0) == -1) {
        perror("Queue Send to collector from container Error\n");
        exit(-1);
    }
    return 0;
}