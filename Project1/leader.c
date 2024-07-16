#include "header.h"
#include "constants.h"

#define LEADER_BALL_SIGNAL SIGUSR2 //signal used to pass the ball between leaders
char *hatem = "/tmp/cal_fifo";
int energy;
int team_id;
int position;
int balls;
pid_t opengl_pid;
int again_t1 = 0;
int again_t2 = 0;
void read_fifo();
void write_fifo();
void ball_received_handler(int signum);
void leaders_passing_handler(int signum);

int main(int argc, char* argv[]) {

    if (argc < 3) {
        perror("Argc less than 3\n");
        exit(-1);
    }

    //-----SETTING ATTRIBUTES-----
    energy = atoi(argv[1]) /100;
    team_id = atoi(argv[2]);
    position = atoi(argv[3]);
    opengl_pid = atoi(argv[4]);
    balls = 0;

    printf("Hi im a **leader** (PID %d) with energy = %d, team = %d, position = %d\n\n", getpid(), energy, team_id, position);

    if (position == 0 | position == 6) balls++;

    while (1)
    {
        if (balls > 0 && (again_t1 == 0 || again_t2 == 0) )
        {
            sleep(energy);
            if(kill(getpid()+1, SIGUSR1) == -1){
                perror("signal to player @1 failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }

            //write_fifo();
            balls--;
            if (position == 0){
                again_t1++;
            }
            else{
                again_t2++;
            }
            printf("Passing from %d to %d\n",getpid(),getpid() +1);
        }
        else if (balls > 0 && again_t1 > 0 ){
            sleep(energy);
            if(kill(getpid()+6, LEADER_BALL_SIGNAL) == -1){
                perror("signal to the other team leader failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            balls--;
            again_t1 = 0;
            printf("Passing from %d to %d\n",getpid(),getpid() +6);
        }
        else if (balls > 0 && again_t2 > 0 ){
            sleep(energy);
            if(kill(getpid()-6, LEADER_BALL_SIGNAL) == -1){
                perror("signal to the other team leader failed");
                printf("signal must be passed the parent\n");
                exit(EXIT_FAILURE);
            }
            //write_fifo();
            balls--;
            again_t2 = 0;
            printf("Passing from %d to %d\n",getpid(),getpid() +6);
        }
        else{

            signal(SIGUSR1,ball_received_handler);
            signal(LEADER_BALL_SIGNAL, leaders_passing_handler);
            pause();
            balls++;

        }

    }

    return 0;
}


void read_fifo(){
    int fd = open(hatem, O_RDONLY );
    if (fd == -1) {
        perror("Error opening fifo for reading");
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
        perror("Error opening fifo for writing ");
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
    printf("\n---> received signal from a player and started reading\n\n");
}
void leaders_passing_handler(int signum) {
    printf("\n---> received signal from a leader and started reading\n\n");
}