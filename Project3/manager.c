#include "header.h"
#include "shared_memories.h"
#include "message_queues.h"
#include "constants.h"
#include "functions.h"
int main(int argc, char** argv){
    if ( argc < 1){
        perror("Argc less than 1\n");
        exit(-1);
    }
    readfile("arguments.txt");
    int num_of_lines = NumOfPillProductionLines+NumOfLiquidProductionLines;

    while(Read_Info_Num_Of_Medicine_Produced() < MaxMedicineProduce &&
          Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec() < MaxLiquidMedicineOutOfSpec
          && Read_Info_Num_Of_Pill_Medicine_Out_Of_spec() < MaxPillMedicineOutOfSpec && readTimer() < SimulationTime){
        for(int i = 0; i < num_of_lines ; i++){
            if(Read_Liquid_Line_MedicineProduced(i) - Read_Liquid_Line_MedicinePackaged(i) - Read_Liquid_Line_outofspec(i)
            > SwapEmployeeWhenProductionExceedsPackagingBy){
                printf("Liquid Production Line(%d) needs employee\n",i);
                // look for Liquid line in which produced < packaged (for loop), send signal to it, so it can pause a thread(employee)
                // then send another signal to the line (i), so it can create a new thread(employee)
                for(int j = 0 ; j < NumOfLiquidProductionLines ; j++){
                    if(Read_Liquid_Line_MedicineProduced(j) <=  Read_Liquid_Line_MedicinePackaged(j)){
                        // now we found the line in which produced < packaged
                        printf("moving employee from liquid line(%d) --> liquid line(%d)",j,i);
                        // Send signal to pause a thread on line j
                        kill(Read_Liquid_Line_PID(j), SIGUSR1);
                        // Send signal to create a new thread on line i
                        kill(Read_Liquid_Line_PID(i), SIGUSR2);
                        break;
                    }
                    sleep(2);
                }
            }

            if(Read_Pill_Line_MedicineProduced(i) - Read_Pill_Line_MedicinePackaged(i) - Read_Pill_Line_outofspec(i)
            > SwapEmployeeWhenProductionExceedsPackagingBy){
                printf("Pill Production Line(%d) needs employee\n",i);
                // look for Pill line in which produced < packaged (for loop), send signal to it, so it can pause a thread(employee)
                // then send another signal to the line (i), so it can create a new thread(employee)
                for(int j = 0 ; j < NumOfPillProductionLines ; j++){
                    if((Read_Pill_Line_MedicineProduced(j) - Read_Pill_Line_MedicinePackaged(j) - Read_Pill_Line_outofspec(j)
                    < SwapEmployeeWhenProductionExceedsPackagingBy) && Read_Pill_Line_NumOfEmployees(j) > 0){
                        // now we found the line in which produced < packaged
                        printf("Moving employee from Pill line(%d) --> pill line(%d)\n",j,i);
                        // Send signal to pause a thread on line j
                        kill(Read_Pill_Line_PID(j), SIGUSR1);
                        sleep(1);
                        // Send signal to create a new thread on line i
                        kill(Read_Pill_Line_PID(i), SIGUSR2);
                        break;
                    }
                    sleep(5);
                }
            }
            sleep(5);
        }
    }

    return 0;
}

