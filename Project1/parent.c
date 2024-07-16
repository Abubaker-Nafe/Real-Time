#include "header.h"
#include "constants.h"


Team teams[NUM_TEAMS];
int energies[12];
int generate_random_energy(int min_range, int max_range);
int generate_random_energy(int min_range, int max_range) {
    int value = (int)(min_range + (rand() % (max_range - min_range)));
    return value;
}
void create_team_processes(int team_id, pid_t parent_pid, pid_t opengl_pid) {
    // Creating the team
    Team team;
    printf("Team %d created\n", team_id);
    sprintf(team.name, "Team%d", team_id);
    team.id = team_id;

    // Create child processes for the players

    for (int i = 0; i < NUM_PLAYERS_PER_TEAM + 1; i++) {
        if (getpid() == parent_pid){

            pid_t player_pid = fork();
            team.players[i].pid = player_pid;

            if (player_pid == 0) {
                // Player process logic
                char  energy_str[2];
                char index_str[2];
                char team_id_str[2];
                char opengl_pid_str[10]; //80 80 76 90 66 78 80 80 76 90 66 78
                if(team_id == 0){
                    sprintf(energy_str, "%d", energies[i]); // Convert integer to string
                }
                if(team_id == 1){
                    sprintf(energy_str, "%d", energies[i+6]); // Convert integer to string
                }
//                sprintf(energy_str, "%d", energy); // Convert integer to string
                sprintf(team_id_str, "%d", team_id); // Convert integer to string
                sprintf(index_str, "%d", i); // Convert integer to string
                sprintf(opengl_pid_str, "%d", opengl_pid); // Convert integer to string
                if (i == 0){
                    execlp("./leader", "leader", energy_str, team_id_str, index_str, opengl_pid_str, NULL); // Execute player executable
                }
                else{
                    execlp("./playerr", "playerr", energy_str, team_id_str, index_str, opengl_pid_str, NULL); // Execute player executable
                }
                perror("execlp error ...\n"); // Print error if execlp fails
                exit(-7);
            } else if (player_pid < 0) {
                // Fork failed
                perror("fork failed");
                exit(EXIT_FAILURE);
            }
            else
            {
                sleep(1);
            }
        }
    }

    teams[team_id] = team;
}


int main() {
    printf("-------------- HI I AM THE ROOT PROCESS %d ENJOY THE GAME --------------\n", getpid());
    pid_t parent_pid = getpid();

    char energyStr[256] = ""; // Make sure this is large enough to hold the resulting string
    char buffer[20]; // Temporary buffer for each converted integer
    srand((unsigned) getpid());
    for(int i=0;i<12; i++){
        energies[i] = generate_random_energy(MIN_ENERGY_LEVEL, MAX_ENERGY_LEVEL);
        //printf("%d\n", energies[i]);
    }

    for (int i = 0; i < 12; i++) {
        sprintf(buffer, "%d", energies[i]); // Convert each integer to string
        strcat(energyStr, buffer); // Append the converted integer to the final string
        if (i < 11) { // Check if this is not the last element to avoid a trailing comma
            strcat(energyStr, ","); // Add a comma after each number except the last
        }
    }

    //-----THROWING THE FIRST TWO BALLS-----
    printf("\n--------------------------------------------------------------\n");
    printf("THROWING THE BALLS TO EACH TEAM LEADER\n");
    pid_t opengl_pid;
    if (getpid() == parent_pid){
        opengl_pid = fork();
        if(opengl_pid == 0) execlp("./opengl", "opengl", energyStr, NULL);
    }

    sleep(5);

    //-----CREATING THE TEAMS-----
    for (int team_id = 0; team_id < NUM_TEAMS; team_id++) {
        create_team_processes(team_id, parent_pid, opengl_pid);
    }
    wait(NULL);

    //-----JUST WAITING TO ENSURE EVERYTHING IS GOOD-----
    printf("\n\nWait a sec ...");
    sleep(3);
    printf("\n\n");










    //-----ENDING THE PARENT-----
    printf("waiting...\n");
    //sleep(5);
    printf("parent finished\n");
    return 0;
}
