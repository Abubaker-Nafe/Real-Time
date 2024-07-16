#include "header.h"
#include "message_queues.h"
#include "shared_memories.h"
#include "semaphores.h"

int main(int argc, char** argv){
//    int sleep1 = 1;
//    while(sleep1); //for debugging

    if ( argc < 4){
        perror("Argc less than 4\n");
        exit(-1);
    }

    QueueMessage MQ_Struct;
    PlaneRefilling PlaneRefillingQueue;

    int NUM_OF_CONTAINERS = atoi(argv[1]);
    int DROP_PERIOD = atoi(argv[2]); //drop container every (DROP_PERIOD) second
    int REFILL_PERIOD = atoi(argv[3]);
    int PlaneIndex = atoi(argv[4]);
    char ContainerIndex[32],PlaneINDEX[32],ContainerBasedOnPlaneIndex[32];

    while(readTimer() < readSimulationTime() && readNumOfCollectorsShot() < readMaxCollectingWorkersShot() && readNumOfDistributersShot() < readMaxDistributionWorkersShot()
          && readNumOfFamiliesDead() < readMaxFamiliesDead() && readNumOfContainersShot() < readMaxNumOfShotContainers()){
        for (int j = 0; j<NUM_OF_CONTAINERS; j++) {
            for(int i=0;i<MAX_CONTAINERS;i++){
                if(read_dropped(i) == 0){
                    set_dropped(i,1); // set that container has been dropped
                    decNumOfContainersDummy(PlaneIndex); // for opengl
                    pid_t Container = fork();
                    if(Container == -1){
                        perror("No memory\n");
                        exit(-5);
                    } else if(Container == 0) {
                        sprintf(ContainerIndex, "%d", i); //turning i into string to pass it to execlp
                        sprintf(PlaneINDEX, "%d", PlaneIndex);
                        sprintf(ContainerBasedOnPlaneIndex, "%d", j);
                        execlp("./containers", "container", ContainerIndex,PlaneINDEX,ContainerBasedOnPlaneIndex, NULL);
                        perror("Execution Error!\n");
                    }else{
                        printf("\nPlane %d dropping container %d with content:%d\n",PlaneIndex,j, read_content(i));
                        inc_containers_dropped(); // incrementing containers dropped for info
                        sleep(DROP_PERIOD);
                        break;
                    }
                }
            }
        }
        SetPlaneState(PlaneIndex,1); //done dropping and ready for refilling
        printf("Plane %d going for refill for %d seconds\n", PlaneIndex, REFILL_PERIOD);
        sleep(REFILL_PERIOD);
        SetPlaneState(PlaneIndex,0); //refilled and ready to drop
        setNumOfContainersDummy(PlaneIndex,NUM_OF_CONTAINERS); // opengl

    }
    printf("Plane %d Stopped working..\n",PlaneIndex);
    return 1;
}