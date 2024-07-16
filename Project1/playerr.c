#include "header.h"
#include "constants.h"

#define PLAYER_BALL_SIGNAL SIGUSR1 //signal used to pass the ball between players
char *hatem = "/tmp/cal_fifo";
int energy;
int team_id;
int position;
int balls;
pid_t opengl_pid;
void read_fifo();
void write_fifo();
void ball_received_handler(int signum);
int players_pids[12];

int main( int argc, char** argv) {

    if (argc < 3) {
        perror("Argc less than 3\n");
        exit(-1);
    }

    pid_t last_player;//30 31 32 33 34 35
    pid_t leader;
    if (position == 1){
        last_player = getpid()+4;
        leader = getpid()-1;
    }
    else if(position == 2){
        last_player = getpid()+3;
        leader = getpid()-2;
    }
    else if(position == 3){
        last_player = getpid()+2;
        leader = getpid()-3;
    }
    else if(position == 4){
        last_player = getpid()+1;
        leader = getpid()-4;
    } else{
        last_player = getpid();
        leader = getpid()-5;
    }
//    for(int i=0; i<12; i++){ //330 331 332
//        if(i==0 || i==6)players_pids[i] = getpid()-1;
//        else{
//            //int temp = getpid();
//            players_pids[i] = getpid();
//        }
//        //printf("%d\n", players_pids[i]);
//    }
    //-----SETTING ATTRIBUTES-----
    energy = atoi(argv[1]) /100;
    team_id = atoi(argv[2]);
    position = atoi(argv[3]);
    opengl_pid = atoi(argv[4]);
    balls = 0;

    printf("Hi im (PID %d) with energy = %d, team = %d, position = %d\n", getpid(), energy, team_id, position);
    signal(PLAYER_BALL_SIGNAL,ball_received_handler);
    //read_fifo();
    if (position == 1 || position == 7){
        balls++;
    }
    remove(hatem);
    if (mkfifo(hatem,  S_IFIFO | 0777) == -1){
        perror("Fifo creation Error...\n");
        exit(-1);
    }
    while (1)
    {
        if ( (balls > 0) && (getpid()<=last_player-1 && team_id == 0))
        {
            sleep(energy);
            printf("\nopengl is %d\n", opengl_pid);
            if(kill(opengl_pid, SIGUSR1) == -1){
                perror("signal from player to opengl failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            if(kill((getpid()+1), PLAYER_BALL_SIGNAL) == -1){
                perror("sending signal failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            write_fifo();
            balls--;
            printf("writing from %d to %d",getpid(),getpid() +1);
        }
        else if ( (balls > 0) &&  (getpid()<=last_player-1 && team_id == 1) )
        {
            sleep(energy);
            printf("\nopengl is %d\n", opengl_pid);
            if(kill(opengl_pid, SIGUSR2) == -1){
                perror("signal from player to opengl failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            if(kill((getpid()+1), PLAYER_BALL_SIGNAL) == -1){
                perror("sending signal failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            write_fifo();
            balls--;
            printf("writing from %d to %d",getpid(),getpid() +1);
        }
        else if (balls > 0 && position == 5){
            sleep(energy);
            if(kill(leader, PLAYER_BALL_SIGNAL) == -1){
                perror("sending signal from player @5 back to the leader failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            balls--;
            printf("Sending the ball back from %d to the leader\n\n",getpid());
        }
        else if (balls > 0 && position == 11){
            sleep(energy);
            if(kill(leader, PLAYER_BALL_SIGNAL) == -1){
                perror("sending signal from player @5 back to the leader failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            balls--;
            printf("Sending the ball back from %d to the leader\n\n",getpid());
        }
        else{
            signal(PLAYER_BALL_SIGNAL,ball_received_handler);
            pause();
            read_fifo();
            balls++;

        }

    }



    return 0;
}

void read_fifo(){
    int fd = open(hatem, O_RDONLY );
    if (fd == -1) {
        perror("Error opening the fifo for reading");
        return;
    }

    char x[30];
    if (read(fd, &x, sizeof(char) * 30) == -1) {
        perror("Error reading from fifo");
    } else {
        printf("Message read successfully from pid = %d  id %d: %s\n", getpid()-1,getpid(),x);
    }
    close(fd);
}

void write_fifo(){
    int fd = open(hatem, O_WRONLY );
    if (fd == -1) {
        perror("Error opening file for writing ");
        return;
    }

    char x[30] = "hello";
    if (write(fd, &x, sizeof(char) * 30) == -1) {
        perror("Error writing to file");
    } else {
        printf("Message written successfully: %s from %d\n", x,getpid());
    }
    close(fd);
}

void ball_received_handler(int signum) {
    printf("received signal and started reading %d\n", getpid());
}