
#ifndef LIBRARIES
#define LIBRARIES

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "stdbool.h"
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include<sys/shm.h>
#include "math.h"
#include <errno.h>
#include <GL/glut.h>
#include "constants.h"

typedef struct {
    pid_t pid;
    int team_id;
    int energy;
    int balls;
} Player;

typedef struct {
    int id;
    char name[20];
    Player players[NUM_PLAYERS_PER_TEAM + 1]; // Array of Player constructors, one extra for the team leader
} Team;




#endif