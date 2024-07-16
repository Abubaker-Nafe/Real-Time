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
        if(strcmp(label,"numOfPlanes") == 0){
            numOfPlanes = atoi(str);
        }else if(strcmp(label,"minNumOfContainers") == 0){
            minNumOfContainers = atoi(str);
        }else if(strcmp(label,"maxNumOfContainers") == 0){
            maxNumOfContainers = atoi(str);
        }else if(strcmp(label,"MindropPeriod") == 0){
            MindropPeriod = atoi(str);
        }else if(strcmp(label,"MaxdropPeriod") == 0){
            MaxdropPeriod = atoi(str);
        }else if(strcmp(label,"MinrefillPeriod") == 0){
            MinrefillPeriod = atoi(str);
        }else if(strcmp(label,"MaxrefillPeriod") == 0){
            MaxrefillPeriod = atoi(str);
        }else if(strcmp(label,"numOfCollectingWorkers") == 0){
            numOfCollectingWorkers = atoi(str);
        }else if(strcmp(label,"numOfSplittingWorkers") == 0){
            numOfSplittingWorkers = atoi(str);
        }else if(strcmp(label,"numOfDistributionWorkers") == 0){
            numOfDistributionWorkers = atoi(str);
        }else if(strcmp(label,"numOfFamilies") == 0){
            numOfFamilies = atoi(str);
        }else if(strcmp(label,"replaceDistributionWorkersWhenLessThan") == 0){
            replaceDistributionWorkersWhenLessThan = atoi(str);
        }else if(strcmp(label,"maxThresholdTime") == 0){
            maxThresholdTime = atoi(str);
        }else if(strcmp(label,"maxNumOfPlanesCrashed") == 0){
            maxNumOfPlanesCrashed = atoi(str);
        }else if(strcmp(label,"maxNumOfShotContainers") == 0){
            maxNumOfShotContainers = atoi(str);
        }else if(strcmp(label,"maxCollectingWorkersShot") == 0){
            maxCollectingWorkersShot = atoi(str);
        }else if(strcmp(label,"maxDistributionWorkersShot") == 0){
            maxDistributionWorkersShot = atoi(str);
        }else if(strcmp(label,"maxFamiliesDead") == 0){
            maxFamiliesDead = atoi(str);
        }
    }

    fclose(file);
}

#endif