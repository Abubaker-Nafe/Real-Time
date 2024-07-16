#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"


void readfile(char* filename){
    char line[200];
    char label[50];

    FILE *file;
    file = fopen(filename,"r");

    if(file == NULL){
        perror("the file does not exist!\n");
        exit(-2);
    }

    while(fgets(line, sizeof(line), file) != NULL)
    {
        char *str = strtok(line, "="); // First part is saved under "str"
        strncpy(label, str, sizeof(label)); // First part is saved under "label"

        str = strtok(NULL, "="); // the value is stored in "str"
        if(strcmp(label,"NumOfDifferentMedicines") == 0){
            NumOfDifferentMedicines = atoi(str);
        }else if(strcmp(label,"NumOfLiquidProductionLines") == 0){
            NumOfLiquidProductionLines = atoi(str);
        }else if(strcmp(label,"NumOfPillProductionLines") == 0){
            NumOfPillProductionLines = atoi(str);
        }else if(strcmp(label,"PillFailurePercentage") == 0){
            PillFailurePercentage = atoi(str);
        }else if(strcmp(label,"LiquidFailurePercentage") == 0){
            LiquidFailurePercentage = atoi(str);
        }else if(strcmp(label,"MaxMedicineProduce") == 0){
            MaxMedicineProduce = atoi(str);
        }else if(strcmp(label,"MaxLiquidMedicineOutOfSpec") == 0){
            MaxLiquidMedicineOutOfSpec = atoi(str);
        }else if(strcmp(label,"MaxPillMedicineOutOfSpec") == 0){
            MaxPillMedicineOutOfSpec = atoi(str);
        }else if(strcmp(label,"SimulationTime") == 0){
            SimulationTime = atoi(str);
        }else if(strcmp(label,"NumOfEmployeesPerLine") == 0){
            NumOfEmployeesPerLine = atoi(str);
        }else if(strcmp(label,"MinLineSpeed") == 0){
            MinLineSpeed = atoi(str);
        }else if(strcmp(label,"MaxLineSpeed") == 0){
            MaxLineSpeed = atoi(str);
        }else if(strcmp(label,"SwapEmployeeWhenProductionExceedsPackagingBy") == 0){
            SwapEmployeeWhenProductionExceedsPackagingBy = atoi(str);
        }else if(strcmp(label,"RightNumberOfPlasticContainersInPackage") == 0){
            RightNumberOfPlasticContainersInPackage = atoi(str);
        }
    }

    fclose(file);
}

#endif