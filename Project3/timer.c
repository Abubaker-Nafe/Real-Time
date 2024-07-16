#include "header.h"
#include "shared_memories.h"
#include "constants.h"
#include "functions.h"

int main(int argc, char** argv){

    if ( argc < 1){
        perror("Argc less than 1\n");
        exit(-1);
    }
    readfile("arguments.txt");
    while(Read_Info_Num_Of_Medicine_Produced() < MaxMedicineProduce && Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec() < MaxLiquidMedicineOutOfSpec
          && Read_Info_Num_Of_Pill_Medicine_Out_Of_spec() < MaxPillMedicineOutOfSpec && readTimer() < SimulationTime){
        incTimer();
        sleep(1);
    }
    if(readTimer() < SimulationTime){
        printf("Program reached max threshold time (Simulation is over)\n");
    }
    return 0;
}