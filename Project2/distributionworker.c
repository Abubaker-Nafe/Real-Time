#include "header.h"
#include "shared_memories.h"

int main(int argc, char** argv){

    //int sleep1 = 1;
    //while(sleep1); //for debugging

    if ( argc < 2){
        perror("Argc less than 2\n");
        exit(-1);
    }

    ContainerInfo ContainerInfoQueue;
    pid_t openglPID = atoi(argv[1]); //pid of opengl

    while(readTimer() < readSimulationTime() && readNumOfDistributers() > 0 && readNumOfCollectorsShot() < readMaxCollectingWorkersShot()
          && readNumOfDistributersShot() < readMaxDistributionWorkersShot()
          && readNumOfFamiliesDead() < readMaxFamiliesDead() && readNumOfContainersShot() < readMaxNumOfShotContainers()){
        for(int k =0 ; k<readNumOfDistributers() ; k++){
            if(readDistributerState(k) == 0 && readDistributerDead(k) == 0){
                if(readDistributerEnergy(k) < 10){
                    if(kill(openglPID, SIGUSR1) == -1){
                        perror("signal from distibuter to opengl failed\n");
                        exit(EXIT_FAILURE);
                    }
                    setDistributerDead(k,1);
                    inc_distributers_shot();
                    decNumOfDistributersEdited();
                    printf("\033[31mDistributer(%d) with pid:%d has been shot استشهد, Number of Distribuers shot:%d\033[0m\n",
                           k, getpid(), readNumOfDistributersShot());
                    exit(0);
                    //done
                }
                if(read_Storage() > 0){
                    setDistributerState(k,1); // distributer is busy now
                    printf("\033[34mDistributer(%d) is going to collect 2 bags\033[0m\n", k);

                    // to opengl
                    ContainerInfoQueue.type = 6;
                    ContainerInfoQueue.DistributerIndex = k;
                    if(msgsnd(get_queue(DistributerToOpengl), &ContainerInfoQueue, sizeof(ContainerInfoQueue), 0) == -1) {
                        perror("Queue Send to Opengl from distributer Error\n");
                        exit(-1);
                    }

                    sleep(4); // time for distributer to move to safe area to collect the bag of wheat
                    dec_Storage(); // -2
                    decDistributerEnergy(k); //-15
                    printf("\033[34mDistributer(%d) with pid %d received 2 bags and going to give them to a needed family\033[0m\n", k, getpid());
                    // get the highest starvation rate between all the families

                    int highestRate,index;
                    // to get the first index of non-dead family
                    for(int j=0 ; k < readNumOfFamilies() ; j++){
                        if(read_Family_Hide(j) == 0){
                            highestRate = read_StarvationRate(j);
                            index = j;
                            break;
                        }
                    }
                    // now we have the rate and index of a living family
                    // to get the highest starvation rate between families and the index of the family(non-dead)
                    for (int i = 0; i < readNumOfFamilies(); ++i) {
                        if(read_Family_Hide(i) == 0){
                            if (read_StarvationRate(i) > highestRate) {
                                highestRate = read_StarvationRate(i);
                                index = i;
                            }
                        }
                    }
                    //now we have the highest starvation rate family with it's index
                    sleep(4); // time for distributer to take the bag from safe area to family
                    printf("2 Bags went to family %d with starvation rate %d\n",index, read_StarvationRate(index));
                    dec_StarvationRate(index); // -10
                    //now that the distributer has done his job, he is free for a new bag distribution
                    setDistributerState(k,0);
                }
            }
        }
    }
    printf("Distributing worker %d terminated..\n",getpid());
    return 0;
}
