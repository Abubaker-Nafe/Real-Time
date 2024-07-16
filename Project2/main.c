#include "header.h"
#include "constants.h"
#include "functions.h"
#include "message_queues.h"
#include "shared_memories.h"
#include "semaphores.h"

/*
 * Partners:
 * Nafe Abubaker - Team Leader
 * Hatem Hussain
 * Ali Othman
 * Liana Nassar
 */

void create_shared_memories();
void clean_shared_memories();

void create_shared_memory_plane_drop();
void clean_shared_memory_plane_drop();

void create_shared_memory_INFO();
void clean_shared_memory_INFO();

void create_shared_memory_STORAGE();
void clean_shared_memory_STORAGE();

void create_shared_memory_Family();
void clean_shared_memory_Family();

void create_shared_memory_PlaneInfo();
void clean_shared_memory_PlaneInfo();

void create_shared_memory_Collectors();
void clean_shared_memory_Collectors();

void create_shared_memory_Distributers();
void clean_shared_memory_Distributers();

void create_shared_memory_Splitters();
void clean_shared_memory_Splitters();

void create_shared_memory_Timer();
void clean_shared_memory_Timer();

void create_shared_memory_PlaneNumOfContainers(int NUM_OF_CONTAINERS[]);
void clean_shared_memory_PlaneNumOfContainers();

void create_shared_memory_Soldier();
void clean_shared_memory_Soldier();

//semaphores
void create_all_semaphores();
void create_semaphore(const char* key);
void clean_all_semaphores();
void clean_semaphore(const char* key);

//queues
void create_all_queues();
void create_queue(int key);
void clean_all_queue();
void clean_queue(int key);

void cleanup(int sig);

int main(int argc, char** argv){
    signal(SIGINT, cleanup);

//    int sleep1=1;
//    while(sleep1); //for debugging

    readfile("arguments.txt");

    srand(time(NULL));   // Initialization, should only be called once.
    int NUM_OF_CONTAINERS[numOfPlanes];
    for(int i = 0; i < numOfPlanes; i++){
        NUM_OF_CONTAINERS[i] = minNumOfContainers + rand() % (maxNumOfContainers - minNumOfContainers + 1); // # of containers on a plane
    }

    create_all_queues();
    create_all_semaphores();
    create_shared_memories();
    create_shared_memory_PlaneNumOfContainers(NUM_OF_CONTAINERS);

////-----------------------------------------openGL--------------------------------------------------------------------
    pid_t opengl = fork();
    char openglPID[32];
    sprintf(openglPID, "%d", opengl);

    if(opengl == -1){
        perror("No memory\n");
        exit(-5);
    } else if(opengl == 0) {
        execlp("./opengl", "opengl", NULL);
        perror("Execution Error!\n");
    }else{
        printf("The main file, create opengl with id: %d\n",opengl);
    }


////------------------------------------------------------Plane---------------------------------------------------
    pid_t planes[numOfPlanes];
    char num_containers_str[12], drop_period_strod_str[12], drop_period_str[12], refill_period_str[12], index[2];
    for (int i = 0; i < numOfPlanes; ++i) {
        planes[i] = fork();
        // the plane will continue from here
        if(planes[i] == -1){
            perror("No memory\n");
            exit(-5);
        } else if(planes[i] == 0){
            int REFILL_PERIOD = MinrefillPeriod + rand() % (MaxrefillPeriod - MinrefillPeriod + 1);
            int DROP_PERIOD = MindropPeriod + rand() % (MaxdropPeriod - MindropPeriod + 1);
            sprintf(index, "%d", i);
            sprintf(num_containers_str, "%d", NUM_OF_CONTAINERS[i]);
            sprintf(drop_period_str, "%d", DROP_PERIOD+i);
            sprintf(refill_period_str, "%d", REFILL_PERIOD);
            setPlaneID(i,i); //set plane ID
            setPlaneNumOfContainers(i,NUM_OF_CONTAINERS[i]); // set plane num of containers
            execlp("./plane", "Plane",num_containers_str,drop_period_str,refill_period_str,index, NULL);
            perror("Execution Error!\n");
            break;
        }else{
            printf("The main file, create the plane %d with id: %d\n",i ,planes[i]);
        }
    }

////-----------------------------------------------Collecting Workers---------------------------------------------------
    pid_t Collecting_Workers[numOfCollectingWorkers];
    for (int i = 0; i < numOfCollectingWorkers; ++i) {
        Collecting_Workers[i] = fork();
        if(Collecting_Workers[i] == -1){
            perror("No memory\n");
            exit(-5);
        } else if(Collecting_Workers[i] == 0){
            execlp("./collectingworker", "collectingworker",openglPID,NULL);
            perror("Execution Error!\n");
            break;
        }else{
            printf("The main file, create the Collecting_Worker %d with id: %d\n",i+1 ,Collecting_Workers[i]);
        }
    }


////-----------------------------------------------Distribution Workers---------------------------------------------------
    pid_t Distribution_Workers[numOfDistributionWorkers];
    for (int i = 0; i < numOfDistributionWorkers; ++i) {
        Distribution_Workers[i] = fork();
        //the plane will continue from here
        if(Distribution_Workers[i] == -1){
            perror("No memory\n");
            exit(-5);
        } else if(Distribution_Workers[i] == 0){
            execlp("./distributionworker", "distributionworker",openglPID, NULL);
            perror("Execution Error!\n");
            break;
        }else{
            printf("The main file, create the Distribution_Worker %d with id: %d\n",i+1 ,Distribution_Workers[i]);
        }
    }

////-----------------------------------------------Splitting Workers---------------------------------------------------
    pid_t Splitting_Workers[numOfSplittingWorkers];
    for (int i = 0; i < numOfSplittingWorkers; ++i) {
        Splitting_Workers[i] = fork();
        // the plane will continue from here
        if(Splitting_Workers[i] == -1){
            perror("No memory\n");
            exit(-5);
        } else if(Splitting_Workers[i] == 0){
            execlp("./splittingworker", "splittingworker",openglPID, NULL);
            perror("Execution Error!\n");
            break;
        }else{
            printf("The main file, create the Splitting_Worker %d with id: %d\n",i+1 ,Splitting_Workers[i]);
        }
    }

////-----------------------------------------------Families---------------------------------------------------
    pid_t FAMILIES[readNumOfFamilies()];
    char index1[2];
    for (int i = 0; i < readNumOfFamilies(); ++i) {
        FAMILIES[i] = fork();
        // the family child will continue from here
        if(FAMILIES[i] == -1){
            perror("No memory\n");
            exit(-5);
        } else if(FAMILIES[i] == 0){
            sprintf(index1, "%d", i);
            execlp("./families", "family",index1, NULL);
            perror("Execution Error!\n");
            break;
        }else{
            printf("The main file, create the family %d with id: %d\n",i+1 ,FAMILIES[i]);
        }
    }

////-----------------------------------------------Soldier---------------------------------------------------
    pid_t Soldier = fork();
    if(Soldier == -1){
        perror("No memory\n");
        exit(-5);
    } else if(Soldier == 0) {
        execlp("./soldier", "soldier", NULL);
        perror("Execution Error!\n");
    }else{
        printf("The main file, create soldier with id: %d\n",Soldier);
    }

    pid_t Timer = fork();
    if(Timer == -1){
        perror("No memory\n");
        exit(-5);
    } else if(Timer == 0) {
        execlp("./timer", "Timer", NULL);
        perror("Execution Error!\n");
    }else{
        printf("The main file, created timer with id: %d\n",Timer);
    }

    // +3 for the Soldier and opengl and timer
    int totalChildren = numOfPlanes + numOfCollectingWorkers + numOfSplittingWorkers + numOfDistributionWorkers + numOfFamilies + 3;
    // Wait for all child processes to exit
    int status;
    pid_t pid;
    while (totalChildren > 0) {
        pid = wait(&status); // Wait for any child process to end
        if (pid == -1) {
            perror("");
            exit(EXIT_FAILURE);
        }
        printf("Process %d terminated with status %d\n", pid, WEXITSTATUS(status));
        totalChildren--; // Decrement the count of running children
    }

    clean_all_queue();
    clean_shared_memories();
    clean_all_semaphores();
    return 0;
}

////---------------------------------------------functions-------------------------------------------------------------
void create_all_queues(){
    create_queue(PlaneToCollecterQueue);
    create_queue(CollectorToSplittingQueue);
    create_queue(SplittingToDistributerQueue);
    create_queue(PlaneToOpenglQueue);
    create_queue(ContainerToOpenglQueue);
    create_queue(ContainerToCollecterQueue);
    create_queue(CollectorToOpengl);
    create_queue(SplitterToDistibuter);
    create_queue(SplitterToOpengl);
    create_queue(DistributerToOpengl);

}
void create_queue(int key){
    if(msgget(key,IPC_CREAT | 0777) == -1){
        perror("MQ creation error!\n");
        exit(-1);
    }
}
void clean_all_queue()  {
    clean_queue(PlaneToCollecterQueue);
    clean_queue(CollectorToSplittingQueue);
    clean_queue(SplittingToDistributerQueue);
    clean_queue(PlaneToOpenglQueue);
    clean_queue(ContainerToOpenglQueue);
    clean_queue(ContainerToCollecterQueue);
    clean_queue(CollectorToOpengl);
    clean_queue(SplitterToDistibuter);
    clean_queue(SplitterToOpengl);
    clean_queue(DistributerToOpengl);
}
void clean_queue(int key){
    if(msgctl(get_queue(key) , IPC_RMID, 0 ) < 0){
        perror("Queue removal Error");
        exit(-1);
    } else{
        printf("queue removed successfully3\n");
    }
}

void create_all_semaphores(){
    create_semaphore(Container_SEMAPHORE);
    create_semaphore(INFO_SEMAPHORE);
    create_semaphore(Storage_SEMAPHORE);
    create_semaphore(Family_SEMAPHORE);
    create_semaphore(Plane_SEMAPHORE);
    create_semaphore(Collector_SEMAPHORE);
    create_semaphore(Distributer_SEMAPHORE);
    create_semaphore(Splitter_SEMAPHORE);
    create_semaphore(Timer_SEMAPHORE);
    create_semaphore(PlaneNumOfContainers_SEMAPHORE);
    create_semaphore(Soldier_SEMAPHORE);
}
void create_semaphore(const char* key){
    sem_t* sem = sem_open(key, O_CREAT, 0777, 0);
    if(sem == SEM_FAILED){
        perror("Semaphore Creation Error\n");
        exit(-1);
    }
    // When return -1 then wrong issue happened
    if (sem_post(sem) < 0){
        perror("Semaphore Release Error\n");
        exit(-1);
    }
    sem_close(sem);
}

void clean_all_semaphores(){
    clean_semaphore(Container_SEMAPHORE);
    clean_semaphore(INFO_SEMAPHORE);
    clean_semaphore(Storage_SEMAPHORE);
    clean_semaphore(Family_SEMAPHORE);
    clean_semaphore(Plane_SEMAPHORE);
    clean_semaphore(Collector_SEMAPHORE);
    clean_semaphore(Distributer_SEMAPHORE);
    clean_semaphore(Splitter_SEMAPHORE);
    clean_semaphore(Timer_SEMAPHORE);
    clean_semaphore(PlaneNumOfContainers_SEMAPHORE);
    clean_semaphore(Soldier_SEMAPHORE);
}
void clean_semaphore(const char* key){
    if(sem_unlink(key) < 0){
        perror("Semaphore Unlink Error\n");
        exit(-1);
    }else{
        printf("Semaphore cleaned success4\n");
    }
}

void create_shared_memories(){
    create_shared_memory_plane_drop();
    create_shared_memory_INFO();
    create_shared_memory_STORAGE();
    create_shared_memory_Family();
    create_shared_memory_PlaneInfo();
    create_shared_memory_Collectors();
    create_shared_memory_Distributers();
    create_shared_memory_Splitters();
    create_shared_memory_Timer();
}
void clean_shared_memories(){
    clean_shared_memory_plane_drop();
    clean_shared_memory_INFO();
    clean_shared_memory_STORAGE();
    clean_shared_memory_Family();
    clean_shared_memory_PlaneInfo();
    clean_shared_memory_Collectors();
    clean_shared_memory_Distributers();
    clean_shared_memory_Splitters();
    clean_shared_memory_Timer();
    clean_shared_memory_PlaneNumOfContainers();
}

////____________________________________________Shared memories___________________________________________________
void create_shared_memory_plane_drop(){
    int sm_id = shmget(plane_drop_shm_key, sizeof(struct shared_memory1) * MAX_CONTAINERS, IPC_CREAT | 0777);
    Plane_SM Container = (Plane_SM)shmat(sm_id,NULL,0);
    if (Container == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of containers
    for (int i = 0; i < MAX_CONTAINERS; i++) {
//        srand(time(NULL));
        Container[i].height = 100;
        Container[i].content = 5;
        Container[i].collected = 0;
        Container[i].dropped = 0;
        Container[i].shot = 0;
        Container[i].POSY = 0;
        Container[i].POSX = 0;
    }
}
void clean_shared_memory_plane_drop(){
    if(shmctl(get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM)),IPC_RMID, 0) == -1){
        perror("Plane SM Remove Error\n");
        exit(-1);
    }else{
        printf("Plane SM removed\n");
    }
}

void create_shared_memory_Family(){
    int sm_id = shmget(StarvationRate_sh_key, sizeof(struct shared_memory4), IPC_CREAT | 0777);
    Family_SM Family = (Family_SM)shmat(sm_id,NULL,0);
    if (Family == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of Families
    for (int i = 0; i < readNumOfFamilies(); i++) {
        int random = rand() % 21; //random between 0-20
        Family[i].StarvationRate = random;
        Family[i].hide = 0;
    }
}
void clean_shared_memory_Family(){

    if(shmctl(get_shared_memory(StarvationRate_sh_key, sizeof(Family_SM)*readNumOfFamilies()),IPC_RMID, 0) == -1){
        perror("Family SM Remove Error\n");
        exit(-1);
    }else{
        printf("Family SM removed\n");
    }
}

void create_shared_memory_INFO(){
    // For create the memory
    int sm_id = shmget(INFO_sh_key, sizeof(struct shared_memory3), IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sm_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }
    tsm_mem->Num_Of_Collectors = numOfCollectingWorkers;
    tsm_mem->Num_Of_Distributers = numOfDistributionWorkers;
    tsm_mem->Num_Of_Splitters = numOfSplittingWorkers;
    tsm_mem->Num_Of_Planes = numOfPlanes;
    tsm_mem->Num_Of_Families = numOfFamilies;
    tsm_mem->SimulationTime = maxThresholdTime;
    tsm_mem->ReplaceDistributersWhenNumberLessThan = replaceDistributionWorkersWhenLessThan;
    tsm_mem->maxNumOfPlanesCrashed = maxNumOfPlanesCrashed;
    tsm_mem->maxNumOfShotContainers = maxNumOfShotContainers;
    tsm_mem->maxCollectingWorkersShot = maxCollectingWorkersShot;
    tsm_mem->maxDistributionWorkersShot = maxDistributionWorkersShot;
    tsm_mem->maxFamiliesDead = maxFamiliesDead;
    tsm_mem->num_of_collectors_shot = 0;
    tsm_mem->num_of_distributers_shot = 0;
    tsm_mem->num_of_containers_dropped = 0;
    tsm_mem->num_of_containers_shot = 0;
    tsm_mem->num_of_families_dead = 0;
    tsm_mem->Num_Of_CollectorsEdit = numOfCollectingWorkers ;
    tsm_mem->Num_Of_SplittersEdit = numOfSplittingWorkers;
    tsm_mem->Num_Of_DistributersEdit = numOfDistributionWorkers;

    //detach
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}
void clean_shared_memory_INFO(){
    // clean the memory
    if(shmctl(get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3)),IPC_RMID, 0) == -1){
        perror("INFO SM Remove Error\n");
        exit(-1);
    }else{
        printf("INFO SM removed\n");
    }
}

void create_shared_memory_STORAGE(){
    // For create the memory
    int sm_id = shmget(Storage_sh_key, sizeof(struct shared_memory2), IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    // For access the TSM / attach
    Storage_SM tsm_mem = shmat(sm_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }
    tsm_mem->storage = 0;
    //detach
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}
void clean_shared_memory_STORAGE(){
    // clean the memory
    if(shmctl(get_shared_memory(Storage_sh_key, sizeof(struct shared_memory2)),IPC_RMID, 0) == -1){
        perror("Storage SM Remove Error\n");
        exit(-1);
    }else{
        printf("Storage SM removed\n");
    }
}

void create_shared_memory_PlaneInfo(){
    int sm_id = shmget(Plane_Info_sh_key, sizeof(struct shared_memory5), IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sm_id,NULL,0);
    if (Plane == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of containers
    for (int i = 0; i < numOfPlanes; i++) {
        Plane[i].pos_x = 0;
        Plane[i].pos_y = 0;
        Plane[i].id = i;
        Plane[i].state = 0;
    }
}
void clean_shared_memory_PlaneInfo(){
    // clean the memory
    if(shmctl(get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5)),IPC_RMID, 0) == -1){
        perror("PlaneINFO SM Remove Error\n");
        exit(-1);
    }else{
        printf("PlaneINFO SM removed\n");
    }
}

void create_shared_memory_Collectors(){
    int sm_id = shmget(Collector_sh_key, sizeof(struct shared_memory6), IPC_CREAT | 0777);
    Collector_SM Collector = (Collector_SM)shmat(sm_id,NULL,0);
    if (Collector == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of containers
    for (int i = 0; i < numOfCollectingWorkers+numOfSplittingWorkers+5 ; i++) {
        Collector[i].state = 0;
        Collector[i].id = i;
        Collector[i].energy = 100;
        Collector[i].hide = 0;
        Collector[i].dead = 0;
        Collector[i].POSY = 0;
        Collector[i].POSX = 0;
    }
}
void clean_shared_memory_Collectors(){
    // clean the memory
    if(shmctl(get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6)),IPC_RMID, 0) == -1){
        perror("Collector SM Remove Error\n");
        exit(-1);
    }else{
        printf("Collector SM removed\n");
    }
}

void create_shared_memory_Distributers(){
    int sm_id = shmget(Distributer_sh_key, sizeof(struct shared_memory7), IPC_CREAT | 0777);
    Distributer_SM Distributer = (Distributer_SM)shmat(sm_id,NULL,0);
    if (Distributer == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of containers
    for (int i = 0; i < numOfDistributionWorkers+numOfSplittingWorkers+5 ; i++) {
        Distributer[i].state = 0;
        Distributer[i].id = i;
        Distributer[i].energy = 100;
        Distributer[i].hide = 0;
        Distributer[i].dead = 0;
        Distributer[i].POSX = 0;
        Distributer[i].POSY = 0;
    }
}
void clean_shared_memory_Distributers(){
    // clean the memory
    if(shmctl(get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7)),IPC_RMID, 0) == -1){
        perror("Distributer SM Remove Error\n");
        exit(-1);
    }else{
        printf("Distributer SM removed\n");
    }
}

void create_shared_memory_Splitters(){
    int sm_id = shmget(Splitter_sh_key, sizeof(struct shared_memory8), IPC_CREAT | 0777);
    Splitter_SM Splitter = (Splitter_SM)shmat(sm_id,NULL,0);
    if (Splitter == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of containers
    for (int i = 0; i < numOfSplittingWorkers ; i++) {
        Splitter[i].POSX = 0;
        Splitter[i].POSY = 0;
        Splitter[i].hide = 0;
        Splitter[i].state = 0;
    }
}
void clean_shared_memory_Splitters(){
    // clean the memory
    if(shmctl(get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8)),IPC_RMID, 0) == -1){
        perror("Splitter SM Remove Error\n");
        exit(-1);
    }else{
        printf("Splitter SM removed\n");
    }
}

void create_shared_memory_Timer(){
    int sm_id = shmget(Timer_sh_key, sizeof(struct shared_memory9), IPC_CREAT | 0777);
    Timer_SM Timer = (Timer_SM)shmat(sm_id,NULL,0);
    if (Timer == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }

    Timer->Time = 0;
}
void clean_shared_memory_Timer(){
    // clean the memory
    if(shmctl(get_shared_memory(Timer_sh_key, sizeof(struct shared_memory9)),IPC_RMID, 0) == -1){
        perror("Timer SM Remove Error\n");
        exit(-1);
    }else{
        printf("Timer SM removed\n");
    }
}

void create_shared_memory_PlaneNumOfContainers(int NUM_OF_CONTAINERS[]){
    int sm_id = shmget(PlaneNumOfContainers_sh_key, sizeof(struct shared_memory10), IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Create Error\n");
        exit(-1);
    }

    PlaneNumOfContainers_SM Plane = (PlaneNumOfContainers_SM)shmat(sm_id,NULL,0);
    if (Plane == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }
    //initializing the array of containers
    for (int i = 0; i < numOfPlanes; i++) {
        Plane[i].num_of_containers_dummy = NUM_OF_CONTAINERS[i];
    }
}
void clean_shared_memory_PlaneNumOfContainers(){
    // clean the memory
    if(shmctl(get_shared_memory(PlaneNumOfContainers_sh_key, sizeof(struct shared_memory10)),IPC_RMID, 0) == -1){
        perror("PlaneNumOfContainers SM Remove Error\n");
        exit(-1);
    }else{
        printf("PlaneNumOfContainers SM removed\n");
    }
}

void create_shared_memory_Soldier(){
    int sm_id = shmget(Soldier_sh_key, sizeof(struct shared_memory11), IPC_CREAT | 0777);
    Soldier_SM Soldier = (Soldier_SM)shmat(sm_id,NULL,0);
    if (Soldier == (void*)-1) {
        perror("Memory Attached Error");
        exit(-1);
    }

    Soldier->shot = 0;
}
void clean_shared_memory_Soldier(){
    // clean the memory
    if(shmctl(get_shared_memory(Soldier_sh_key, sizeof(struct shared_memory11)),IPC_RMID, 0) == -1){
        perror("Soldier SM Remove Error\n");
        exit(-1);
    }else{
        printf("Soldier SM removed\n");
    }
}


void cleanup(int sig) {
    printf("Caught signal %d, cleaning up...\n", sig);
    // Command to remove message queue
    clean_all_queue();
    clean_shared_memories();
    clean_all_semaphores();
    // Exit after cleanup
    exit(0);
}