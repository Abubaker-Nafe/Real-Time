#include "header.h"
#include "shared_memories.h"
#include "message_queues.h"
#include "constants.h"
#include "functions.h"

// semaphore for threads
sem_t ThreadSemaphore;

// struct used to hold the attributes for each employee
typedef struct {
    int emp_id;
    int line_index;
} employee;

// Global variables
void incchosenmedicinee();
int chosenmedicinee;
int flag1 = 0,flag2 = 0;
pthread_t Employees[15];
employee employees[15];  // Ensure enough space for maximum employees
int num_of_employees;
int LineIndex;
int chosenMedicineIndex;
int RightNumberOfPlasticContainersInPackagee;
const char* chosenMedicineChar;
// Array of strings representing medicine names
const char* medicineNames[10] = {
        "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T"
};

/// THREADS FUNCTION FOR HANDLING INSPECTION AND PACKAGING PROCESS
void* inspect_and_package_liquid(void* args) {
    employee* employees = (employee*) args;

    while (Read_Info_Num_Of_Medicine_Produced() < MaxMedicineProduce &&
           Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec() < MaxLiquidMedicineOutOfSpec
           && Read_Info_Num_Of_Pill_Medicine_Out_Of_spec() < MaxPillMedicineOutOfSpec && readTimer() < SimulationTime) {
        // testing if some of the liquid are not valid
        for (int i = 0; i < MaxMedicineProduce; i++) {
            sleep(chosenmedicinee);
            if (Read_Liquid_produced(i) == 1 && Read_Liquid_LineIndex(i) == employees->line_index && Read_Liquid_OutOfSpec(i) == 0
                && Read_Liquid_packaged(i) == 0 && Read_Liquid_beinginspected(i) == 0) {
                Set_Liquid_beinginspected(i,1);
                sleep(1);
                if (Read_Liquid_level(i) == 0) {
                    printf("\033[1;31mLiquid medicine(%d): %s is not within the required level by emp(%d) on line(%d)\033[0m\n"
                           , i, medicineNames[Read_Liquid_type(i)], employees->emp_id, employees->line_index);
                    Set_Liquid_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Liquid_Medicine_Out_Of_spec(); // increment liquid medicine outofspec overall
                    Inc_Liquid_Line_outofspec(LineIndex); // increment liquid medicine outofspec for this line
                } else if (Read_Liquid_color(i) == 0) {
                    printf("\033[1;31mLiquid medicine(%d): %s is not within the required color by emp(%d) on line(%d)\033[0m\n"
                           , i, medicineNames[Read_Liquid_type(i)], employees->emp_id, employees->line_index);
                    Set_Liquid_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Liquid_Medicine_Out_Of_spec(); // increment liquid medicine outofspec overall
                    Inc_Liquid_Line_outofspec(LineIndex); // increment liquid medicine outofspec for this line
                } else if (Read_Liquid_expdate(i) == 0) {
                    printf("\033[1;31mLiquid medicine(%d): %s has no exp date by emp(%d) on line(%d)\033[0m\n"
                           , i, medicineNames[Read_Liquid_type(i)], employees->emp_id, employees->line_index);
                    Set_Liquid_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Liquid_Medicine_Out_Of_spec(); // increment liquid medicine outofspec overall
                    Inc_Liquid_Line_outofspec(LineIndex); // increment liquid medicine outofspec for this line
                } else if (Read_Liquid_label(i) == 0) {
                    printf("\033[1;31mLiquid medicine(%d): %s has no label by emp(%d) on line(%d)\033[0m\n"
                           , i, medicineNames[Read_Liquid_type(i)], employees->emp_id, employees->line_index);
                    Set_Liquid_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Liquid_Medicine_Out_Of_spec(); // increment liquid medicine outofspec overall
                    Inc_Liquid_Line_outofspec(LineIndex); // increment liquid medicine outofspec for this line
                } else if (Read_Liquid_sealedcodition(i) == 0) {
                    printf("\033[1;31mLiquid medicine(%d): %s isn't sealed conditioned by emp(%d) on line(%d)\033[0m\n"
                           , i, medicineNames[Read_Liquid_type(i)], employees->emp_id, employees->line_index);
                    Set_Liquid_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Liquid_Medicine_Out_Of_spec(); // increment liquid medicine outofspec overall
                    Inc_Liquid_Line_outofspec(LineIndex); // increment liquid medicine outofspec for this line
                } else {
                    printf("Liquid Medicine(%d): %s has been packaged by employee(%d) on line(%d)\n"
                           , i, medicineNames[Read_Liquid_type(i)],employees->emp_id,employees->line_index);
                    Set_Liquid_packaged(i, 1);
                    Inc_Info_Num_Of_Liquid_Medicine_Packaged(); // increment Liquid medicine packaged overall
                    Inc_Info_Num_Of_Medicine_Packaged(); // increment medicine packaged overall
                    Inc_Liquid_Line_MedicinePackaged(LineIndex); // increment medicine packaged by this line
                }
            }
            sleep(1);
        }
    }
    return NULL;
}

void handle_pause_signal(int sig) {
    // Pause a thread
    // For example, canceling the last thread
    if (num_of_employees > 1) {
        pthread_cancel(Employees[num_of_employees - 1]);
        printf("Employee(%d) thread paused on liquid line(%d), num of employees=%d.\n",num_of_employees - 1,LineIndex,num_of_employees-1);
        num_of_employees--;
        Set_Liquid_Line_NumOfEmployees(LineIndex,Read_Liquid_Line_NumOfEmployees(LineIndex)-1);
        flag2=1;
        incchosenmedicinee();
    }
}

void handle_create_signal(int sig) {
    // Create a new thread
    if (num_of_employees < 5) {
        employee new_employee;
        new_employee.emp_id = num_of_employees;
        new_employee.line_index = LineIndex;
        if (pthread_create(&Employees[num_of_employees], NULL, inspect_and_package_liquid, (void*)&new_employee) != 0) {
            perror("Thread creation failed");
            exit(-1);
        }else{
            num_of_employees++;
            Set_Liquid_Line_NumOfEmployees(LineIndex,Read_Liquid_Line_NumOfEmployees(LineIndex)+1);
            printf("New employee thread created on liquid line(%d), num of employees=%d.\n",LineIndex,num_of_employees);
            flag1=1;
            incchosenmedicinee();
        }
    }
}

int main(int argc, char** argv){
//    int sleep1=1;
//    while(sleep1); // for debugging

    if ( argc < 2){
        perror("Argc less than 2\n");
        exit(-1);
    }
    readfile("arguments.txt");
    srand(time(NULL ) + getpid());   // Initialization, should only be called once.
    LineIndex = atoi(argv[1]); // might be needed when line needs employees
    chosenmedicinee = rand() % Read_Liquid_Line_speed(LineIndex)+2;
    num_of_employees = Read_Liquid_Line_NumOfEmployees(LineIndex); // get number of employees for this line
    RightNumberOfPlasticContainersInPackagee = RightNumberOfPlasticContainersInPackage; // to make it global

    // Initialize the semaphores
    if (sem_init(&ThreadSemaphore, 0, 1) != 0) {
        perror("Semaphore initialization failed");
        exit(-1);
    }

    // Register signal handlers
    signal(SIGUSR1, handle_pause_signal);
    signal(SIGUSR2, handle_create_signal);

    // Creating employee threads
    for (int i = 0; i < num_of_employees; i++) {
        employees[i].emp_id = i;
        employees[i].line_index = LineIndex;
        if (pthread_create(&Employees[i], NULL, inspect_and_package_liquid, (void*)&employees[i]) != 0) {
            perror("Thread creation failed");
            exit(-1);
        }
        sleep(1); //delay creating of threads so no confusion occurs
    }

    while (Read_Info_Num_Of_Medicine_Produced() < MaxMedicineProduce && Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec() < MaxLiquidMedicineOutOfSpec
            && Read_Info_Num_Of_Pill_Medicine_Out_Of_spec() < MaxPillMedicineOutOfSpec && readTimer() < SimulationTime){
        for(int i = 0; i < MaxMedicineProduce ; i++){
            if(Read_Liquid_produced(i) == 0){
                Set_Liquid_produced(i,1); // set the medicine produce value to 1
                Set_Liquid_LineIndex(i,LineIndex); // Connect/relate the medicine to this line
                Inc_Liquid_Line_MedicineProduced(LineIndex); // increment medicine produced by this line
                Inc_Info_Num_Of_Liquid_Medicine_Produced(); // increment liquid medicines produced overall
                Inc_Info_Num_Of_Medicine_Produced(); // increment medicines produced overall
                chosenMedicineIndex = rand() % NumOfDifferentMedicines; //choose type index
                chosenMedicineChar = medicineNames[chosenMedicineIndex]; // choose tye char
                Set_Liquid_type(i,chosenMedicineIndex); // Assign the index of the chosen type to the medicine produced
                printf("\033[1;32mLiquid(%d): %s has been produced by line: %d\033[0m\n",i, chosenMedicineChar, LineIndex);
                sleep(Read_Liquid_Line_speed(LineIndex));
            }
        }
    }

    // Wait for all threads to finish their work
    for (int i = 0; i < num_of_employees; i++) {
        pthread_join(Employees[i], NULL);
    }
    /// CLEANING UP
    // destroying the semaphore used
    sem_destroy(&ThreadSemaphore);
    return 0;
}

void incchosenmedicinee(){
    chosenmedicinee = (flag1 == 1) ? chosenmedicinee-2 : chosenmedicinee; //create
    chosenmedicinee = (flag2 == 1) ? chosenmedicinee+2 : chosenmedicinee; //pause
    flag1=0;
    flag2=0;
}
