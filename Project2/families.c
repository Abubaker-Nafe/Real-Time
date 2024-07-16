#include "header.h"
#include "shared_memories.h"

int main(int argc, char** argv){

    //int sleep1 = 1;
    //while(sleep1); //for debugging

    if ( argc < 2){
        perror("Argc less than 1\n");
        exit(-1);
    }

    int index = atoi(argv[1]); // get the index of the family

    while(read_StarvationRate(index) < 100 && read_Family_Hide(index) == 0){
        if(readTimer() >= readSimulationTime() || readNumOfCollectorsShot() >= readMaxCollectingWorkersShot() || readNumOfDistributersShot() >= readMaxDistributionWorkersShot()
           || readNumOfFamiliesDead() >= readMaxFamiliesDead() || readNumOfContainersShot() >= readMaxNumOfShotContainers()){
            printf("Family process %d terminated..\n",getpid());
            exit(1);
        }
        sleep(5);
        inc_StarvationRate(index);
    }
    printf("Family(%d) starved to death...\n",index);
    set_Family_Hide(index,1);
    inc_Families_Dead();
    return 0;
}