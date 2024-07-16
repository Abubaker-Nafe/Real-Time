#include "header.h"
#include "shared_memories.h"
#include "message_queues.h"

void init();
void display();
void setPosition();
void drawPlane(double posX, double posY, int flag);
void drawLine(double from_posX, double from_posY,double to_posX, double to_posY);
void drawText(double x, double y, const char* text);
void drawText1(double x, double y, const char* text);
void drawFamily(double posX, double posY, int familyNo);
void drawPerson(double posX, double posY, int flag);
void drawWorker(double posX, double posY, int flag);
void drawEnergy(double posX, double posY, int energy);
void drawNumOfContainer(double posX, double posY, int content);
void timer(int minutes);
void KillWorkerHandler(int signum);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    setPosition();
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1300, 750); // Hight = 750px, width = 1500px
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Distributing wheat flour aid bags to our people in northern Gaza strip");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}


void setPosition(){
    for(int i = 0 ;i < readNumOfPlanes() ; i++){
        setPlanePOSX(i,50 + (i*100) + (i*20));
        setPlanePOSY(i,50);
    }

    int counter = 0;
    int counter2 = 0;
    int counter3 = 0;

    // Draw families and persons inside tents
    for (int i = 0; i < readNumOfFamilies(); i++) {
        if (counter < 5){
            set_Family_POSX(i,755);
            set_Family_POSY(i,745 - (i * 120));
            counter++;
        }
        else{
            set_Family_POSX(i,1045);
            set_Family_POSY(i,745 - ( counter2 * 120));
            counter2++;
        }
    }

    // Reset Counters
    counter = 0;
    counter2 = 0;
    counter3 = 0;

    for (int i = 0; i < readNumOfDistributers(); i++) {
        if (counter < 15){
            SetDistributerPOSX(i,230 + (i * 30));
            SetDistributerPOSY(i,690);
            counter++;
        }
        else if(counter >= 15 && counter < 30){
            SetDistributerPOSX(i,230 + (counter2 * 30));
            SetDistributerPOSY(i,650);
            counter2++;
            counter++;
        }
        else {
            SetDistributerPOSX(i,230 + (counter3 * 30));
            SetDistributerPOSY(i,610);
            counter3++;
            counter2++;
            counter++;
        }
    }

    // Reset Counters
    counter = 0;
    counter2 = 0;
    counter3 = 0;

    // Draw Splitting Worker
    for (int i = 0; i < readNumOfSplitters(); i++) {
        if (counter < 15){
            SetSplitterPOSX(i,230 + (i * 30));
            SetSplitterPOSY(i,540);
            counter++;
        }
        else if(counter >= 15 && counter < 30){
            SetSplitterPOSX(i,230 + (counter2 * 30));
            SetSplitterPOSY(i,500);
            counter2++;
            counter++;
        }
        else {
            SetSplitterPOSX(i,230 + (counter3 * 30));
            SetSplitterPOSY(i,460);
            counter3++;
            counter2++;
            counter++;
        }
    }

    // Reset Counters
    counter = 0;
    counter2 = 0;
    counter3 = 0;

    // Draw collecter Worker
    for (int i = 0; i < readNumOfCollectors(); i++) {
        if (counter < 15){
            SetCollectorPOSX(i,230 + (i * 30));
            SetCollectorPOSY(i,390);
            counter++;
        }
        else if(counter >= 15 && counter < 30){
            SetCollectorPOSX(i,230 + (counter2 * 30));
            SetCollectorPOSY(i,350);
            counter2++;
            counter++;
        }
        else {
            SetCollectorPOSX(i,230 + (counter3 * 30));
            SetCollectorPOSY(i,310);
            counter3++;
            counter2++;
            counter++;
        }
    }

}

void init() {
    glClearColor(0.5, 0.5, 0.5, 0.0); // Gray color (R = G = B = 0.5)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1300, 0, 750);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    setPosition();

    timer(readSimulationTime()/60);
    drawLine(0, 150, 1300, 150); // Airspace boundaries
    drawLine(200, 150, 200, 750); // The location of the Zionist occupation forces
    drawLine(750, 150, 750, 750);
    drawLine(200, 600, 750, 600);
    drawLine(200, 450, 750, 450);
    drawLine(200, 300, 750, 300);

    for (int i = 0; i < readNumOfPlanes(); i++) {
        if (readPlaneState(i) == 0) {
            drawPlane(readPlanePOSX(i), readPlanePOSY(i), 0);
        } else {
            drawPlane(readPlanePOSX(i), readPlanePOSY(i), 1);
        }
    }

    // Draw families and persons inside tents
    for (int i = 0; i < readNumOfFamilies(); i++) {
        if (read_Family_Hide(i) == 0) {
            drawFamily(read_Family_POSX(i), read_Family_POSY(i), i);
        }
    }

    drawText(230, 730, "Distributing Area");
    char text[40];
    sprintf(text, "We have %d Distributers", ReadNumOfDistributersEdited());
    drawText(550, 730, text);

    // Draw Distributing Worker
    for (int i = 0; i < readNumOfDistributers(); i++) {
        if (readDistributerDead(i) == 0 && readDistributerState(i) == 0) { // if distributor not dead and not busy draw it in blue color
            drawWorker(readDistributerPOSX(i), readDistributerPOSY(i), 0);
            drawEnergy(readDistributerPOSX(i) - 8, readDistributerPOSY(i) + 8, readDistributerEnergy(i));
        }
        else if(readDistributerDead(i) == 0 && readDistributerState(i) == 1){ // if distributor not dead and busy draw it in Light Cyan color
            drawWorker(readDistributerPOSX(i), readDistributerPOSY(i), 1);
            drawEnergy(readDistributerPOSX(i) - 8, readDistributerPOSY(i) + 8, readDistributerEnergy(i));
        }
    }

    drawText(230, 580, "Splitting Area");
    char text1[32];
    sprintf(text1, "We have %d Splitters", ReadNumOfSplittersEdited());
    drawText(550, 580, text1);

    // Draw Splitting Worker
    for (int i = 0; i < readNumOfSplitters(); i++) {
        if (readSplitterHide(i) == 0 && readSplitterState(i) == 0 ) { // if splitter not dead and not busy draw it in red color
            drawWorker(readSplitterPOSX(i), readSplitterPOSY(i), 2);
        }
        else if (readSplitterHide(i) == 0 && readSplitterState(i) == 1 ) { // if splitter not dead and busy draw it in Light Coral color
            drawWorker(readSplitterPOSX(i), readSplitterPOSY(i), 3);
        }
    }

    drawText(230, 430, "Collecting Area");
    char text2[32];
    sprintf(text2, "We have %d Collecters", ReadNumOfCollectersEdited());
    drawText(550, 430, text2);

    // Draw Collecting Worker
    for (int i = 0; i < readNumOfCollectors(); i++) {
        if (readCollectorDead(i) == 0 && readCollectorState(i) == 0) { // if collector not dead and not busy draw it in green color
            drawWorker(readCollectorPOSX(i), readCollectorPOSY(i), 4);
            drawEnergy(readCollectorPOSX(i) - 8, readCollectorPOSY(i) + 8, readCollectorEnergy(i));
        }
        else if (readCollectorDead(i) == 0 && readCollectorState(i) == 1) { // if collector not dead and busy draw it in Light Sea Green color
            drawWorker(readCollectorPOSX(i), readCollectorPOSY(i), 5);
            drawEnergy(readCollectorPOSX(i) - 8, readCollectorPOSY(i) + 8, readCollectorEnergy(i));
        }
    }

    for (int i = 0; i < readNumOfPlanes(); i++) {
        if(readPlaneState(i) == 0){ // for dropping // 1 for refilling
            drawNumOfContainer(readPlanePOSX(i) + 5, readPlanePOSY(i) + 10, readNumOfContainersDummy(i));
        }
    }

    char text3[32];
    sprintf(text3, "Storage: %d", read_Storage());
    drawText1(2, 730, text3);

    char text4[32];
    sprintf(text4, "Shot Collectors: %d", readNumOfCollectorsShot());
    drawText1(2, 700, text4);

    char text5[32];
    sprintf(text5, "Shot Distributers: %d", readNumOfDistributersShot());
    drawText1(2, 670, text5);

    char text6[32];
    sprintf(text6, "Shot Containers: %d", readNumOfContainersShot());
    drawText1(2, 640, text6);

    char text7[32];
    sprintf(text7, "Dead Families: %d", readNumOfFamiliesDead());
    drawText1(2, 610, text7);

    char text9[32];
    sprintf(text9, "Containers Dropped: %d", readNumberOfContainersDropped());
    drawText1(2, 580, text9);


    // Draw Soldier
    char text8[100];
    snprintf(text8, sizeof(text8), "%s", "Ebn Alklb");
    drawText(50,530,text8);
    drawText(50,330,text8);
    drawPerson(80,500,1);
//    drawPerson(80,300,1);


    if(readSoldierShot() == 0){
        drawPerson(80,300,1);
    }else{
        drawPerson(80,300,2);
    }

    signal(SIGUSR1,KillWorkerHandler);

//    // when worker dies
//    if(){
//        drawPerson(80,500,1);
//    }else{
//
//    }

    glutSwapBuffers();

}

// Function to draw plane at a specific position
void drawPlane(double posX, double posY, int flag){
    glEnable(GL_BLEND); // Enable blending to handle transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
    if(flag == 0) {
        glColor3f(1.0f, 1.0f, 1.0f); // White color
    }
    else{
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // Transparent white color
    }
    glBegin(GL_POLYGON);
    glVertex2f(posX, posY);
    glVertex2f(posX+100, posY);
    glVertex2f(posX+100, posY+30);
    glVertex2f(posX, posY+30);
    glEnd();
    glDisable(GL_BLEND); // Disable blending after drawing
}

// Function to draw line at a specific position
void drawLine(double from_posX, double from_posY,double to_posX, double to_posY){
    // Draw body of the plane
    glColor3f(1,1,1); // White color
    glBegin(GL_LINE_LOOP);
    glVertex2f(from_posX, from_posY);
    glVertex2f(to_posX, to_posY);

    glEnd();
}

// Function to draw text at a specific position
void drawText(double x, double y, const char* text) {
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// Function to draw text at a specific position
void drawText1(double x, double y, const char* text) {
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

// Function to draw a family at a specific position with a given number of members
void drawFamily(double posX, double posY, int familyNo) {
    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    // width 250, hight = 100
    glVertex2f(posX, posY);
    glVertex2f(posX + 250, posY);
    glVertex2f(posX + 250, posY - 100);
    glVertex2f(posX, posY - 100);
    glEnd();

    char text[40];
    sprintf(text, "Family %d, Starvation Rate %d", familyNo, read_StarvationRate(familyNo));
    drawText(posX + 10, posY - 30, text);
    if(read_Family_Hide(familyNo) == 0) {
        drawPerson(posX, posY,0);
    }
    if(read_StarvationRate(familyNo) != 100){
        drawPerson(posX, posY, 0);
    }
}

void drawPerson(double posX, double posY, int flag) {
    float theta;
    glPushMatrix();
    glLoadIdentity();

    if(flag == 0) {
        posX += 20;
        posY -= 45;
        for (int i = 0; i < 5; i++) {
            //draw head (r = 10)
            glBegin(GL_POLYGON);
            glColor3f(0, 0, 1); // Blue color
            for (int i = 0; i < 360; i++) {
                theta = i * (3.1416 / 180);
                glVertex2f(posX + 10 * cos(theta), posY + 10 * sin(theta));
            }
            glEnd();

            // draw body (width = 10, hight = 30)
            glBegin(GL_POLYGON);
            glVertex2f(posX - 10, posY - 10);
            glVertex2f(posX + 10, posY - 10);
            glVertex2f(posX + 10, posY - 40);
            glVertex2f(posX - 10, posY - 40);
            glEnd();

            // draw left Arm (Length = 10)
            glBegin(GL_LINE_LOOP);
            glVertex2f(posX - 10, posY - 20);
            glVertex2f(posX - 20, posY - 25);
            glEnd();

            // draw right Arm (Length = 10)
            glBegin(GL_LINE_LOOP);
            glVertex2f(posX + 10, posY - 20);
            glVertex2f(posX + 20, posY - 25);
            glEnd();

            // draw left leg (Length = 15)
            glBegin(GL_LINE_LOOP);
            glVertex2f(posX - 2.5, posY - 40);
            glVertex2f(posX - 2.5, posY - 55);
            glEnd();

            // draw right leg (Length = 15)
            glBegin(GL_LINE_LOOP);
            glVertex2f(posX + 2.5, posY - 40);
            glVertex2f(posX + 2.5, posY - 55);
            glEnd();

            posX += 40;
        }
    }
    else {
        if(flag == 1) {
            glColor3f(0.502, 0.502, 0); // Olive color (earth-tone, like camouflage)
        }
        else{
            glColor3f(0, 0, 0);
        }
        //draw head (r = 25)
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            theta = i * (3.1416 / 180);
            glVertex2f(posX + 25 * cos(theta), posY + 25 * sin(theta));
        }
        glEnd();

        // draw body (width = 50, hight = 60)
        glBegin(GL_POLYGON);
        glVertex2f(posX - 25, posY - 25);
        glVertex2f(posX + 25, posY - 25);
        glVertex2f(posX + 25, posY - 85);
        glVertex2f(posX - 25, posY - 85);
        glEnd();

        // draw left Arm (Length = 25)
        glBegin(GL_LINE_LOOP);
        glVertex2f(posX - 25, posY - 40);
        glVertex2f(posX - 35, posY - 60);
        glEnd();

        // draw right Arm (Length = 25)
        glBegin(GL_LINE_LOOP);
        glVertex2f(posX + 25, posY - 40);
        glVertex2f(posX + 35, posY - 65);
        glEnd();

        // draw left leg (Length = 25)
        glBegin(GL_LINE_LOOP);
        glVertex2f(posX - 12.5, posY - 85);
        glVertex2f(posX - 12.5, posY - 110);
        glEnd();

        // draw right leg (Length = 25)
        glBegin(GL_LINE_LOOP);
        glVertex2f(posX + 12.5, posY - 85);
        glVertex2f(posX + 12.5, posY - 110);
        glEnd();

    }
}

void drawWorker(double posX, double posY, int flag) {
    if (flag == 0) { // Distributing Worker
        glColor3f(0, 0, 1); // Blue color
    }
    else if(flag == 1){
        glColor3f(0.878, 1, 1);
    }
    else if(flag == 2){
        glColor3f(1, 0, 0); // red color
    }
    else if(flag ==3){
        glColor3f(0.941, 0.502, 0.502);
    }
    else if(flag == 4){
        glColor3f(0, 1, 0); // green color
    }
    else if(flag == 5){
//        glColor3f(0.125, 0.698, 0.667);
        glColor3f(0.5f, 1.0f, 0.5f); // Light green color
    }

    glBegin(GL_QUADS);
    glVertex2f(posX - 8.0f, posY - 8.0f); // Bottom left corner
    glVertex2f(posX + 8.0f, posY - 8.0f); // Bottom right corner
    glVertex2f(posX + 8.0f, posY + 8.0f); // Top right corner
    glVertex2f(posX - 8.0f, posY + 8.0f); // Top left corner
    glEnd();
}



void drawEnergy(double posX, double posY, int energy) {
    glColor3f(1, 1, 1); // White color

    char text[3];
    sprintf(text, "%d", energy);
    glRasterPos2f(posX, posY);
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
    }

}

void drawNumOfContainer(double posX, double posY, int content) {
    char text[20];
    if(content < 0){
        content=0;
    }
    sprintf(text, "%d Container", content);
    drawText(posX, posY, text);

}

void timer(int minutes) {
    // Calculate remaining time
    int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    int remainingTime = minutes * 60 * 1000 - elapsedTime; // 5 minutes converted to milliseconds

    // Convert remaining time to minutes and seconds
    int remainingMinutes = remainingTime / (60 * 1000);
    int seconds = (remainingTime / 1000) % 60;

    // Display the remaining time
    glColor3b(0, 0, 0);
    glRasterPos2d(1240, 730);
    char timerText[20];
    sprintf(timerText, "%02d:%02d", remainingMinutes, seconds);
    int len = (int)strlen(timerText);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, timerText[i]);
    }
    if(remainingTime < 0){
        exit(0);
    }
    glutPostRedisplay();
}


void KillWorkerHandler(int signum) {
    drawPerson(80,500,2);
    glutPostRedisplay();
    sleep(3);
}