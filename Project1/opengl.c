#include "header.h"
#include "constants.h"

bool timerRunning = true; // Flag to indicate whether the timer is running or not

int counterBallInTeam1 = 0; // result team one
int counterBallInTeam2 = 0; // result team two
float timeround = 0; // Round amount of time
int roundNo = 1; // Round number
int energies[12];  // Assuming you know the size of the array

int counterTime1 = 0;
int counterTime2 = 0;


// Position of parent
float P_X = 650, P_Y= 615;

// 2-D array of position players of team one in pairs(x,y)
float Team1[6][2] = {
        {600,370}, // Position Leader
        {400,395}, // Position Player 1
        {400,195}, // Position player 2
        {250,95},  // Position Player 3
        {250,295}, // Position Player 4
        {250,495} // Position Player 5
};

// 2-D array of position players of team two in pairs(x,y)
float Team2[6][2] = {
        {700,370}, // Position Leader
        {900,395}, // Position Player 1
        {900,195}, // Position player 2
        {1050,95},  // Position Player 3
        {1050,295}, // Position Player 4
        {1050,495} // Position Player 5
};

float ballTeam1_x = 650; // Position of x-axis for ball team one
float ballTeam1_y = 615; // Position of y-axis for ball team one
float ballTeam2_x = 650; // Position of x-axis for ball team two
float ballTeam2_y = 615; // Position of y-axis for ball team two

// Flags for signal handler
int Leader_T1_flag = 0;
int Player1_T1_flag = 0;
int Player2_T1_flag = 0;
int Player3_T1_flag = 0;
int Player4_T1_flag = 0;
int Player5_T1_flag = 0;

int Leader_T2_flag = 0;
int Player1_T2_flag = 0;
int Player2_T2_flag = 0;
int Player3_T2_flag = 0;
int Player4_T2_flag = 0;
int Player5_T2_flag = 0;

void drawLine(float from_posX, float from_posY, float to_posX, float to_posY);
void penalty_centerKick(float pos_x, float pos_y, float radius);
void circle(float pos_x, float pos_y, float radius);
void D_box_arc_left(float pos_x, float pos_y);
void D_box_arc_right(float pos_x, float pos_y);
void playerHead(float pos_x, float pos_y, int teamNo);
void ball(float pos_x, float pos_y, int teamNo);
void playerBody(float pos_x, float pos_y, int teamNo);
void playerArms(float pos_x, float pos_y, float teamNo);
void playerLegs(float pos_x, float pos_y, float teamNo);
void player(float pos_x, float pos_y, float teamNo);
void playerNo(float pos_x, float pos_y, char *playerNo);
void matchResult(float pos_x, float pos_y, const char *resultFormat, int resultT1, int resultT2);
void teamWin(float pos_x,float pos_y,char *massage);
void drawStadium();
void drawPlayers();
void timer(int minutes);
void rounds(int roundNo);

void FromL1_To_L2(int value);
void FromL2_To_L1(int value);

void FromP5_To_L_T2(int value);
void FromP4_To_P5_T2(int value);
void FromP3_To_P4_T2(int value);
void FromP2_To_P3_T2(int value);
void FromP1_To_P2_T2(int value);
void FromL_To_P1_T2(int value);

void FromP5_To_L_T1(int value);
void FromP4_To_P5_T1(int value);
void FromP3_To_P4_T1(int value);
void FromP2_To_P3_T1(int value);
void FromP1_To_P2_T1(int value);
void FromL_To_P1_T1(int value);

void FromP_To_L1(int value);
void FromP_To_L2(int value);
void FromL2_To_P(int value);
void FromL1_To_P(int value);

void receiving_signal_handler(int signum);
void receiving_signal_handler1(int signum);
void receiving_signal_handler2(int signum);
void display();
void init();
int EnergysT1 [6]; // Energy's of team one (L, P1, P2, P3, P4, P5)
int EnergysT2 [6];// Energy's of team two (L, P1, P2, P3, P4, P5)
int main(int argc, char **argv) {
    timeround = rand() % 11;
    int i = 0;
    char *token = strtok(argv[1], ",");

    while (token != NULL && i < 12) {  // Parse up to 12 tokens
        energies[i++] = atoi(token);  // Convert token to integer and store in array
        token = strtok(NULL, ",");  // Get next token
    }

    for (int i=0; i<6; i++){
        EnergysT1[i] = energies[i];
        EnergysT2[i] = energies[i+6];
        printf("%d\t%d\n",EnergysT1[i], EnergysT2[i]);
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1300, 750);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Beach ball at Gaza sea");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutTimerFunc(0, FromP_To_L1, 0);
    glutTimerFunc(0, FromP_To_L2, 0);
    Leader_T1_flag = 1;
    Leader_T2_flag = 1;

    //signal(SIGINT,receiving_signal_handler);
    signal(SIGUSR1,receiving_signal_handler1);
    signal(SIGUSR2,receiving_signal_handler2);
    glutMainLoop();
    while(1){
        pause();
    }
    return 0;
}


//// Signal handler for Parent
//void receiving_signal_handler(int signum) {
//    if (signum == SIGINT) {
//        Leader_T1_flag = 1;
//        Leader_T2_flag = 1;
//        glutTimerFunc(0, FromP_To_L1, 0);
//        glutTimerFunc(0, FromP_To_L2, 0);
//
//    }
//}

// Signal handler for team one
void receiving_signal_handler1(int signum) {
    if (signum == SIGUSR1 && Leader_T1_flag == 1) { // From leader to player 1
        Leader_T1_flag = 0; // no active for leader
        Player1_T1_flag = 1; // activation player 1
        glutTimerFunc(0, FromL_To_P1_T1, 0);
    }
    else if (signum == SIGUSR1 && Player1_T1_flag == 1){ // From player 1 to player 2
        Player1_T1_flag = 0; // no active player 1
        Player2_T1_flag = 1; // activation player 2
        glutTimerFunc(0, FromP1_To_P2_T1, 0);
    }
    else if (signum == SIGUSR1 && Player2_T1_flag == 1){ // From player 2 to player 3
        Player2_T1_flag = 0; // no active player 2
        Player3_T1_flag = 1; // activation player 3
        glutTimerFunc(0, FromP2_To_P3_T1, 0);
    }
    else if (signum == SIGUSR1 && Player3_T1_flag == 1){ // From player 3 to player 4
        Player3_T1_flag = 0; // no active player 3
        Player4_T1_flag = 1; // activation player 4
        glutTimerFunc(0, FromP3_To_P4_T1, 0);
    }
    else if (signum == SIGUSR1 && Player4_T1_flag == 1){ // From player 4 to player 5
        Player4_T1_flag = 0; // no active player 4
        Player5_T1_flag = 1; // activation player 5
        glutTimerFunc(0, FromP4_To_P5_T1, 0);
    }
    else if (signum == SIGUSR1 && Player5_T1_flag == 1){ // From player 5 to Leader
        Player5_T1_flag = 0; // no active player 5
        glutTimerFunc(0, FromP5_To_L_T1, 0);
    }
    else { // From leader to Leader
        glutTimerFunc(0, FromL1_To_L2, 0);
    }
}

// Signal handler for team two
void receiving_signal_handler2(int signum) {
     if (signum == SIGUSR2 && Leader_T2_flag == 1) { // From leader to player 1
        Leader_T2_flag = 0; // no active for leader
        Player1_T2_flag = 1; // activation player 1
        glutTimerFunc(0, FromL_To_P1_T2, 0);
    }
    else if (signum == SIGUSR2 && Player1_T2_flag == 1){ // From player 1 to player 2
        Player1_T2_flag = 0; // no active player 1
        Player2_T2_flag = 1; // activation player 2
        glutTimerFunc(0, FromP1_To_P2_T2, 0);
    }
    else if (signum == SIGUSR2 && Player2_T2_flag == 1){ // From player 2 to player 3
        Player2_T2_flag = 0; // no active player 2
        Player3_T2_flag = 1; // activation player 3
        glutTimerFunc(0, FromP2_To_P3_T2, 0);
    }
    else if (signum == SIGUSR2 && Player3_T2_flag == 1){ // From player 3 to player 4
        Player3_T2_flag = 0; // no active player 3
        Player4_T2_flag = 1; // activation player 4
        glutTimerFunc(0, FromP3_To_P4_T2, 0);
    }
    else if (signum == SIGUSR2 && Player4_T2_flag == 1){ // From player 4 to player 5
        Player4_T2_flag = 0; // no active player 4
        Player5_T2_flag = 1; // activation player 5
        glutTimerFunc(0, FromP4_To_P5_T2, 0);
    }
    else if (signum == SIGUSR2 && Player5_T2_flag == 1){ // From player 5 to Leader
        Player5_T2_flag = 0; // no active player 5
        glutTimerFunc(0, FromP5_To_L_T2, 0);
    }
    else { // From leader to Leader
        glutTimerFunc(0, FromL2_To_L1, 0);
    }

}

void drawLine(float from_posX, float from_posY, float to_posX, float to_posY) {
    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(from_posX, from_posY);
    glVertex2f(to_posX, to_posY);
    glEnd();
}

void penalty_centerKick(float pos_x, float pos_y, float radius) {
    float theta;
    glLoadIdentity();
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 360; i++) {
        theta = i * (3.1416 / 180);
        glVertex2f(pos_x + radius * cos(theta), pos_y + radius * sin(theta));
    }
    glEnd();
}

void circle(float pos_x, float pos_y, float radius) {
    float theta;
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 1.0, 1.0);
    for (int i = 0; i < 360; i++) {
        theta = i * (3.1416 / 180);
        glVertex2f(pos_x + radius * cos(theta), pos_y + radius * sin(theta));
    }
    glEnd();
}

void D_box_arc_left(float pos_x, float pos_y) {
    float theta;
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_LINE_LOOP);
    glColor3f(1, 1, 1);
    for (int i = 0; i < 360; i++) {
        if (i <= 90 || i >= 270) {
            theta = i * (3.1416 / 180);
            glVertex2f(pos_x + 40 * cos(theta), pos_y + 40 * sin(theta));
        }
    }
    glEnd();
    glPopMatrix();
}

void D_box_arc_right(float pos_x, float pos_y) {
    float theta;
    glPushMatrix();
    glLoadIdentity();
    glBegin(GL_LINE_LOOP);
    glColor3f(1, 1, 1);
    for (int i = 0; i < 360; i++) {
        if (!(i <= 90 || i >= 270)) {
            theta = i * (3.1416 / 180);
            glVertex2f(pos_x + 40 * cos(theta), pos_y + 40 * sin(theta));
        }
    }
    glEnd();
    glPopMatrix();
}

void playerHead(float pos_x, float pos_y, int teamNo) {
    glPushMatrix();
    glLoadIdentity();
    float theta;
    glBegin(GL_POLYGON);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // Red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // Yellow color
    } else {
        glColor3f(0, 0, 1); // Blue color
    }
    for (int i = 0; i < 360; i++) {
        theta = i * (3.1416 / 180);
        glVertex2f(pos_x + 20 * cos(theta), pos_y + 20 * sin(theta));
    }
    glEnd();
}

void ball(float pos_x, float pos_y, int teamNo) {
    glPushMatrix();
    glLoadIdentity();
    float theta;
    glBegin(GL_POLYGON);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // Red color
    }else {
        glColor3f(0, 0, 1); // Blue color
    }
    for (int i = 0; i < 360; i++) {
        theta = i * (3.1416 / 180);
        glVertex2f(pos_x + 10 * cos(theta), pos_y + 10 * sin(theta));
    }
    glEnd();
}

void playerBody(float pos_x, float pos_y, int teamNo) {
    glLoadIdentity();
    glBegin(GL_POLYGON);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // Red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // Yellow color
    } else {
        glColor3f(0, 0, 1); // Blue color
    }
    glVertex2f(pos_x - 20, pos_y - 20);
    glVertex2f(pos_x + 20, pos_y - 20);
    glVertex2f(pos_x + 20, pos_y - 80);
    glVertex2f(pos_x - 20, pos_y - 80);
    glEnd();
}

void playerArms(float pos_x, float pos_y, float teamNo) {
    glLoadIdentity();
    //Left arm
    glBegin(GL_LINE_LOOP);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // Red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // Yellow color
    } else if (teamNo == 2.5 || teamNo == 1.5) {
        glColor3f(0, 1, 0); // Green color (Team leader)
    } else {
        glColor3f(0, 0, 1); // Blue color
    }
    glVertex2f(pos_x - 20, pos_y - 30);
    glVertex2f(pos_x - 50, pos_y - 50);
    glEnd();

    //Right arm
    glBegin(GL_LINE_LOOP);
    if (teamNo == 1) {
        glColor3f(1, 0, 0); // Red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // Yellow color
    } else {
        glColor3f(0, 0, 1); // Blue color
    }
    glVertex2f(pos_x + 20, pos_y - 30);
    glVertex2f(pos_x + 50, pos_y - 50);
    glEnd();
}

void playerLegs(float pos_x, float pos_y, float teamNo) {
    glLoadIdentity();
    //Right Leg
    glBegin(GL_LINE_LOOP);
    if (teamNo == 1 || teamNo == 1.5) {
        glColor3f(1, 0, 0); // Red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // Yellow color
    }else {
        glColor3f(0, 0, 1); // Blue color
    }
    glVertex2f(pos_x + 10, pos_y - 80);
    glVertex2f(pos_x + 10, pos_y - 100);
    glEnd();

    //Left Leg
    glBegin(GL_LINE_LOOP);
    if (teamNo == 1 || teamNo == 1.5) {
        glColor3f(1, 0, 0); // Red color
    } else if (teamNo == 3) {
        glColor3f(1, 1, 0); // Yellow color
    } else {
        glColor3f(0, 0, 1); // Blue color
    }
    glVertex2f(pos_x - 10, pos_y - 80);
    glVertex2f(pos_x - 10, pos_y - 100);
    glEnd();
}

void player(float pos_x, float pos_y, float teamNo) {
    playerHead(pos_x, pos_y, teamNo);
    playerBody(pos_x, pos_y, teamNo);
    playerArms(pos_x, pos_y, teamNo);
    playerLegs(pos_x, pos_y, teamNo);
}

void energyPlayer(float pos_x, float pos_y, int energy) {

    char EnergyPlayer[20];
    // Convert the energy value to a string
    sprintf(EnergyPlayer, "%d", energy);

    glColor3b(0, 0, 0);
    glRasterPos2d(pos_x - 10, pos_y + 25);
    int len = (int)strlen(EnergyPlayer);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, EnergyPlayer[i]);
    }
}

void playerNo(float pos_x, float pos_y, char *playerNo) {
    glColor3b(0, 0, 0);
    glRasterPos2d(pos_x - 6, pos_y - 50);
    int len = (int)strlen(playerNo);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, playerNo[i]);
    }
}

void matchResult(float pos_x, float pos_y, const char *resultFormat, int resultT1, int resultT2) {
    char matchResult[100]; // Assuming the maximum length of the result string is 100 characters
    sprintf(matchResult, resultFormat, resultT1, resultT2);

    glColor3b(0, 0, 0);
    glRasterPos2d(pos_x, pos_y);

    int len = (int)strlen(matchResult);
    for (int i = 0; i < len; i++) {
        if (matchResult[i] == '\n') {
            // Move to the next line
            pos_y -= 20; // Adjust as needed based on font size
            glRasterPos2d(pos_x, pos_y);
        } else {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, matchResult[i]);
        }
    }
}

void teamWin(float pos_x,float pos_y,char *massage){
    glColor3b(127, 127, 127);
    glRasterPos2d(pos_x, pos_y);

    int len = (int)strlen(massage);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, massage[i]);
    }
}
void drawStadium() {
    glLoadIdentity();

    // Border stadium
    drawLine(50, 50, 50, 700); // Left border stadium vertical line
    drawLine(1250, 50, 1250, 700); // Right border stadium vertical line
    drawLine(50, 50, 1250, 50); // Lower border stadium horizontal line
    drawLine(50, 700, 1250, 700); // Upper border stadium horizontal line

    // Left side D-Box
    drawLine(50, 200, 300, 200); // Lower horizontal line
    drawLine(50, 550, 300, 550); // Upper horizontal line
    drawLine(300, 200, 300, 550); // vertical line

    // Left side penalty box
    drawLine(50, 250, 225, 250); // Lower horizontal line
    drawLine(50, 500, 225, 500); // Upper horizontal line
    drawLine(225, 250, 225, 500); // vertical line

    // Right side D-Box
    drawLine(1250, 200, 1000, 200); // Lower horizontal line
    drawLine(1250, 550, 1000, 550); // Upper horizontal line
    drawLine(1000, 200, 1000, 550); // vertical line

    // Right side penalty box
    drawLine(1250, 250, 1075, 250); // Lower horizontal line
    drawLine(1250, 500, 1075, 500); // Upper horizontal line
    drawLine(1075, 250, 1075, 500); // vertical line

    D_box_arc_left(300, 375); // Left side
    D_box_arc_right(1000, 375); // Right side

    // Penalty Kick
    penalty_centerKick(262.5, 375, 3); // Left side
    penalty_centerKick(1037.5, 375, 3); // Right side

    // Half line
    drawLine(650, 50, 650, 700); // Center of stadium

    // Center circle
    circle(650, 375, 150);

    // Center Kick
    penalty_centerKick(650, 375, 10);

    // Corners
    drawLine(100, 50, 50, 100); // Left bottom side corner
    drawLine(1200, 50, 1250, 100); // Right bottom side corner
    drawLine(100, 700, 50, 650); // Left top side corner
    drawLine(1200, 700, 1250, 650); // Left top side corner
}

void drawPlayers() {
    glLoadIdentity();

    // Team one
    player(600, 475, 1.5); // Leader player

    player(400, 500, 1); // Player 1
    player(400, 300, 1); // Player 2

    player(250, 200, 1); // Player 3
    player(250, 400, 1); // Player 4
    player(250, 600, 1); // Player 5

    // Team two
    player(700, 475, 2.5); // Leader player

    player(900, 500, 2); // Player 1
    player(900, 300, 2); // Player 2

    player(1050, 200, 2); // Player 3
    player(1050, 400, 2); // Player 4
    player(1050, 600, 2); // Player 5

    // Match manager(parent)
    player(650, 720, 3);

    // Energy player as a text for team one
    energyPlayer(600, 475, EnergysT1[0]); // Leader player

    energyPlayer(400, 500, EnergysT1[1]); // Player 1
    energyPlayer(400, 300, EnergysT1[2]); // Player 2

    energyPlayer(250, 200, EnergysT1[3]); // Player 3
    energyPlayer(250, 400, EnergysT1[4]); // Player 4
    energyPlayer(250, 600, EnergysT1[5]); // Player 5

    // Energy player as a text for team two
    energyPlayer(700, 475, EnergysT2[0]); // Leader player

    energyPlayer(900, 500, EnergysT2[1]); // Player 1
    energyPlayer(900, 300, EnergysT2[2]); // Player 2

    energyPlayer(1050, 200, EnergysT2[3]); // Player 3
    energyPlayer(1050, 400, EnergysT2[4]); // Player 4
    energyPlayer(1050, 600, EnergysT2[5]); // Player 5

    // Player number as a text for team one
    playerNo(600, 475, "C"); // Leader player

    playerNo(400, 500, "1"); // Player 1
    playerNo(400, 300, "2"); // Player 2

    playerNo(250, 200, "3"); // Player 3
    playerNo(250, 400, "4"); // Player 4
    playerNo(250, 600, "5"); // Player 5

    // Player number as a text for team two
    playerNo(700, 475, "C"); // Leader player

    playerNo(900, 500, "1"); // Player 1
    playerNo(900, 300, "2"); // Player 2

    playerNo(1050, 200, "3"); // Player 3
    playerNo(1050, 400, "4"); // Player 4
    playerNo(1050, 600, "5"); // Player 5
}

void FromL2_To_P(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = 3000;
    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team2[0][0] - t * (Team2[0][0] - P_X); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team2[0][1] - t * (Team2[0][1] - P_Y);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromL2_To_P, 0);
    }
    else{
        counterTime1 += 2000;
        glutTimerFunc(0, FromP_To_L1, 0);

    }
}

void FromL1_To_P(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = 3000;

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team1[0][0] - t * (Team1[0][0] - P_X); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team1[0][1] - t * (Team1[0][1] - P_Y);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromL1_To_P, 0);
    }
    else{
        counterTime2 += 2000;
        glutTimerFunc(0, FromP_To_L2, 0);

    }
}

void FromL1_To_L2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[0])%10)*1000);
    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[0][0] - t * (Team1[0][0] - Team2[0][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[0][1] - t * (Team1[0][1] - Team2[0][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromL1_To_L2, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[0])%10)*1000);
        EnergysT1[0] -=(rand() % 10);
        energyPlayer(600, 475, EnergysT1[0]); // Leader player

        counterBallInTeam1++;
        matchResult(10, 730, "Team one = %d\nTeam two = %d", counterBallInTeam1, counterBallInTeam2);
//        glutTimerFunc(0, FromL2_To_P, 0);

    }
}

void FromL2_To_L1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[0])%10)*1000);
    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[0][0] - t * (Team2[0][0] - Team1[0][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[0][1] - t * (Team2[0][1] - Team1[0][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromL2_To_L1, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[0])%10)*1000);
        EnergysT2[0] -=(rand() % 10);
        energyPlayer(700, 475, EnergysT2[0]); // Leader player

        counterBallInTeam2++;
        matchResult(10, 730, "Team one = %d\nTeam two = %d", counterBallInTeam1, counterBallInTeam2);
        glutTimerFunc(0, FromL1_To_P, 0);
    }
}

void FromP5_To_L_T2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[5])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[5][0] - t * (Team2[5][0] - Team2[0][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[5][1] - t * (Team2[5][1] - Team2[0][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP5_To_L_T2, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[5])%10)*1000);
        EnergysT2[5] -=(rand() % 10);
        energyPlayer(1050, 600, EnergysT2[5]); // player5
//        glutTimerFunc(0, FromL2_To_L1, 0);
    }
}

void FromP4_To_P5_T2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[4])%10)*1000);


    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[4][0] - t * (Team2[4][0] - Team2[5][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[4][1] - t * (Team2[4][1] - Team2[5][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP4_To_P5_T2, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[4])%10)*1000);
        EnergysT2[4] -=(rand() % 10);
        energyPlayer(1050, 400, EnergysT2[4]); // player4
//        glutTimerFunc(0, FromP5_To_L_T2, 0);
    }
}

void FromP3_To_P4_T2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[3])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[3][0] - t * (Team2[3][0] - Team2[4][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[3][1] - t * (Team2[3][1] - Team2[4][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP3_To_P4_T2, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[3])%10)*1000);
        EnergysT2[3] -=(rand() % 10);
        energyPlayer(1050, 200, EnergysT2[3]); // player3
//        glutTimerFunc(0, FromP4_To_P5_T2, 0);
    }
}

void FromP2_To_P3_T2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[2])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[2][0] - t * (Team2[2][0] - Team2[3][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[2][1] - t * (Team2[2][1] - Team2[3][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP2_To_P3_T2, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[2])%10)*1000);
        EnergysT2[2] -=(rand() % 10);
        energyPlayer(900, 300, EnergysT2[2]); // player2
//        glutTimerFunc(0, FromP3_To_P4_T2, 0);
    }
}

void FromP1_To_P2_T2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[1])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[1][0] - t * (Team2[1][0] - Team2[2][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[1][1] - t * (Team2[1][1] - Team2[2][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP1_To_P2_T2, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[1])%10)*1000);
        EnergysT2[1] -=(rand() % 10);
        energyPlayer(900, 500, EnergysT2[1]); // player1
//        glutTimerFunc(0, FromP2_To_P3_T2, 0);
    }
}

void FromL_To_P1_T2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = (((100 - EnergysT2[0])%10)*1000);
    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = Team2[0][0] - t * (Team2[0][0] - Team2[1][0]); // Move from (600, 370) to (650, 615)
        ballTeam2_y = Team2[0][1] - t * (Team2[0][1] - Team2[1][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromL_To_P1_T2, 0);
    }
    else{
        counterTime2 += (((100 - EnergysT2[0])%10)*1000);
        EnergysT2[0] -=(rand() % 10);
        energyPlayer(700, 475, EnergysT2[0]); // Leader player
        glutTimerFunc(0, FromP1_To_P2_T2, 0);
    }
}

void FromP5_To_L_T1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[5])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[5][0] - t * (Team1[5][0] - Team1[0][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[5][1] - t * (Team1[5][1] - Team1[0][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP5_To_L_T1, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[5])%10)*1000);
        EnergysT1[5] -=(rand() % 10);
        energyPlayer(250, 600, EnergysT1[5]); // Player5
//        glutTimerFunc(0, FromL1_To_L2, 0);
    }
}
void FromP4_To_P5_T1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[4])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[4][0] - t * (Team1[4][0] - Team1[5][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[4][1] - t * (Team1[4][1] - Team1[5][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP4_To_P5_T1, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[4])%10)*1000);
        EnergysT1[4] -=(rand() % 10);
        energyPlayer(2500, 400, EnergysT1[4]); // Player4
//        glutTimerFunc(0, FromP5_To_L_T1, 0);
    }
}
void FromP3_To_P4_T1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[3])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[3][0] - t * (Team1[3][0] - Team1[4][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[3][1] - t * (Team1[3][1] - Team1[4][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP3_To_P4_T1, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[3])%10)*1000);
        EnergysT1[3] -=(rand() % 10);
        energyPlayer(250, 200, EnergysT1[3]); // Player3
//        glutTimerFunc(0, FromP4_To_P5_T1, 0);
    }
}

void FromP2_To_P3_T1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[2])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[2][0] - t * (Team1[2][0] - Team1[3][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[2][1] - t * (Team1[2][1] - Team1[3][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP2_To_P3_T1, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[2])%10)*1000);
        EnergysT1[2] -=(rand() % 10);
        energyPlayer(400, 300, EnergysT1[2]); // Player2
//        glutTimerFunc(0, FromP3_To_P4_T1, 0);
    }
}

void FromP1_To_P2_T1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[1])%10)*1000);

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[1][0] - t * (Team1[1][0] - Team1[2][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[1][1] - t * (Team1[1][1] - Team1[2][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP1_To_P2_T1, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[1])%10)*1000);
        EnergysT1[1] -=(rand() % 10);
        energyPlayer(400, 500, EnergysT1[1]); // Player1
//        glutTimerFunc(0, FromP2_To_P3_T1, 0);
    }
}

void FromL_To_P1_T1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = (((100 - EnergysT1[0])%10)*1000);
    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = Team1[0][0] - t * (Team1[0][0] - Team1[1][0]); // Move from (600, 370) to (650, 615)
        ballTeam1_y = Team1[0][1] - t * (Team1[0][1] - Team1[1][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromL_To_P1_T1, 0);
    }
    else{
        counterTime1 += (((100 - EnergysT1[0])%10)*1000);
        EnergysT1[0] -=(rand() % 10);
        energyPlayer(600, 475, EnergysT1[0]); // Leader player
//        glutTimerFunc(0, FromP1_To_P2_T1, 0);
    }
}


void FromP_To_L1(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime1;
    int duration = 5*1000;
    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam1_x = P_X - t * (P_X - Team1[0][0]); // Move from (650, 615) to (600, 370)
        ballTeam1_y = P_Y - t * (P_Y - Team1[0][1]);


        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP_To_L1, 0);
    } else {
        counterTime1 += 5000;
//        glutTimerFunc(0, FromL_To_P1_T1, 0);

    }
}
void FromP_To_L2(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currentTime - counterTime2;
    int duration = 5*1000;

    if (elapsedTime < duration) {
        // Calculate the position of the ball based on elapsed time
        float t = (float)elapsedTime / duration;
        ballTeam2_x = P_X - t * (P_X - Team2[0][0]); // Move from (650, 615) to (600, 370)
        ballTeam2_y = P_Y - t * (P_Y - Team2[0][1]);

        // Update display
        glutPostRedisplay();

        // Continue the animation
        glutTimerFunc(16, FromP_To_L2, 0);
    } else {
        counterTime2 += 5000;
//        glutTimerFunc(0, FromL_To_P1_T2, 0);

    }
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawStadium();
    drawPlayers();

    //Time match
    timer(timeround);

    //Round number
    rounds(roundNo);


    // Match result as a text
    matchResult(10, 730, "Team one = %d\nTeam two = %d", counterBallInTeam1, counterBallInTeam2);

    // Draw first ball for team 1
    ball(ballTeam1_x, ballTeam1_y, 1);

    // Draw second ball for team 2
    ball(ballTeam2_x, ballTeam2_y, 2);



    glutSwapBuffers();
}


void init() {
    glClearColor(.2, 0.8, 0.5, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1300, 0, 750);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int minutes) {
    if (!timerRunning)
        return; // If the timer is not running, return immediately

    // Calculate remaining time
    int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    int remainingTime = minutes * 60 * 1000 - elapsedTime; // 5 minutes converted to milliseconds

    // Convert remaining time to minutes and seconds
    int remainingMinutes = remainingTime / (60 * 1000);
    int seconds = (remainingTime / 1000) % 60;

    // Display the remaining time
    glColor3b(0, 0, 0);
    glRasterPos2d(1200, 720);
    char timerText[20];
    sprintf(timerText, "%02d:%02d", remainingMinutes, seconds);
    int len = (int)strlen(timerText);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, timerText[i]);
    }
    glutPostRedisplay();

    if (remainingTime <= 0) {
        timerRunning = false; // Stop the timer when time reaches zero
        if (counterBallInTeam1 > counterBallInTeam2) {
            teamWin(630, 370, "Team one is win");
        } else if (counterBallInTeam1 < counterBallInTeam2) {
            teamWin(630, 370, "Team two is win");
        } else {
            teamWin(630, 370, "Draw");
        }
    }
}



void rounds(int roundNo){
// Display the remaining time
    glColor3b(0, 0, 0);
    glRasterPos2d(620, 25);
    char timerText[20];
    sprintf(timerText, "Round : %d", roundNo);
    int len = (int)strlen(timerText);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, timerText[i]);
    }
}
