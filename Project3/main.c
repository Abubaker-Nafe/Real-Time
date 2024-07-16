#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"
#include "shared_memories.h"
#include "semaphores.h"

////Partners:
/*
 * Nafe Abubaker - team leader
 * Hatem Hussain
 * Ali Othman
 */

//shared memories
void create_shared_memory_pills(int numofmedicines,int PercentageOfFailure);
void clean_shared_memory_pills(int numofmedicines); ///
void create_shared_memory_liquid(int numofmedicines,int PercentageOfFailure);
void clean_shared_memory_liquid(int numofmedicines);
void create_shared_memory_Pill_line(int numoflines,int MinLineSpeed, int MaxLineSpeed); ///
void clean_shared_memory_Pill_line(int numoflines);
void create_shared_memory_Liquid_line(int numoflines,int MinLineSpeed, int MaxLineSpeed);////
void clean_shared_memory_Liquid_line(int numoflines);
void create_shared_memory_info();////
void clean_shared_memory_info();
void create_shared_memory_PillPackages(int numofmedicines);///
void clean_shared_memory_PillPackages(int numofmedicines);
void create_shared_memory_Timer();///
void clean_shared_memory_Timer();
void create_shared_memories(int MaxMedicineProduce, int NumOfPillProductionLines, int NumOfLiquidProductionLines, int NumOfDifferentMedicines);///
void clean_shared_memories(int MaxMedicineProduce, int NumOfPillProductionLines, int NumOfLiquidProductionLines, int NumOfDifferentMedicines);
//semaohores
void create_all_semaphores();
void create_semaphore(const char *key);
void clean_all_semaphores();
void clean_semaphore(const char *key);

void cleanup(int sig);

int main(int argc, char **argv){
//    int sleep1=1;
//    while(sleep1); // for debugging

    signal(SIGINT, cleanup); // if I wanted to stop the program using control+c, the handler cleans IPC's
    srand(time(NULL)); // Initialization, should only be called once.
    readfile("arguments.txt"); // read file for initilizing constants

////--------------------------------------------------initiation of IPC's---------------------------------------------------------------
    create_shared_memories(MaxMedicineProduce, NumOfPillProductionLines, NumOfLiquidProductionLines, NumOfDifferentMedicines);
    create_all_semaphores();

 ////--------------------------------------------------OpenGL---------------------------------------------------------------
    pid_t opengl = fork();
    if(opengl == -1){
        perror("No memory\n");
        exit(-5);
    } else if(opengl == 0) {
        execlp("./opengl", "opengl", NULL);
        perror("Execution Error!\n");
    }else{
        printf("The main file, create opengl with id: %d\n",opengl);
    }

////--------------------------------------------------Liquid Lines---------------------------------------------------------------

    char Index[12];
    pid_t LiquidLines[NumOfLiquidProductionLines];
    for (int i = 0; i < NumOfLiquidProductionLines; ++i){
        LiquidLines[i] = fork();
        if (LiquidLines[i] == -1){
            perror("No memory\n");
            exit(-5);
        }else if (LiquidLines[i] == 0){
            sprintf(Index, "%d", i);
            Set_Liquid_Line_PID(i,LiquidLines[i]);
            execlp("./liquidline", "liquidline", Index, NULL);
            perror("Execution Error!\n");
            break;
        }else{
            printf("The main file, created the Liquid Line %d with id: %d\n", i + 1, LiquidLines[i]);
        }
        sleep(1);
    }

////--------------------------------------------------Pill Lines---------------------------------------------------------------

    pid_t PillLines[NumOfPillProductionLines];
//    Set_Pill_Line_speed(0,30);
    Set_Pill_Line_speed(1,4); //test case
    Set_Pill_Line_speed(2,30); // test case
    for (int i = 0; i < NumOfPillProductionLines; ++i){
        PillLines[i] = fork();
        if (PillLines[i] == -1){
            perror("No memory\n");
            exit(-5);
        }else if (PillLines[i] == 0){
            sprintf(Index, "%d", i);
            execlp("./pillline", "pillline", Index, NULL);
            perror("Execution Error!\n");
            break;
        }else{
            Set_Pill_Line_PID(i,PillLines[i]);
            printf("The main file, created the Pill Line %d with id: %d\n", i + 1, PillLines[i]);
        }
        sleep(1);
    }

////-------------------------------------------------- Manager ---------------------------------------------------------------

    pid_t Manager = fork();
    if(Manager == -1){
        perror("No memory\n");
        exit(-5);
    }
    else if(Manager == 0) {
        execlp("./manager", "manager", NULL);
        perror("Execution Error!\n");
    }
    else{
        printf("The main file, create manager with id: %d\n",Manager);
    }

////----------------------------------------------------Timer---------------------------------------------------------------

    pid_t Timer = fork();
    if(Timer == -1){
        perror("No memory\n");
        exit(-5);
    } else if(Timer == 0) {
        execlp("./timer", "Timer", NULL);
        perror("Execution Error!\n");
    }

///-----------------------------------------------------------------------------------------------------------------------------------
    // let the parent(main process) wait his children before terminating
    int totalChildren = NumOfLiquidProductionLines + NumOfPillProductionLines+3; // opengl,timer,manager
    // Wait for all child processes to exit
    int status;
    pid_t pid;
    while (totalChildren > 0){
        pid = wait(&status); // Wait for any child process to end
        if (pid == -1){
            perror("");
            exit(EXIT_FAILURE);
        }
        //        printf("Process %d terminated with status %d\n", pid, WEXITSTATUS(status));
        totalChildren--; // Decrement the count of running children
    }

///------------------------------------------------------------Cleaning IPC's------------------------------------------------------------------
    clean_shared_memories(MaxMedicineProduce, NumOfPillProductionLines, NumOfLiquidProductionLines, NumOfDifferentMedicines);
    clean_all_semaphores();
    return 0;
}

void create_all_semaphores()
{
    create_semaphore(Employee_Semaphore);
    create_semaphore(Liquid_Semaphore);
    create_semaphore(Pill_Semaphore);
    create_semaphore(Line_Semaphore);
    create_semaphore(Info_Semaphore);
    create_semaphore(LiquidLine_Semaphore);
    create_semaphore(LiquidPackages_Semaphore);
    create_semaphore(PillPackages_Semaphore);
    create_semaphore(Timer_SEMAPHORE);
}
void create_semaphore(const char *key)
{
    sem_t *sem = sem_open(key, O_CREAT, 0777, 0);
    if (sem == SEM_FAILED)
    {
        perror("Semaphore Creation Error\n");
        exit(-1);
    }
    // When return -1 then wrong issue happened
    if (sem_post(sem) < 0)
    {
        perror("Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(sem);
}
void clean_all_semaphores()
{
    clean_semaphore(Employee_Semaphore);
    clean_semaphore(Liquid_Semaphore);
    clean_semaphore(Pill_Semaphore);
    clean_semaphore(Line_Semaphore);
    clean_semaphore(Info_Semaphore);
    clean_semaphore(LiquidLine_Semaphore);
    clean_semaphore(LiquidPackages_Semaphore);
    clean_semaphore(PillPackages_Semaphore);
    clean_semaphore(Timer_SEMAPHORE);
}
void clean_semaphore(const char *key)
{
    if (sem_unlink(key) < 0)
    {
        perror("Semaphore Unlink Error\n");
        exit(-1);
    }
}

void create_shared_memory_pills(int numofmedicines,int PercentageOfFailure)
{
    // set a random to all the values related to pill medecine
    int sm_id = shmget(Pill_Medecine_shm_key, sizeof(PillMedicine_SM) * numofmedicines, IPC_CREAT | 0777);
    if (sm_id == -1) {
        perror("shmget failed");
        exit(-1);
    }
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sm_id, NULL, 0);
    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error");
        exit(-1);
    }

    // initializing the array of Pills
    for (int i = 0; i < numofmedicines; i++)
    {
        int random = rand() % 101; // Random number between 0 and 100
        Pill[i].produced = 0;      // initialized to 0
        Pill[i].packaged = 0;      // initialized to 0
        Pill[i].ProductionLineIndex = -1;
        Pill[i].type = -1;
        Pill[i].beinginspected = 0;
        if (random < PercentageOfFailure){   // if the random number is below 20, then we set one of the flags to zero so the med becomes out of spec in future
            int flagIndex = rand() % 4; // Randomly select one of the four flags
            Pill[i].size = 1;
            Pill[i].full = 1;
            Pill[i].color = 1;
            Pill[i].expdate = 1;
            switch (flagIndex)
            {
            case 0:
                Pill[i].size = 0;
                break;
            case 1:
                Pill[i].full = 0;
                break;
            case 2:
                Pill[i].color = 0;
                break;
            case 3:
                Pill[i].expdate = 0;
                break;
            }
        }
        else
        {
            Pill[i].size = 1;
            Pill[i].full = 1;
            Pill[i].color = 1;
            Pill[i].expdate = 1;
        }
    }
}
void clean_shared_memory_pills(int numofmedicines)
{
    if (shmctl(get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM) * numofmedicines), IPC_RMID, 0) == -1)
    {
        perror("Pills SM Remove Error\n");
        exit(-1);
    }
}

void create_shared_memory_liquid(int numofmedicines,int PercentageOfFailure)
{
    // set a random to all the values related to pill medecine
    int sm_id = shmget(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM) * numofmedicines, IPC_CREAT | 0777);
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sm_id, NULL, 0);
    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error");
        exit(-1);
    }

    // initializing the array of liquids
    for (int i = 0; i < numofmedicines; i++)
    {
        int random = rand() % 101; // Random number between 0 and 100
        Liquid[i].produced = 0;    // Always initialized to 0
        Liquid[i].packaged = 0;    // Always initialized to 0
        Liquid[i].ProductionLineIndex = -1;
        Liquid[i].type = -1;
        Liquid[i].beinginspected = 0;
        if (random < PercentageOfFailure)
        {                               // if the random number is below 20, then we set one of the flags to zero so the med becomes out of spec in future
            int flagIndex = rand() % 5; // Randomly select one of the five flags
            Liquid[i].level = 1;
            Liquid[i].sealedCondition = 1;
            Liquid[i].color = 1;
            Liquid[i].label = 1;
            Liquid[i].expdate = 1;
            switch (flagIndex)
            {
            case 0:
                Liquid[i].level = 0;
                break;
            case 1:
                Liquid[i].sealedCondition = 0;
                break;
            case 2:
                Liquid[i].color = 0;
                break;
            case 3:
                Liquid[i].label = 0;
                break;
            case 4:
                Liquid[i].expdate = 0;
                break;
            }
        }
        else
        {
            Liquid[i].level = 1;
            Liquid[i].sealedCondition = 1;
            Liquid[i].color = 1;
            Liquid[i].label = 1;
            Liquid[i].expdate = 1;
        }

    }
}
void clean_shared_memory_liquid(int numofmedicines)
{
    if (shmctl(get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM) * numofmedicines), IPC_RMID, 0) == -1)
    {
        perror("Liquid SM Remove Error\n");
        exit(-1);
    }
}

void create_shared_memory_Pill_line(int numoflines,int MinLineSpeed, int MaxLineSpeed){
    int size = sizeof(Line_SM) * numoflines;
    if(size == 0){
        size = sizeof(Line_SM);
    }
    int sm_id = shmget(Line_shm_key, size, IPC_CREAT | 0777);
    Line_SM Line = (Line_SM)shmat(sm_id, NULL, 0);
    if (Line == (void *)-1)
    {
        perror("Memory Attached Error");
        exit(-1);
    }
    // initializing the array of employees
    for (int i = 0; i < numoflines; i++)
    {
//        int randomspeed = rand() % 10 + 5; // Random number between 5 and 15
        int randomspeed = MinLineSpeed + rand() % (MaxLineSpeed - MinLineSpeed + 1);
        Line[i].NumOfEmployees = NumOfEmployeesPerLine;
        Line[i].MedicinePackaged = 0;
        Line[i].MedicineProduced = 0;
        Line[i].speed = randomspeed;
        Line[i].PID = 0;
        Line[i].outofspec = 0;
        Line[i].NumOfinspected = 0;
    }

}
void clean_shared_memory_Pill_line(int numoflines)
{
    if (shmctl(get_shared_memory(Line_shm_key, sizeof(Line_SM) * numoflines), IPC_RMID, 0) == -1)
    {
        perror("pill line SM Remove Error\n");
        exit(-1);
    }
}

void create_shared_memory_Liquid_line(int numoflines,int MinLineSpeed, int MaxLineSpeed){
    int size = sizeof(LiquidLine_SM) * numoflines;
    if(size == 0){
        size = sizeof(LiquidLine_SM);
    }
    int sm_id = shmget(LiquidLine_shm_key, size, IPC_CREAT | 0777);
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sm_id, NULL, 0);
    if (Line == (void *)-1)
    {
        perror("Memory Attached Error");
        exit(-1);
    }
    // initializing the array of employees
    for (int i = 0; i < numoflines; i++)
    {
//        int randomspeed = rand() % 10 + 5; // Random number between 5 and 15
        int randomspeed = MinLineSpeed + rand() % (MaxLineSpeed - MinLineSpeed + 1);
        Line[i].NumOfEmployees = NumOfEmployeesPerLine;
        Line[i].MedicinePackaged = 0;
        Line[i].MedicineProduced = 0;
        Line[i].speed = randomspeed;
        Line[i].PID = 0;
        Line[i].outofspec = 0;
        Line[i].NumOfinspected = 0;
    }
}
void clean_shared_memory_Liquid_line(int numoflines)
{
    if (shmctl(get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM) * numoflines), IPC_RMID, 0) == -1)
    {
        perror("Liquid line SM Remove Error\n");
        exit(-1);
    }
}

void create_shared_memory_info()
{
    int sm_id = shmget(Info_shm_key, sizeof(Info_SM), IPC_CREAT | 0777);
    Info_SM Info = (Info_SM)shmat(sm_id, NULL, 0);
    if (Info == (void *)-1)
    {
        perror("Memory Attached Error");
        exit(-1);
    }
    Info->Num_Of_Medicine_Packaged = 0;
    Info->Num_Of_Liquid_Medicine_Packaged = 0;
    Info->Num_Of_Pill_Medicine_Packaged = 0;
    Info->Num_Of_Medicine_Produced = 0;
    Info->Num_Of_Liquid_Medicine_Produced = 0;
    Info->Num_Of_Pill_Medicine_Produced = 0;
    Info->Num_Of_Liquid_Medicine_Out_Of_spec = 0;
    Info->Num_Of_Pill_Medicine_Out_Of_spec = 0;
    Info->Num_Of_Pill_Medicine_Inspected = 0;
    Info->Num_Of_Liquid_Medicine_Inspected = 0;
}
void clean_shared_memory_info()
{
    if (shmctl(get_shared_memory(Info_shm_key, sizeof(Info_SM)), IPC_RMID, 0) == -1)
    {
        perror("Liquid SM Remove Error\n");
        exit(-1);
    }
}

void create_shared_memory_PillPackages(int numofmedicines){
    int sm_id = shmget(PillPackages_shm_key, sizeof(PillPackages_SM)*numofmedicines, IPC_CREAT | 0777);
    PillPackages_SM PillPackages = (PillPackages_SM)shmat(sm_id, NULL, 0);
    if (PillPackages == (void *)-1){
        perror("Memory Attached Error");
        exit(-1);
    }

    for (int i = 0; i < numofmedicines; i++){
        PillPackages[i].NumOfPackaged = 0;
        PillPackages[i].HowManyPackaged = 0;
    }
}
void clean_shared_memory_PillPackages(int numofmedicines){
    if (shmctl(get_shared_memory(PillPackages_shm_key, sizeof(PillPackages_SM) * numofmedicines), IPC_RMID, 0) == -1)
    {
        perror("Pill packages SM Remove Error\n");
        exit(-1);
    }
}

void create_shared_memory_Timer(){
    int sm_id = shmget(Timer_shm_key, sizeof(Timer_SM), IPC_CREAT | 0777);
    Timer_SM Timer = (Timer_SM)shmat(sm_id, NULL, 0);
    if (Timer == (void *)-1)
    {
        perror("Memory Attached Error");
        exit(-1);
    }
    Timer->Time = 0;
}
void clean_shared_memory_Timer(){
    if (shmctl(get_shared_memory(Timer_shm_key, sizeof(Timer_SM)), IPC_RMID, 0) == -1)
    {
        perror("Liquid SM Remove Error\n");
        exit(-1);
    }

}

void create_shared_memories(int MaxMedicineProduce, int NumOfPillProductionLines, int NumOfLiquidProductionLines, int NumOfDifferentMedicines){
    create_shared_memory_liquid(MaxMedicineProduce,LiquidFailurePercentage);
    create_shared_memory_pills(MaxMedicineProduce,PillFailurePercentage);
    create_shared_memory_Pill_line(NumOfPillProductionLines, MinLineSpeed, MaxLineSpeed);
    create_shared_memory_Liquid_line(NumOfLiquidProductionLines,MinLineSpeed, MaxLineSpeed);
    create_shared_memory_info();
    create_shared_memory_PillPackages(NumOfDifferentMedicines);
    create_shared_memory_Timer();
}
void clean_shared_memories(int MaxMedicineProduce, int NumOfPillProductionLines, int NumOfLiquidProductionLines, int NumOfDifferentMedicines){
    clean_shared_memory_liquid(MaxMedicineProduce);
    clean_shared_memory_pills(MaxMedicineProduce);
    clean_shared_memory_Pill_line(NumOfPillProductionLines);
    clean_shared_memory_Liquid_line(NumOfLiquidProductionLines);
    clean_shared_memory_info();
    clean_shared_memory_PillPackages(NumOfDifferentMedicines);
    clean_shared_memory_Timer();
}

void cleanup(int sig)
{
    clean_shared_memory_liquid(MaxMedicineProduce);
    clean_shared_memory_pills(MaxMedicineProduce);
    clean_shared_memory_info();
    clean_shared_memory_Pill_line(NumOfPillProductionLines);
    clean_shared_memory_Liquid_line(NumOfLiquidProductionLines);
    clean_shared_memory_PillPackages(NumOfDifferentMedicines);
    clean_shared_memory_Timer();
    clean_all_semaphores();
    exit(0);
}