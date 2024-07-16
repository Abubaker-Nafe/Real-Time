#ifndef SHARED_MEMORIES
#define SHARED_MEMORIES

#include "header.h"
#include "semaphores.h"

#define Pill_Medecine_shm_key 0x8366
#define Liquid_Medecine_shm_key 0x4567
#define Line_shm_key 0x2214
#define LiquidLine_shm_key 0x5544
#define Info_shm_key 0x8457
#define PillPackages_shm_key 0x5555
#define Timer_shm_key 0x1545

struct Info{
    int Num_Of_Medicine_Produced;
    int Num_Of_Pill_Medicine_Produced;
    int Num_Of_Liquid_Medicine_Produced;
    int Num_Of_Medicine_Packaged;
    int Num_Of_Pill_Medicine_Packaged;
    int Num_Of_Liquid_Medicine_Packaged;
    int Num_Of_Liquid_Medicine_Out_Of_spec;
    int Num_Of_Pill_Medicine_Out_Of_spec;
    int Num_Of_Pill_Medicine_Inspected;
    int Num_Of_Liquid_Medicine_Inspected;
    int Num_Of_containers;
}; // to track the info of the medicine (produced/packaged/outofspec)

struct Line{
    int MedicineProduced;
    int MedicinePackaged;
    int speed;
    int NumOfEmployees;
    int PID;
    int outofspec;
    int NumOfinspected;
}; // Info of pill line

struct LiquidLine{
    int MedicineProduced;
    int MedicinePackaged;
    int speed;
    int NumOfEmployees;
    int PID;
    int outofspec;
    int NumOfinspected;
}; // Info of pill line

struct employee{
    int busy;
    int LiquidOrPillLine; // 0 --> liquid , 1 --> pill
    int WokingInLinePID;
}; // not planning to use it yet

struct PillMedicine
{
    int id;
    int full;     // 1 --> full , 0 --> empty
    int size;     // 1 --> correct size , 0 --> false size
    int color;    // 1 --> correct color , 0 --> false color
    int expdate;  // 1 --> expdate printed , 0 --> expdate not printed
    int produced; // to inform that the medicine has been produced from the production line
    int packaged; // to inform that the medicine has been packaged from the employee
    int OutOfSpec;       // to know when the medicine has been threw out.
    int ProductionLineIndex; // to track the line which produced the medicine
    int beinginspected; // tracking if the med is being inspected by an employee or not so no conflict occur
    int type; // to assign the type of the med according to different types of med's
}; // Info of Pill medicines

struct LiquidMedicine
{
    int level;           // 1 --> within normal range , 0 --> outrage or beyond range
    int sealedCondition; // 1 --> properly sealed , 0 --> not properly sealed
    int color;           // 1 --> correct color , 0 --> false color
    int label;           // 1 --> correct label , 0 --> false label
    int expdate;         // 1 --> expdate printed , 0 --> expdate not printed
    int produced;        // to inform that the medicine has been produced from the production line
    int packaged;        // to inform that the medicine has been packaged from the employee
    int OutOfSpec;       // to know when the medicine has been threw out.
    int ProductionLineIndex; // to track the line which produced the medicine
    int beinginspected; // tracking if the med is being inspected by an employee or not so no conflict occur
    int type; // to assign the type of the med according to different types of med's
}; // Info of Liquid medicines

struct PillPackages{
    int NumOfPackaged;
    int HowManyPackaged;
};

struct Timer
{
    int Time;
};

typedef struct Info *Info_SM;
typedef struct Line *Line_SM;
typedef struct LiquidLine *LiquidLine_SM;
typedef struct PillMedicine *PillMedicine_SM;
typedef struct LiquidMedicine *LiquidMedicine_SM;
typedef struct PillPackages *PillPackages_SM;
typedef struct Timer *Timer_SM;

//// --------------------------------------------------------Functions----------------------------------------------------------

int get_shared_memory(int key, int memory_struct_size)
{

    // For get the memory
    int sm_id = shmget(key, memory_struct_size, IPC_CREAT | 0777);
    if (sm_id == -1)
    {
        perror("Memory Get Error\n");
        exit(-1);
    }

    return sm_id;
}

////--------------------------------------------------Pill Medicine shared memory----------------------------------------------------------

int Read_Pill_full(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].full;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_full(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].full = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_size(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].size;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_size(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].size = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_color(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].color;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_color(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].color = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_expdate(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].expdate;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_expdate(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].expdate = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_produced(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].produced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_produced(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].produced = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_packaged(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].packaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_packaged(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].packaged = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_OutOfSpec(int index)
{
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].OutOfSpec;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_OutOfSpec(int index, int Value)
{
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].OutOfSpec = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_LineIndex(int index){
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].ProductionLineIndex;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_LineIndex(int index, int Value){
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].ProductionLineIndex = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_beinginspected(int index){
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].beinginspected;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_beinginspected(int index, int Value){
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].beinginspected = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_type(int index){
    int Value;
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Pill[index].type;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_type(int index, int Value){
    int sh_id = get_shared_memory(Pill_Medecine_shm_key, sizeof(PillMedicine_SM));
    sem_t *sm = get_semaphore(Pill_Semaphore);

    // For access the TSM / attach
    PillMedicine_SM Pill = (PillMedicine_SM)shmat(sh_id, NULL, 0);

    if (Pill == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Pill[index].type = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Pill) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}
////--------------------------------------------------Liquid medicine shared memory----------------------------------------------------------

int Read_Liquid_level(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].level;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_level(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].level = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_sealedcodition(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].sealedCondition;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_sealedcodition(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].sealedCondition = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_color(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].color;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_color(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].color = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_expdate(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].expdate;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_expdate(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].expdate = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_label(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].label;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_label(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].label = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_produced(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].produced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_produced(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].produced = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_packaged(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].packaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_packaged(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].packaged = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_OutOfSpec(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].OutOfSpec;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_OutOfSpec(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].OutOfSpec = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_LineIndex(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].ProductionLineIndex;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_LineIndex(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Liquid[index].ProductionLineIndex = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_beinginspected(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].beinginspected;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_beinginspected(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }
    attach_semaphore(sm);
    Liquid[index].beinginspected = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_type(int index)
{
    int Value;
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Liquid[index].type;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_type(int index, int Value)
{
    int sh_id = get_shared_memory(Liquid_Medecine_shm_key, sizeof(LiquidMedicine_SM));
    sem_t *sm = get_semaphore(Liquid_Semaphore);

    // For access the TSM / attach
    LiquidMedicine_SM Liquid = (LiquidMedicine_SM)shmat(sh_id, NULL, 0);

    if (Liquid == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }
    attach_semaphore(sm);
    Liquid[index].type = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Liquid) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}
////--------------------------------------------------Pill Line shared memory----------------------------------------------------------

void Inc_Pill_Line_MedicineProduced(int index)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicineProduced++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Dec_Pill_Line_MedicineProduced(int index)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicineProduced--;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_MedicineProduced(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].MedicineProduced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Inc_Pill_Line_MedicinePackaged(int index)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicinePackaged++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Dec_Pill_Line_MedicinePackaged(int index)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicinePackaged--;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_MedicinePackaged(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].MedicinePackaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_Line_speed(int index, int Value)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].speed = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_speed(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].speed;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_Line_NumOfEmployees(int index, int Value)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    Set_Pill_Line_speed(index,Read_Pill_Line_speed(index)+1);

    attach_semaphore(sm);
    Line[index].NumOfEmployees = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_NumOfEmployees(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].NumOfEmployees;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Pill_Line_PID(int index, int Value)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].PID = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_PID(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].PID;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Pill_Line_outofspec(int index)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].outofspec++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_outofspec(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].outofspec;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Pill_Line_NumOfinspected(int index)
{
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].NumOfinspected++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Pill_Line_NumOfinspected(int index)
{
    int Value;
    int sh_id = get_shared_memory(Line_shm_key, sizeof(Line_SM));
    sem_t *sm = get_semaphore(Line_Semaphore);

    // For access the TSM / attach
    Line_SM Line = (Line_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].NumOfinspected;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

////--------------------------------------------------Liquid Line shared memory----------------------------------------------------------

void Inc_Liquid_Line_MedicineProduced(int index)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicineProduced++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Dec_Liquid_Line_MedicineProduced(int index)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicineProduced--;
    if(Line[index].MedicineProduced < 0){
        Line[index].MedicineProduced = 0;
    }
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_MedicineProduced(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].MedicineProduced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Liquid_Line_MedicinePackaged(int index)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicinePackaged++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Dec_Liquid_Line_MedicinePackaged(int index)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].MedicinePackaged--;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_MedicinePackaged(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].MedicinePackaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_Line_speed(int index, int Value)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].speed = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_speed(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].speed;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_Line_NumOfEmployees(int index, int Value)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].NumOfEmployees = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_NumOfEmployees(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].NumOfEmployees;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_Liquid_Line_PID(int index, int Value)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].PID = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_PID(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].PID;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Liquid_Line_outofspec(int index)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].outofspec++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_outofspec(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].outofspec;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Liquid_Line_NumOfinspected(int index)
{
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Line[index].NumOfinspected++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Liquid_Line_NumOfinspected(int index)
{
    int Value;
    int sh_id = get_shared_memory(LiquidLine_shm_key, sizeof(LiquidLine_SM));
    sem_t *sm = get_semaphore(LiquidLine_Semaphore);

    // For access the TSM / attach
    LiquidLine_SM Line = (LiquidLine_SM)shmat(sh_id, NULL, 0);

    if (Line == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Line[index].NumOfinspected;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Line) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

////--------------------------------------------------Info shared memory----------------------------------------------------------

void Inc_Info_Num_Of_Medicine_Produced()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Medicine_Produced++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Pill_Medicine_Produced()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Pill_Medicine_Produced++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Liquid_Medicine_Produced()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Liquid_Medicine_Produced++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Medicine_Packaged()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Medicine_Packaged++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Pill_Medicine_Packaged()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Pill_Medicine_Packaged++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Liquid_Medicine_Packaged()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Liquid_Medicine_Packaged++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Liquid_Medicine_Out_Of_spec()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Liquid_Medicine_Out_Of_spec++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

void Inc_Info_Num_Of_Pill_Medicine_Out_Of_spec()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Pill_Medicine_Out_Of_spec++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Info_Num_Of_Medicine_Produced()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Medicine_Produced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Pill_Medicine_Produced()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Pill_Medicine_Produced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Liquid_Medicine_Produced()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Liquid_Medicine_Produced;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Medicine_Packaged()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Medicine_Packaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Pill_Medicine_Packaged()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Pill_Medicine_Packaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Liquid_Medicine_Packaged()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Liquid_Medicine_Packaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Liquid_Medicine_Out_Of_spec;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

int Read_Info_Num_Of_Pill_Medicine_Out_Of_spec()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Pill_Medicine_Out_Of_spec;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Info_Num_Of_Pill_Inspected()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Pill_Medicine_Inspected++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Info_Num_Of_Pill_Medicine_Inspected()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Pill_Medicine_Inspected;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Info_Num_Of_Liquid_Inspected()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_Liquid_Medicine_Inspected++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Info_Num_Of_Liquid_Medicine_Inspected()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_Liquid_Medicine_Inspected;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Inc_Info_Num_Of_containers()
{
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Info->Num_Of_containers++;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_Info_Num_Of_containers()
{
    int Value;
    int sh_id = get_shared_memory(Info_shm_key, sizeof(Info_SM));
    sem_t *sm = get_semaphore(Info_Semaphore);

    // For access the TSM / attach
    Info_SM Info = (Info_SM)shmat(sh_id, NULL, 0);

    if (Info == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = Info->Num_Of_containers;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(Info) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

///--------------------------------------------------------------Pill Packages-------------------------------------------------

void Set_PillPackages_NumOfPackaged(int index, int Value)
{
    int sh_id = get_shared_memory(PillPackages_shm_key, sizeof(PillPackages_SM));
    sem_t *sm = get_semaphore(PillPackages_Semaphore);

    // For access the TSM / attach
    PillPackages_SM PillPackages = (PillPackages_SM)shmat(sh_id, NULL, 0);

    if (PillPackages == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    PillPackages[index].NumOfPackaged = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(PillPackages) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_PillPackages_NumOfPackaged(int index)
{
    int Value;
    int sh_id = get_shared_memory(PillPackages_shm_key, sizeof(PillPackages_SM));
    sem_t *sm = get_semaphore(PillPackages_Semaphore);

    // For access the TSM / attach
    PillPackages_SM PillPackages = (PillPackages_SM)shmat(sh_id, NULL, 0);

    if (PillPackages == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = PillPackages[index].NumOfPackaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(PillPackages) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}

void Set_PillPackages_HowManyPackaged(int index, int Value)
{
    int sh_id = get_shared_memory(PillPackages_shm_key, sizeof(PillPackages_SM));
    sem_t *sm = get_semaphore(PillPackages_Semaphore);

    // For access the TSM / attach
    PillPackages_SM PillPackages = (PillPackages_SM)shmat(sh_id, NULL, 0);

    if (PillPackages == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    PillPackages[index].HowManyPackaged = Value;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(PillPackages) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
}

int Read_PillPackages_HowManyPackaged(int index)
{
    int Value;
    int sh_id = get_shared_memory(PillPackages_shm_key, sizeof(PillPackages_SM));
    sem_t *sm = get_semaphore(PillPackages_Semaphore);

    // For access the TSM / attach
    PillPackages_SM PillPackages = (PillPackages_SM)shmat(sh_id, NULL, 0);

    if (PillPackages == (void *)-1)
    {
        perror("Memory Attached Error\n");
        exit(-1);
    }

    attach_semaphore(sm);
    Value = PillPackages[index].HowManyPackaged;
    detach_semaphore(sm);

    // detach (out of shared memory)
    if (shmdt(PillPackages) == -1)
    {
        perror("Memory Detached Error\n");
        exit(-1);
    }
    return Value;
}


///--------------------------------------------------------------Timer -------------------------------------------------

int readTimer(){
    int Value;
    int sh_id = get_shared_memory(Timer_shm_key, sizeof(Timer_SM));
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
    int sh_id = get_shared_memory(Timer_shm_key, sizeof(Timer_SM));
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


#endif
