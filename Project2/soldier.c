#include "header.h"
#include "shared_memories.h"

int main(int argc, char** argv){

//    int sleep1 = 1;
    //while(sleep1); //for debugging

    if ( argc < 1){
        perror("Argc less than 1\n");
        exit(-1);
    }

    while(readTimer() < readSimulationTime() && readNumOfCollectorsShot() < readMaxCollectingWorkersShot() && readNumOfDistributersShot() < readMaxDistributionWorkersShot()
          && readNumOfFamiliesDead() < readMaxFamiliesDead() && readNumOfContainersShot() < readMaxNumOfShotContainers()){
        sleep(5);
        srand(time(NULL));
        int randomNumber = rand() % 12 + 1;
        if (randomNumber < 4){ //30% chance
            for (int i = 0; i <MAX_CONTAINERS; ++i) {
                if(read_dropped(i) == 1 && read_height(i) > 0 && read_shot(i) == 0){
                    set_shot(i,1); //set shot value of the container to 1 // message queue to opengl
                    if(read_height(i) >= 80 && read_height(i) < 100){ // 80<=height<100
                        setSoldierShot(1); // opengl
                        decrement_content(i,1);
                        printf("\033[31mContainer %d has been shot at height:%d, it's content now:%d\033[0m\n", i, read_height(i), read_content(i));
                        inc_containers_shot();
                        sleep(2);
                        setSoldierShot(0);
                        break;

                    }else if(read_height(i) >= 60 && read_height(i) < 80){ // 60<=height<80
                        setSoldierShot(1);
                        decrement_content(i,0.75);
                        printf("\033[31mContainer %d has been shot at height:%d, it's content now:%d\033[0m\n", i, read_height(i), read_content(i));
                        inc_containers_shot();
                        sleep(2);
                        setSoldierShot(0);
                        break;

                    }else if(read_height(i) >= 20 && read_height(i) < 60){ // 20<=height<60
                        setSoldierShot(1);
                        decrement_content(i,0.5);
                        printf("\033[31mContainer %d has been shot at height:%d, it's content now:%d\033[0m\n", i, read_height(i), read_content(i));
                        inc_containers_shot();
                        sleep(2);
                        setSoldierShot(0);
                        break;

                    }else if(read_height(i) > 0 && read_height(i) < 20){ // 0<height<20
                        setSoldierShot(1);
                        decrement_content(i,0.25);
                        printf("\033[31mContainer %d has been shot at height:%d, it's content now:%d\033[0m\n", i, read_height(i), read_content(i));
                        inc_containers_shot();
                        sleep(2);
                        setSoldierShot(0);
                        break;
                    }
                }
            }
        }
    }
    printf("Soldier job is finished الله لا يوفقه..\n");
    return 0;
}