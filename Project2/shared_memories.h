#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES

#include "header.h"
#include "semaphores.h"

#define MAX_CONTAINERS 150
#define plane_drop_shm_key 0x8421
#define Storage_sh_key 0x7941
#define INFO_sh_key 0x1444
#define StarvationRate_sh_key 0x1647
#define Plane_Info_sh_key 0x1787
#define Collector_sh_key 0x1348
#define Distributer_sh_key 0x2599
#define Splitter_sh_key 0x2411
#define Timer_sh_key 0x9874
#define PlaneNumOfContainers_sh_key 0x8484
#define Soldier_sh_key 0x9879

struct shared_memory1 { //Container
    int height;
    int content; //how many kg's
    int collected; // a worker started working with it or not
    int dropped; //if a plane has dropped it or not
    int shot; // if a soldier has shot the container or not
    double POSY;
    double POSX;
};
typedef struct shared_memory1* Plane_SM;

struct shared_memory2 { //Storage
    int storage;
};
typedef struct shared_memory2* Storage_SM;

struct shared_memory3 { // INFORMATION
    int Num_Of_Planes;
    int Num_Of_Collectors;
    int Num_Of_Splitters;
    int Num_Of_Distributers;
    int Num_Of_Families;
    int SimulationTime;
    int ReplaceDistributersWhenNumberLessThan;
    int num_of_collectors_shot;
    int num_of_distributers_shot;
    int num_of_containers_dropped; //new
    int num_of_containers_shot; //new
    int num_of_families_dead;
    // max values
    int maxNumOfPlanesCrashed;
    int maxNumOfShotContainers;
    int maxCollectingWorkersShot;
    int maxDistributionWorkersShot;
    int maxFamiliesDead;
    // to edit in opengl
    int Num_Of_CollectorsEdit;
    int Num_Of_SplittersEdit;
    int Num_Of_DistributersEdit;
};
typedef struct shared_memory3* INFO_SM;

struct shared_memory4{ //FAMILY
    int StarvationRate;
    int hide;
    double pos_x;
    double pos_y;
};
typedef struct shared_memory4* Family_SM;

struct shared_memory5{
    int state; // 1 --> refilling, 0 --> dropping
    int id;
    double pos_x;
    double pos_y;
    int num_ofContainers;
};
typedef struct shared_memory5* PlaneInfo_SM;

//temp
struct shared_memory6{
    int id;
    int state; // busy or not
    int energy;
    int dead;
    int hide;
    double POSX;
    double POSY;
};
typedef struct shared_memory6* Collector_SM;

struct shared_memory7{
    int id;
    int state;
    int energy;
    int dead;
    int hide;
    double POSX;
    double POSY;
};
typedef struct shared_memory7* Distributer_SM;

struct shared_memory8{
    int state;
    int hide;
    double POSX;
    double POSY;
};
typedef struct shared_memory8* Splitter_SM;

struct shared_memory9{
    int Time;
};
typedef struct shared_memory9* Timer_SM;

struct shared_memory10{
    int num_of_containers_dummy;
};
typedef struct shared_memory10* PlaneNumOfContainers_SM;

struct shared_memory11{
    int shot;
};
typedef struct shared_memory11* Soldier_SM;


////--------------------------------------------------Functions--------------------------------------------------

int get_shared_memory(int key, int memory_struct_size){

    // For get the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1){
        perror("Memory Get Error\n");
        exit(-1);
    }

    return sm_id;
}

////--------------------------------------------------shared memory 1--------------------------------------------------
void decrement_height(int height,int index){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].height -= height;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void decrement_content(int index, double fraction){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    int value = (int)tsm_mem[index].content * fraction;
    tsm_mem[index].content -= value;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void set_collected(int index,int collected){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].collected = collected;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void set_dropped(int index,int dropped){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].dropped = dropped;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void set_shot(int index, int shot){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].shot = shot;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int read_collected(int index){
    int Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].collected;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int read_dropped(int index){
    int Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].dropped;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int read_height(int index) {
    int Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].height;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int read_content(int index) {
    int Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].content;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int read_shot(int index){
    int Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].shot;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

double read_Container_POSX(int index){
    double Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

double read_Container_POSY(int index){
    double Value;
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = tsm_mem[index].POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void set_Container_POSX(int index, double posx){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].POSX = posx;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void set_Container_POSY(int index, double posy){
    int sh_id = get_shared_memory(plane_drop_shm_key, sizeof(Plane_SM) * MAX_CONTAINERS);
    sem_t* sm = get_semaphore(Container_SEMAPHORE);

    // For access the TSM / attach
    Plane_SM tsm_mem = (Plane_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].POSY = posy;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

////------------------------------------------------------------shared memory 2--------------------------------------------------------

void inc_Storage(int content){
    int sh_id = get_shared_memory(Storage_sh_key, sizeof(struct shared_memory2));
    sem_t* sm = get_semaphore(Storage_SEMAPHORE);

    // For access the TSM / attach
    Storage_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->storage += content;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void dec_Storage(){
    int sh_id = get_shared_memory(Storage_sh_key, sizeof(struct shared_memory2));
    sem_t* sm = get_semaphore(Storage_SEMAPHORE);

    // For access the TSM / attach
    Storage_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->storage -= 2;
    if(tsm_mem->storage < 0){
        tsm_mem->storage = 0;
    }
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int read_Storage(){
    int Value;
    int sh_id = get_shared_memory(Storage_sh_key, sizeof(struct shared_memory2));
    sem_t* sm = get_semaphore(Storage_SEMAPHORE);

    // For access the TSM / attach
    Storage_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    if(tsm_mem->storage < 0){
        tsm_mem->storage = 0;
    }
    Value = tsm_mem->storage;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

////--------------------------------------------------shared memory 3--------------------------------------------------
void inc_collectors_shot(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->num_of_collectors_shot++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_distributers_shot(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->num_of_distributers_shot++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_containers_dropped(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->num_of_containers_dropped++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_containers_shot(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->num_of_containers_shot++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void dec_Collecting_Workers(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Collectors--;
    if(tsm_mem->Num_Of_Collectors < 0){
        tsm_mem->Num_Of_Collectors = 0;
    }
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void dec_Distributing_Workers(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Distributers--;
    if(tsm_mem->Num_Of_Distributers < 0){
        tsm_mem->Num_Of_Distributers = 0;
    }
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void dec_Splitting_Workers(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Splitters--;
    if(tsm_mem->Num_Of_Splitters < 0){
        tsm_mem->Num_Of_Splitters = 0;
    }
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_Collecting_Workers(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Collectors++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_Distributing_Workers(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Distributers++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_Splitting_Workers(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Splitters++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void dec_SimulationTime(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->SimulationTime--;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void dec_Families(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_Families--;
    if(tsm_mem->Num_Of_Families < 0){
        tsm_mem->Num_Of_Families = 0;
    }
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void inc_Families_Dead(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->num_of_families_dead++;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readNumOfFamiliesDead(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->num_of_families_dead;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfPlanes(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    // For access the TSM / attach
    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_Planes;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfCollectors() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_Collectors;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfSplitters() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_Splitters;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfDistributers() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_Distributers;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfFamilies() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_Families;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readSimulationTime() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->SimulationTime;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readReplaceDistributersWhenNumberLessThan() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->ReplaceDistributersWhenNumberLessThan;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfCollectorsShot() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->num_of_collectors_shot;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfDistributersShot() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->num_of_distributers_shot;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumberOfContainersDropped() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->num_of_containers_dropped;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readNumOfContainersShot() {
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->num_of_containers_shot;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

void decNumOfCollectorsEdited(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_CollectorsEdit--;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void incNumOfCollectorsEdited(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_CollectorsEdit++;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void decNumOfSplittersEdited(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_SplittersEdit--;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void incNumOfSplittersEdited(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_SplittersEdit++;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void decNumOfDistributersEdited(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_DistributersEdit--;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}


void incNumOfDistributersEdited(){
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem->Num_Of_DistributersEdit++;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int ReadNumOfCollectersEdited(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_CollectorsEdit;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int ReadNumOfSplittersEdited(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_SplittersEdit;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int ReadNumOfDistributersEdited(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->Num_Of_DistributersEdit;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

// Max Values

int readMaxNumOfPlanesCrashed(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->maxNumOfPlanesCrashed;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readMaxNumOfShotContainers(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->maxNumOfShotContainers;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readMaxCollectingWorkersShot(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->maxCollectingWorkersShot;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readMaxDistributionWorkersShot(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->maxDistributionWorkersShot;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

int readMaxFamiliesDead(){
    int value;
    int sh_id = get_shared_memory(INFO_sh_key, sizeof(struct shared_memory3));
    sem_t* sm = get_semaphore(INFO_SEMAPHORE);

    INFO_SM tsm_mem = shmat(sh_id, NULL, 0);
    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = tsm_mem->maxFamiliesDead;
    detach_semaphore(sm);

    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

////--------------------------------------------------shared memory 4-------------------------------------------------------------------

int read_StarvationRate(int index){
    int Value;
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM FSM = (Family_SM)shmat(sh_id, NULL, 0);

    if (FSM == (void *) -1) {
            perror("Memory Attached Error\n");
            exit(-1);
        }

    attach_semaphore(sm);
    Value = FSM[index].StarvationRate;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(FSM) == -1) {
            perror("Memory Detached Error\n");
            exit(-1);
        }
    return Value;
};

void inc_StarvationRate(int index){
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM tsm_mem = (Family_SM)shmat(sh_id, NULL, 0);

    if (tsm_mem == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    tsm_mem[index].StarvationRate += 5;
    if(tsm_mem[index].StarvationRate > 100){
        tsm_mem[index].StarvationRate = 100;
    }
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(tsm_mem) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
};

void dec_StarvationRate(int index){
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Family[index].StarvationRate -= 10;
    if(Family[index].StarvationRate < 0 ){
        Family[index].StarvationRate = 0;
    }
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

double read_Family_POSX(int index){
    double posx;
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    posx = Family[index].pos_x;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return posx;
}

double read_Family_POSY(int index){
    double posy;
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    posy = Family[index].pos_y;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return posy;
}

void set_Family_POSX(int index, double posx){
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Family[index].pos_x = posx;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void set_Family_POSY(int index, double posy){
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Family[index].pos_y = posy;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void set_Family_Hide(int index, int hide){
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Family[index].hide = hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int read_Family_Hide(int index){
    int Value;
    int sh_id = get_shared_memory(StarvationRate_sh_key, sizeof(struct shared_memory4));
    sem_t* sm = get_semaphore(Family_SEMAPHORE);

    // For access the TSM / attach
    Family_SM Family = (Family_SM)shmat(sh_id, NULL, 0);

    if (Family == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Family[index].hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Family) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

////--------------------------------------------------shared memory 5--------------------------------------------------

int readPlaneNumOfContainers(int index){
    int Value;
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Plane[index].num_ofContainers;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

double readPlanePOSX(int index){
    int Value;
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Plane[index].pos_x;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

double readPlanePOSY(int index){
    int Value;
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Plane[index].pos_y;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void setPlaneNumOfContainers(int index, int value){
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].num_ofContainers = value;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void setPlanePOSX(int index, double value){
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].pos_x = value;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void setPlanePOSY(int index, double value){
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].pos_y = value;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readPlaneID(int index){
    int Value;
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Plane[index].id;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readPlaneState(int index){
    int Value;
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Plane[index].state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void SetPlaneState(int index, int state){
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].state = state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void setPlaneID(int index,int ID){
    int sh_id = get_shared_memory(Plane_Info_sh_key, sizeof(struct shared_memory5));
    sem_t* sm = get_semaphore(Plane_SEMAPHORE);

    // For access the TSM / attach
    PlaneInfo_SM Plane = (PlaneInfo_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].id = ID;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

////--------------------------------------------------shared memory 6--------------------------------------------------
void setCollectorState(int index, int state){
        int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
        sem_t* sm = get_semaphore(Collector_SEMAPHORE);

        // For access the TSM / attach
        Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

        if (Collector == (void *) -1) {
            perror("Memory Attached Error\n");
            exit(-1);
        }

        attach_semaphore(sm);
        Collector[index].state = state;
        detach_semaphore(sm);

        //detach (out of shared memory)
        if (shmdt(Collector) == -1) {
            perror("Memory Detached Error\n");
            exit(-1);
        }
};

int readCollectorState(int index){
    int Value;
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Collector[index].state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

int readCollectorEnergy(int index){
    int Value;
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Collector[index].energy;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

void decCollectorEnergy(int index){
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Collector[index].energy -= 20;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readCollectorDead(int index){
    int Value;
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Collector[index].dead;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;

}

void setCollectedDead(int index,int dead){
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Collector[index].dead = dead;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readCollectorHide(int index){
    int Value;
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Collector[index].hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;

}

void setCollectedHide(int index,int hide){
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Collector[index].hide = hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void SetCollectorPOSX(int index, double POSX){
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Collector[index].POSX = POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void SetCollectorPOSY(int index, double POSY){
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Collector[index].POSY = POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

double readCollectorPOSX(int index){
    double Value;
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Collector[index].POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

double readCollectorPOSY(int index){
    double Value;
    int sh_id = get_shared_memory(Collector_sh_key, sizeof(struct shared_memory6));
    sem_t* sm = get_semaphore(Collector_SEMAPHORE);

    // For access the TSM / attach
    Collector_SM Collector = (Collector_SM)shmat(sh_id, NULL, 0);

    if (Collector == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Collector[index].POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Collector) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

////--------------------------------------------------shared memory 7--------------------------------------------------

void setDistributerState(int index, int state){
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Distributer[index].state = state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
};

int readDistributerState(int index){
    int Value;
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Distributer[index].state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

int readDistributerEnergy(int index){
    int Value;
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Distributer[index].energy;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

void decDistributerEnergy(int index){
    srand(time(NULL));
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Distributer[index].energy -= rand()%16+1;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readDistributerDead(int index){
    int Value;
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Distributer[index].dead;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;

}

void setDistributerDead(int index,int dead){
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Distributer[index].dead = dead;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void SetDistributerPOSX(int index, double POSX){
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Distributer[index].POSX = POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void SetDistributerPOSY(int index, double POSY){
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Distributer[index].POSY = POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readDistributerHide(int index){
    int Value;
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Distributer[index].hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int setistributerHide(int index, int hide){
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Distributer[index].hide = hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

double readDistributerPOSX(int index){
    double Value;
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Distributer[index].POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

double readDistributerPOSY(int index){
    double Value;
    int sh_id = get_shared_memory(Distributer_sh_key, sizeof(struct shared_memory7));
    sem_t* sm = get_semaphore(Distributer_SEMAPHORE);

    // For access the TSM / attach
    Distributer_SM Distributer = (Distributer_SM)shmat(sh_id, NULL, 0);

    if (Distributer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Distributer[index].POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Distributer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

////--------------------------------------------------shared memory 8--------------------------------------------------

void SetSplitterPOSX(int index, double POSX){
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Splitter[index].POSX = POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void SetSplitterPOSY(int index, double POSY){
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Splitter[index].POSY = POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

double readSplitterPOSX(int index){
    double Value;
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Splitter[index].POSX;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

double readSplitterPOSY(int index){
    double Value;
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Splitter[index].POSY;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }

    return Value;
}

void setSplitterHide(int index, int Hide){
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Splitter[index].hide = Hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readSplitterHide(int index){
    int Value;
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Splitter[index].hide;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int readSplitterState(int index){
    int Value;
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Splitter[index].state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void setSplitterState(int index, int state){
    int sh_id = get_shared_memory(Splitter_sh_key, sizeof(struct shared_memory8));
    sem_t* sm = get_semaphore(Splitter_SEMAPHORE);

    // For access the TSM / attach
    Splitter_SM Splitter = (Splitter_SM)shmat(sh_id, NULL, 0);

    if (Splitter == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Splitter[index].state = state;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Splitter) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

////--------------------------------------------------shared memory 9--------------------------------------------------

int readTimer(){
    int Value;
    int sh_id = get_shared_memory(Timer_sh_key, sizeof(struct shared_memory9));
    sem_t* sm = get_semaphore(Timer_SEMAPHORE); // change me

    // For access the TSM / attach
    Timer_SM Timer = (Timer_SM)shmat(sh_id, NULL, 0);

    if (Timer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Timer->Time;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Timer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void incTimer(){
    int sh_id = get_shared_memory(Timer_sh_key, sizeof(struct shared_memory9));
    sem_t* sm = get_semaphore(Timer_SEMAPHORE); // change me

    // For access the TSM / attach
    Timer_SM Timer = (Timer_SM)shmat(sh_id, NULL, 0);

    if (Timer == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Timer->Time++;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Timer) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

////--------------------------------------------------shared memory 10--------------------------------------------------

void decNumOfContainersDummy(int index){
    int sh_id = get_shared_memory(PlaneNumOfContainers_sh_key, sizeof(struct shared_memory10));
    sem_t* sm = get_semaphore(PlaneNumOfContainers_SEMAPHORE);

    // For access the TSM / attach
    PlaneNumOfContainers_SM Plane = (PlaneNumOfContainers_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].num_of_containers_dummy--;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int readNumOfContainersDummy(int index){
    int value;
    int sh_id = get_shared_memory(PlaneNumOfContainers_sh_key, sizeof(struct shared_memory10));
    sem_t* sm = get_semaphore(PlaneNumOfContainers_SEMAPHORE);

    // For access the TSM / attach
    PlaneNumOfContainers_SM Plane = (PlaneNumOfContainers_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = Plane[index].num_of_containers_dummy;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

void setNumOfContainersDummy(int index, int numofcontainers){
    int sh_id = get_shared_memory(PlaneNumOfContainers_sh_key, sizeof(struct shared_memory10));
    sem_t* sm = get_semaphore(PlaneNumOfContainers_SEMAPHORE);

    // For access the TSM / attach
    PlaneNumOfContainers_SM Plane = (PlaneNumOfContainers_SM)shmat(sh_id, NULL, 0);

    if (Plane == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Plane[index].num_of_containers_dummy = numofcontainers;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Plane) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

////--------------------------------------------------shared memory 11--------------------------------------------------

int readSoldierShot(){
    int value;
    int sh_id = get_shared_memory(Soldier_sh_key, sizeof(struct shared_memory11));
    sem_t* sm = get_semaphore(Soldier_SEMAPHORE);

    // For access the TSM / attach
    Soldier_SM Soldier = (Soldier_SM)shmat(sh_id, NULL, 0);

    if (Soldier == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    value = Soldier->shot;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Soldier) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return value;
}

void setSoldierShot(int shot){
    int sh_id = get_shared_memory(Soldier_sh_key, sizeof(struct shared_memory11));
    sem_t* sm = get_semaphore(Soldier_SEMAPHORE);

    // For access the TSM / attach
    Soldier_SM Soldier = (Soldier_SM)shmat(sh_id, NULL, 0);

    if (Soldier == (void *) -1) {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Soldier->shot = shot;
    detach_semaphore(sm);

    //detach (out of shared memory)
    if (shmdt(Soldier) == -1) {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

#endif