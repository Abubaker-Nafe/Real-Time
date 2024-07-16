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
        "A", "B", "C", "D", "E",
        "F", "G", "H", "I", "J"
};

/// THREADS FUNCTION FOR HANDLING INSPECTION AND PACKAGING PROCESS
void* inspect_and_package_Pill(void* args) {
    employee* employees = (employee*) args;

    while (Read_Info_Num_Of_Medicine_Produced() < MaxMedicineProduce &&
           Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec() < MaxLiquidMedicineOutOfSpec
           && Read_Info_Num_Of_Pill_Medicine_Out_Of_spec() < MaxPillMedicineOutOfSpec && readTimer() < SimulationTime) {
        // testing if some of the liquid are not valid
        for (int i = 0; i < MaxMedicineProduce; i++) {
            sleep(chosenmedicinee); // delay to prevent employee conflicts
            if (Read_Pill_produced(i) == 1 && Read_Pill_LineIndex(i) == employees->line_index && Read_Pill_OutOfSpec(i) == 0
                && Read_Pill_packaged(i) == 0 && Read_Pill_beinginspected(i) == 0) {
                Set_Pill_beinginspected(i,1);
                sleep(1); // give some time to opengl
                if (Read_Pill_size(i) == 0) {
                    printf("\033[1;31mPill medicine(%d): %s is not within the required size by emp(%d) on line(%d)\033[0m\n"
                            , i, medicineNames[Read_Pill_type(i)], employees->emp_id, employees->line_index);
                    Set_Pill_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Pill_Medicine_Out_Of_spec(); // increment Pill medicine outofspec overall
                    Inc_Pill_Line_outofspec(LineIndex); // increment Pill medicine outofspec for this line
                } else if (Read_Pill_color(i) == 0) {
                    printf("\033[1;31mPill medicine(%d): %s is not within the required color by emp(%d) on line(%d)\033[0m\n"
                            , i, medicineNames[Read_Pill_type(i)], employees->emp_id, employees->line_index);
                    Set_Pill_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Pill_Medicine_Out_Of_spec(); // increment Pill medicine outofspec overall
                    Inc_Pill_Line_outofspec(LineIndex); // increment Pill medicine outofspec for this line
                } else if (Read_Pill_expdate(i) == 0) {
                    printf("\033[1;31mPill medicine(%d): %s has no exp date by emp(%d) on line(%d)\033[0m\n"
                            , i, medicineNames[Read_Pill_type(i)], employees->emp_id, employees->line_index);
                    Set_Pill_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Pill_Medicine_Out_Of_spec(); // increment Pill medicine outofspec overall
                    Inc_Pill_Line_outofspec(LineIndex); // increment Pill medicine outofspec for this line
                } else if (Read_Pill_full(i) == 0) {
                    printf("\033[1;31mPill medicine(%d): %s is not full by emp(%d) on line(%d)\033[0m\n"
                            , i, medicineNames[Read_Pill_type(i)], employees->emp_id, employees->line_index);
                    Set_Pill_OutOfSpec(i, 1);
                    Inc_Info_Num_Of_Pill_Medicine_Out_Of_spec(); // increment Pill medicine outofspec overall
                    Inc_Pill_Line_outofspec(LineIndex); // increment Pill medicine outofspec for this line
                }else {
                    printf("Pill Medicine(%d): %s has been packaged by employee(%d) on line(%d)\n"
                            , i, medicineNames[Read_Pill_type(i)],employees->emp_id,employees->line_index);
                    Set_Pill_packaged(i, 1);
                    Inc_Info_Num_Of_Pill_Medicine_Packaged(); // increment Pill medicine packaged overall
                    Inc_Info_Num_Of_Medicine_Packaged(); // increment medicine packaged overall
                    Inc_Pill_Line_MedicinePackaged(LineIndex); // increment medicine packaged by this line

                    Set_PillPackages_NumOfPackaged(Read_Pill_type(i),Read_PillPackages_NumOfPackaged(Read_Pill_type(i)) + 1);
                    if(Read_PillPackages_NumOfPackaged(Read_Pill_type(i)) >= RightNumberOfPlasticContainersInPackagee){
                        printf("Pill Medicine: %s has reached %d plastic containers, putting them into the carton and adding prescription\n",
                               medicineNames[Read_Pill_type(i)],RightNumberOfPlasticContainersInPackagee);
                        Set_PillPackages_NumOfPackaged(Read_Pill_type(i),
                                                       Read_PillPackages_NumOfPackaged(Read_Pill_type(i)) - RightNumberOfPlasticContainersInPackagee);
                        Set_PillPackages_HowManyPackaged(Read_Pill_type(i), Read_PillPackages_HowManyPackaged(Read_Pill_type(i)) + 1);
                        Inc_Info_Num_Of_containers();
                    }
                }
            }
            sleep(1);
        }
    }
    return NULL;
}

void handle_pause_signal(int sig) {
    // Pause a thread
    // For example, canceling the first thread
    if (num_of_employees > 1) {
        pthread_cancel(Employees[num_of_employees - 1]);
        printf("Employee(%d) thread paused on pill line(%d), num of employees=%d.\n",num_of_employees - 1,LineIndex,num_of_employees-1);
        num_of_employees--;
        Set_Pill_Line_NumOfEmployees(LineIndex,Read_Pill_Line_NumOfEmployees(LineIndex)-1);
        flag2=1;
        incchosenmedicinee();
    }
}

void handle_create_signal(int sig) {
    printf("Received create signal (SIGUSR2) for line %d\n", LineIndex);
    //Create a new thread
    employees[num_of_employees].emp_id = num_of_employees;
    employees[num_of_employees].line_index = LineIndex;

//    pthread_cancel(Employees[num_of_employees - 1]);
//    printf("Employee(%d) thread paused on pill line(%d), num of employees=%d.\n",num_of_employees - 1,LineIndex,num_of_employees-1);

    if (pthread_create(&Employees[num_of_employees], NULL, inspect_and_package_Pill, (void*)&employees[num_of_employees]) != 0) {
        perror("Thread creation failed");
        exit(-1);
    }else{
        num_of_employees++;
        Set_Pill_Line_NumOfEmployees(LineIndex,Read_Pill_Line_NumOfEmployees(LineIndex)+1);
        printf("New employee thread created on pill line(%d), num of employees=%d.\n",LineIndex,num_of_employees);
        flag1=1;
        incchosenmedicinee();
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
    srand(time(NULL));   // Initialization, should only be called once.
    LineIndex = atoi(argv[1]);
    chosenmedicinee = rand() % Read_Pill_Line_speed(LineIndex)+2;
    num_of_employees = Read_Pill_Line_NumOfEmployees(LineIndex); // get number of employees for this line
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
        if (pthread_create(&Employees[i], NULL, inspect_and_package_Pill, (void*)&employees[i]) != 0) {
            perror("Thread creation failed");
            exit(-1);
        }
        sleep(1); //delay creating of threads so no confusion occurs
    }

    while (Read_Info_Num_Of_Medicine_Produced() < MaxMedicineProduce && Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec() < MaxLiquidMedicineOutOfSpec
           && Read_Info_Num_Of_Pill_Medicine_Out_Of_spec() < MaxPillMedicineOutOfSpec && readTimer() < SimulationTime){
        for(int i = 0 ; i < MaxMedicineProduce ; i++){
            if(Read_Pill_produced(i) == 0){
                Set_Pill_produced(i,1); // set medicine type to produced
                Set_Pill_LineIndex(i,LineIndex); // connect medicine to the line that produced it
                Inc_Pill_Line_MedicineProduced(LineIndex); // increment medicine produced by this line
                Inc_Info_Num_Of_Pill_Medicine_Produced(); // Increment Pill medicine produced overall
                Inc_Info_Num_Of_Medicine_Produced(); // inc medicine produced overall
                chosenMedicineIndex = rand() % NumOfDifferentMedicines; // randomly choose the medicine type index
                chosenMedicineChar = medicineNames[chosenMedicineIndex]; // choose the medicine type name based in index
                Set_Pill_type(i,chosenMedicineIndex); // Assign the index of the chosen type to the medicine produced
                printf("\033[1;33mPill(%d): %s has been produced by line: %d\033[0m\n", i, chosenMedicineChar, LineIndex);
                sleep(Read_Pill_Line_speed(LineIndex));
            }
        }
    }
    printf("hi\n");

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

