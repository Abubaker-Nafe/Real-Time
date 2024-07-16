#include "header.h"
#include "shared_memories.h"
#include "constants.h"
#include "functions.h"

const char* pillMedicineNames[10] = {
        "A", "B", "C", "D", "E",
        "F", "G", "H", "I", "J"
};

const char* liquidMedicineNames[10] = {
        "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T"
};

void init();
void display();
void setPosition();
void timer(int minutes);
void drawLine(double from_posX, double from_posY,double to_posX, double to_posY);
void drawText(double x, double y, const char* text, int flag);
void drawText1(double x, double y, const char* text, int flag);
void drawText2(double x, double y, const char* text, int flag);
void displayInfo();
void drawLiquidMedicine();
void drawPillMedicine();
void drawMedicines(double posX, double posY, int flag);
void displayLiquidLine();
void displayPillLine();
void packagedPillMedicine();
void packagedLiquidMedicine();
void drawContainer(int posX, int posY, int index, int flag);
void drawEmployee(float posX, float posY, int flag);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    readfile("arguments.txt"); // read file for initilizing constants
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(3000, 1300); // Hight = 3000px, width = 1000px
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Pharmaceutical medicine inspection & packaging Simulation");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}

void init() {
    glClearColor(0.5, 0.5, 0.5, 0.0); // Gray color (R = G = B = 0.5)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 3000, 0, 1300);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    timer(SimulationTime / 60);
    drawLine(600 ,170, 600,1130);
    drawLine(1470, 0, 1470, 1300);
    drawLine(2070, 170, 2070, 1300);
    drawLine(0, 1130, 3000, 1130);
    drawLine(0, 170, 3000, 170);

    displayInfo();
    displayLiquidLine();
    displayPillLine();
    drawLiquidMedicine();
    drawPillMedicine();
    packagedPillMedicine();
    packagedLiquidMedicine();
    glutSwapBuffers();
}
void drawLine(double from_posX, double from_posY,double to_posX, double to_posY){
    // Draw body of the plane
    glColor3f(1,1,1); // White color
    glBegin(GL_LINE_LOOP);
    glVertex2f(from_posX, from_posY);
    glVertex2f(to_posX, to_posY);

    glEnd();
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
    glRasterPos2d(2870, 1270);
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

void drawText(double x, double y, const char* text, int flag) {
    if (flag == 0) {
        glColor3f(0, 0, 0);
    }
    else if (flag == 1) {
        glColor3f(1, 0, 0);
    }else if (flag == 2) {
        glColor3f(0, 0, 1);
    }
    else if (flag == 3) {
        glColor3f(0.8, 0.9, 0.7);
    }
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}
void drawText2(double x, double y, const char* text, int flag) {
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

void displayInfo(){
    drawText(900, 1100, "Liquid Area", 0);
    drawText(2500, 1100, "Pill Area", 0);

    char text[100];
    sprintf(text, "Number of Liquid Medicines produced : %d", Read_Info_Num_Of_Liquid_Medicine_Produced());
    drawText(10, 1270, text, 0);

    char text1[100];
    sprintf(text1, "Number of Liquid Medicines inspected : %d", Read_Info_Num_Of_Liquid_Medicine_Packaged()
                                                                + Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec());
    drawText(10, 1240, text1, 0);

    char text2[100];
    sprintf(text2, "Number of Liquid Medicines packaged : %d", Read_Info_Num_Of_Liquid_Medicine_Packaged());
    drawText(10, 1210, text2, 0);

    char text3[100];
    sprintf(text3, "Number of Liquid Medicines out of spec : %d", Read_Info_Num_Of_Liquid_Medicine_Out_Of_spec());
    drawText(10, 1180, text3, 0);

    char textt[100];
    sprintf(textt, "Number of Liquid Containers : %d", Read_Info_Num_Of_Liquid_Medicine_Packaged());
    drawText(10, 1150, textt, 0);

    char text4[100];
    sprintf(text4, "Number of Pill Medicines produced : %d", Read_Info_Num_Of_Pill_Medicine_Produced());
    drawText(2080, 1270, text4, 0);

    char text5[100];
    sprintf(text5, "Number of Pill Medicines inspected : %d", Read_Info_Num_Of_Pill_Medicine_Packaged()
                                                              + Read_Info_Num_Of_Pill_Medicine_Out_Of_spec());
    drawText(2080, 1240, text5, 0);

    char text6[100];
    sprintf(text6, "Number of Pill Medicines packaged : %d", Read_Info_Num_Of_Pill_Medicine_Packaged());
    drawText(2080, 1210, text6, 0);

    char text7[100];
    sprintf(text7, "Number of Pill Medicines out of spec : %d", Read_Info_Num_Of_Pill_Medicine_Out_Of_spec());
    drawText(2080, 1180, text7, 0);

    char textt1[100];
    sprintf(textt1, "Number of Pill Containers : %d", Read_Info_Num_Of_containers());
    drawText(2080, 1150, textt1, 0);

    char text8[100];
    sprintf(text8, "Liquid info per line");
    drawText(95, 1100, text8, 1);

    int posY = 1065;
    for(int i = 0; i < NumOfLiquidProductionLines; i++){
        char text9[100];
        sprintf(text9, "Line %d", i);
        drawText(10, posY, text9, 3);

        posY -= 32;
        sprintf(text9, "Num of Medicine Produced : %d", Read_Liquid_Line_MedicineProduced(i));
        drawText(10, posY, text9, 2);

        posY -= 32;
        sprintf(text9, "Num of Medicine inspected : %d", Read_Liquid_Line_MedicinePackaged(i)
                                                         + Read_Liquid_Line_outofspec(i));
        drawText(10, posY, text9, 2);

        posY -= 32;
        sprintf(text9, "Num of Medicine packaged : %d", Read_Liquid_Line_MedicinePackaged(i));
        drawText(10, posY, text9, 2);

        posY -= 32;
        sprintf(text9, "Num of Medicine out of Spec : %d", Read_Liquid_Line_outofspec(i));
        drawText(10, posY, text9, 2);

        posY -= 60;
    }

    char text10[100];
    sprintf(text10, "Pill info per line");
    drawText(1600, 1100, text10, 1);
    posY = 1065;

    for(int i = 0; i < NumOfPillProductionLines; i++){
        char text9[100];
        sprintf(text10, "Line %d", i);
        drawText(1480, posY, text10, 3);

        posY -= 32;
        sprintf(text10, "Num of Medicine Produced : %d", Read_Pill_Line_MedicineProduced(i));
        drawText(1480, posY, text10, 2);

        posY -= 32;
        sprintf(text10, "Num of Medicine inspected : %d", Read_Pill_Line_MedicinePackaged(i)
                                                          +  Read_Pill_Line_outofspec(i));
        drawText(1480, posY, text10, 2);

        posY -= 32;
        sprintf(text10, "Num of Medicine packaged : %d", Read_Pill_Line_MedicinePackaged(i));
        drawText(1480, posY, text10, 2);

        posY -= 32;
        sprintf(text10, "Num of Medicine out of Spec : %d", Read_Pill_Line_outofspec(i));
        drawText(1480, posY, text10, 2);

        posY -= 60;
    }


    drawText1(1580, 1270, "Medicine Color", 0);
    drawText1(1480, 1220, "Produced: Blue", 1);
    drawText1(1480, 1140, "Inspected : Yellow", 2);
    drawText1(1750, 1180, "Out of Spec: Red", 3);
    drawText1(1760, 1240, "Packaged: Green", 4);

    drawText(550,130,"Liquid Bottles", 0);
    drawText(2100,130,"Pill Cartons", 0);
}

void drawText1(double x, double y, const char* text, int flag){
    if (flag == 0) {
        glColor3f(1, 1, 1); // White color
    }
    else if (flag == 1) {
        glColor3f(0, 0, 1); // Blue color
    }else if (flag == 2) {
        glColor3f(1, 1, 0); // Yellow color
    }
    else if (flag == 3) {
        glColor3f(1, 0, 0); // Red color
    }
    else if (flag == 4) {
        glColor3f(0, 1, 0); // Green color
    }
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void drawLiquidMedicine() {
    int posX,posY,counter,counter1;
    for(int i =0; i < NumOfLiquidProductionLines; i++) {
        posX = 670;
        posY = 1005 - (i * 155);
        counter = 0;
        counter1 = 0;
        for(int j = 0; j < MaxMedicineProduce; j++){
            if(counter > 20){
                posY = 960 - (i * 155);
                posX = 670;
            }
            if(Read_Liquid_produced(j) == 1 && Read_Liquid_LineIndex(j) == i && Read_Liquid_beinginspected(j) == 0){
                drawMedicines(posX,posY,0);
                drawText2(posX - 6, posY - 6, liquidMedicineNames[Read_Liquid_type(j)], 0);
                counter++;
                posX+= 40 + (counter1 + 1);
            }
            else if(Read_Liquid_produced(j) == 1 && Read_Liquid_LineIndex(j) == i
                    && Read_Liquid_beinginspected(j) == 1 && Read_Liquid_OutOfSpec(j) == 0 && Read_Liquid_packaged(j) == 0){
                drawMedicines(posX,posY,3);
                drawText2(posX - 6, posY - 6, liquidMedicineNames[Read_Liquid_type(j)], 0);
                counter++;
                posX+= 40 + (counter1 + 1);
            }
            else if(Read_Liquid_produced(j) == 1 && Read_Liquid_LineIndex(j) == i
                    && Read_Liquid_beinginspected(j) == 1 && Read_Liquid_OutOfSpec(j) == 1){
                drawMedicines(posX,posY,2);
                drawText2(posX - 6, posY - 6, liquidMedicineNames[Read_Liquid_type(j)], 0);
                counter++;
                posX+= 40 + (counter1 + 1);
            }
            else if(Read_Liquid_produced(j) == 1 && Read_Liquid_LineIndex(j) == i
                    && Read_Liquid_beinginspected(j) == 1 && Read_Liquid_packaged(j) == 1){
                drawMedicines(posX,posY,1);
                drawText2(posX - 6, posY - 6, liquidMedicineNames[Read_Liquid_type(j)], 0);
                counter++;
                posX+= 40 + (counter1 + 1);
            }
        }
    }
}

void drawPillMedicine(){
    int posX,posY,counter,counter1;
    for(int i =0; i < NumOfPillProductionLines; i++) {
        posX = 2150;
        posY = 1005 - (i * 155);
        counter = 0;
        counter1 = 0;
        for(int j = 0; j < MaxMedicineProduce; j++){
            if(counter >= 20){
                posY = 960 - (i * 155);
                posX = 2150;
            }
            if(Read_Pill_produced(j) == 1 && Read_Pill_LineIndex(j) == i && Read_Pill_beinginspected(j) == 0){
                drawMedicines(posX,posY,0);
                drawText2(posX - 6, posY - 6, pillMedicineNames[Read_Pill_type(j)], 0);
                counter++;
                posX += 40 * (counter1 + 1);
            }
            else if(Read_Pill_produced(j) == 1 && Read_Pill_LineIndex(j) == i
                    && Read_Pill_beinginspected(j) == 1 && Read_Pill_OutOfSpec(j) == 0 && Read_Pill_packaged(j) == 0){
                drawMedicines(posX,posY,3);
                drawText2(posX - 6, posY - 6, pillMedicineNames[Read_Pill_type(j)], 0);
                counter++;
                posX += 40 * (counter1 + 1);
            }
            else if(Read_Pill_produced(j) == 1 && Read_Pill_LineIndex(j) == i
                    && Read_Pill_beinginspected(j) == 1 && Read_Pill_OutOfSpec(j) == 1){
                drawMedicines(posX,posY,2);
                drawText2(posX - 6, posY - 6, pillMedicineNames[Read_Pill_type(j)], 0);
                counter++;
                posX += 40 * (counter1 + 1);
            }
            else if(Read_Pill_produced(j) == 1 && Read_Pill_LineIndex(j) == i
                    && Read_Pill_beinginspected(j) == 1 && Read_Pill_packaged(j) == 1){
                drawMedicines(posX,posY,1);
                drawText2(posX - 6, posY - 6, pillMedicineNames[Read_Pill_type(j)], 0);
                counter++;
                posX += 40 * (counter1 + 1);
            }
        }
    }
}
void drawMedicines(double posX, double posY, int flag){
    if (flag == 0) {
        glColor3f(0, 0, 1); // Blue color, Produced
    }
    else if(flag == 2){
        glColor3f(1, 0, 0); // red color , Out of spec
    }
    else if(flag == 1){
        glColor3f(0, 1, 0); // green color, Packaged
    }
    else if(flag == 3){
        glColor3f(1, 1, 0); // green color, inspection
    }
    glBegin(GL_QUADS);
    glVertex2f(posX - 16.0f, posY - 12.0f); // Bottom left corner
    glVertex2f(posX + 16.0f, posY - 12.0f); // Bottom right corner
    glVertex2f(posX + 16.0f, posY + 12.0f); // Top right corner
    glVertex2f(posX - 16.0f, posY + 12.0f); // Top left corner
    glEnd();
}
void displayLiquidLine(){
    int posX = 650, posY = 1020;

    for(int i = 0; i < NumOfLiquidProductionLines; i++){
        glColor3f(1, 1, 1); // white color
        posY = 1020 - (i * 155);
        glBegin(GL_POLYGON);
        glVertex2f(posX, posY); // Bottom left corner
        glVertex2f(posX, posY - 80); // Bottom right corner
        glVertex2f(posX + 800, posY - 80); // Top right corner
        glVertex2f(posX + 800, posY); // Top left corner
        glEnd();
        char text[20];
        sprintf(text,"L%d",i);
        drawText(610, posY - 40, text, 0);
        for(int j = 0; j < Read_Liquid_Line_NumOfEmployees(i); j++){
            drawEmployee((posX + 20) + (j * 70),1080 - (i * 155), 0);
        }
    }
}

void displayPillLine(){
    int posX = 2130, posY = 1020;

    for(int i = 0; i < NumOfPillProductionLines; i++){
        glColor3f(1, 1, 1); // white color
        posY = 1020 - (i * 155);
        glBegin(GL_POLYGON);
        glVertex2f(posX, posY); // Bottom left corner
        glVertex2f(posX, posY - 80); // Bottom right corner
        glVertex2f(posX + 800, posY - 80); // Top right corner
        glVertex2f(posX + 800, posY); // Top left corner
        glEnd();
        char text[20];
        sprintf(text,"L%d",i);
        drawText(2080, posY - 40, text, 0);
        for(int j = 0; j < Read_Pill_Line_NumOfEmployees(i); j++){
            drawEmployee((posX + 20) + (j * 70),1080 - (i * 155), 1);
        }
    }
}
void drawEmployee(float posX, float posY, int flag){
    float theta;
    glPushMatrix();
    glLoadIdentity();

    if(flag == 0) {
        glColor3f(0.7, 0.5, 1); // Blue color

        //draw head (r = 10)
        glBegin(GL_POLYGON);
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

    }
    else if(flag == 1) {
        glColor3f(0, 1, 0); // Green color

        //draw head (r = 10)
        glBegin(GL_POLYGON);
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

    }
}

void packagedPillMedicine(){
    int posX = 1500;
    int PosY = 110;
    for(int i = 0; i < NumOfDifferentMedicines; i++){
        for(int j =0; j < Read_PillPackages_HowManyPackaged(i); j++){
            drawContainer(posX,PosY,i, 0);
            posX += 60;
        }
    }
}

void packagedLiquidMedicine(){
    int posX = 40;
    int PosY = 110;
    for(int i = 0; i < NumOfLiquidProductionLines; i++){
        for(int j = 0; j < MaxMedicineProduce; j++){
            if(Read_Liquid_LineIndex(j) == i && Read_Liquid_packaged(j) == 1){
                drawContainer(posX,PosY, Read_Liquid_type(j), 1);
                posX += 60;
            }
        }
    }
}

void drawContainer(int posX, int posY, int index, int flag){
    glColor3f(1, 1, 1); // white color
    glBegin(GL_QUADS);
    glVertex2f(posX - 20.0f, posY - 20.0f); // Bottom left corner
    glVertex2f(posX + 20.0f, posY - 20.0f); // Bottom right corner
    glVertex2f(posX + 20.0f, posY + 20.0f); // Top right corner
    glVertex2f(posX - 20.0f, posY + 20.0f); // Top left corner
    glEnd();
    if(flag == 0) {
        drawText(posX - 5, posY - 8, pillMedicineNames[index], 0);
    } else if (flag == 1){
        drawText(posX - 5, posY - 8, liquidMedicineNames[index], 0);
    }
}