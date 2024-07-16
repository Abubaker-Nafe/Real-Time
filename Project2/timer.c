#include "header.h"
#include "shared_memories.h"

int main(int argc, char** argv){

    if ( argc < 1){
        perror("Argc less than 1\n");
        exit(-1);
    }

    while(readTimer() < readSimulationTime()){
        if(readTimer() >= readSimulationTime() || readNumOfCollectorsShot() >= readMaxCollectingWorkersShot() || readNumOfDistributersShot() >= readMaxDistributionWorkersShot()
           || readNumOfFamiliesDead() >= readMaxFamiliesDead() || readNumOfContainersShot() >= readMaxNumOfShotContainers()){
            printf("Timer process %d terminated..\n",getpid());
            exit(1);
        }
        incTimer();
        sleep(1);
    }
    printf("Program reached max threshold time (Simulation is over)\n");
    return 0;
}