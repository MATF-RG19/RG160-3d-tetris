#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <math.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20
static int animation_ongoing = 0;
static int animation_parameter = 0;

/* declaration of initializing functions */
static void openGLinit(void);

/* declaration of callback functions */
static void onDisplay(void);
static void onKeyboard(unsigned char key, int x, int y);
static void onReshape(int width, int height);
static void onTimer(int value); 

/* boundaries */
#define X_FROM (-5)
#define X_TO (5)
#define Y_FROM (-5)
#define Y_TO (5)
#define Z_FROM (-3)
#define Z_TO (10)

/* camera rotations */
float rotateScene = 0;
static float angle = 0.03;
//TODO
float zoomScene = 17;
static float zoomZ = 0.1;

/* lighting */
GLfloat lightPosition[] = {15, 15, 15, 0};
GLfloat lightDiffuse[] = {0.7, 0.7, 0.7, 1};
GLfloat lightAmbient[] = {0.5, 0.5, 0.5, 1};
GLfloat lightSpecuar[] = {0.9, 0.9, 0.9, 1};
GLfloat shininess = 30;

/* random choosing tetromino */
#define MAX_ARRAY_SIZE 256
int randCounter = 0;
int randArray[MAX_ARRAY_SIZE];
void chooseTetromino();

/* draw grid */
static void drawGrid();

/* declaration of tetrominos */
static void drawOtetromino();
static void drawTtetromino();
static void drawItetromino();
static void drawLtetromino();
static void drawJtetromino();
static void drawStetromino();
static void drawZtetromino();

/* side functions */
static void moveTetrominoToSpawn();

int main(int argc, char **argv){

    /* initializing glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    /* initializing window */
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(660, 240);
    glutCreateWindow("T3Dris");

    /* initializing callback functions */
    glutReshapeFunc(onReshape);
    glutDisplayFunc(onDisplay);
    glutKeyboardFunc(onKeyboard);

    /* initializing openGL */
    openGLinit();

    /* main loop */
    glutMainLoop();

    return 0;
}

static void openGLinit(void){

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    /* lighting */
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecuar);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

    /* random array for picking tetromino TODO: MOVE */
    srand(time(NULL));
    for(randCounter = 0; randCounter < MAX_ARRAY_SIZE; randCounter++){
        randArray[randCounter]=rand()/(RAND_MAX/7);
    }
    randCounter = 0;
}

static void onReshape(int width, int height){

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 1, 30);
}

static void onKeyboard(unsigned char key, int x, int y){

    (void) x;
    (void) y;

    switch(key){
        /* exiting program on escape button */
        case 27:
            exit(EXIT_SUCCESS);
            break;
        
        /* start program on enter button */
        case 13:
            if (!animation_ongoing) {
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
            }
            break;
        /* pause program on space */
        case 32:
            animation_ongoing = 0;
            break;
        
        /* case 'u':
        case 'U':
            animation_parameter = 0;
            break;
        case 'i':
        case 'I':
            animation_parameter++;
            break;  */

        /* rotating scene */
        case 'q':
        case 'Q':
            rotateScene += angle;
            glutPostRedisplay();
            break;
        case 'e':
        case 'E':
            rotateScene -= angle;
            glutPostRedisplay();
            break;

        /* zoom in */
        case 'z':
        case 'Z':
            if(zoomScene >= 5){
                zoomScene -= zoomZ;
                glutPostRedisplay();
            }
                break;
        /* zoom out */
        case 'c':
        case 'C':
            if(zoomScene <= 17){
                zoomScene += zoomZ;
                glutPostRedisplay();
            }
                break;       

    }
}

static void onTimer(int value){

    if (value != TIMER_ID)
        return;

    glutPostRedisplay();

    if (animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
}

static void onDisplay(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* placing eye, rotating eye */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0 + sin(rotateScene)*15, 0 - cos(rotateScene)*15, zoomScene, 0, 0, 0, 0, 1, 0);

    /* birdeye
    gluLookAt(0, 0, 17, 0, 0, 0, 0, -1, 0); */
    
    glPushMatrix();
    drawGrid();
    glPopMatrix();

    glPushMatrix();
    chooseTetromino();
    glPopMatrix();
        
    glutSwapBuffers();
}

/* drawing grid (dimensions : 10x10x13) */
static void drawGrid(){
    float x, y, z;
    glPushMatrix();
    glColor4f(1, 1, 1, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    z = Z_FROM;
    y = Y_FROM;
        
    for(y = Y_FROM; y<Y_TO; y++){
        for(x = X_FROM; x<X_TO; x++){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, y, z);
            glVertex3f(x+1, y, z);
            glVertex3f(x+1, y+1, z);
            glVertex3f(x, y+1, z);
            glEnd(); 
        }
    }    

    for(z = Z_FROM; z<Z_TO; z++){

    /* front side */
        y = Y_FROM;
        for(x = X_FROM; x<X_TO; x++){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, y, z);
            glVertex3f(x+1, y, z);
            glVertex3f(x+1, y, z+1);
            glVertex3f(x, y, z+1);
            glEnd();        
        }
    /* back side */
        y = Y_TO;
        for(x = X_FROM; x<X_TO; x++){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, y, z);
            glVertex3f(x+1, y, z);
            glVertex3f(x+1, y, z+1);
            glVertex3f(x, y, z+1);
            glEnd();        
        }
    /* right side */
        x = X_TO;
        for(y = Y_FROM; y<Y_TO; y++){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, y, z);
            glVertex3f(x, y+1, z);
            glVertex3f(x, y+1, z+1);
            glVertex3f(x, y, z+1);
            glEnd();        
        }
    /* left side */
        x = X_FROM;
        for(y = Y_FROM; y<Y_TO; y++){
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, y, z);
            glVertex3f(x, y+1, z);
            glVertex3f(x, y+1, z+1);
            glVertex3f(x, y, z+1);
            glEnd();        
        }   
    }
    glPopMatrix();
}

/* drawing tetrominos with colorful material */
static void drawOtetromino(){
    GLfloat materialDiffuseO[] = {0.9, 0, 0, 1};
    GLfloat materialAmbientO[] = {0.8, 0, 0, 1};
    GLfloat materialSpecularO[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseO);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientO);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularO);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glutSolidCube(2);
    glPopMatrix();
}

static void drawTtetromino(){
    GLfloat materialDiffuseT[] = {0, 0.9, 0, 1};
    GLfloat materialAmbientT[] = {0, 0.8, 0, 1};
    GLfloat materialSpecularT[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseT);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientT);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularT);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glTranslatef(0.5, -0.5, -0.5); //placing tetromino in center of integer coordinate 
        glutSolidCube(1);
        glTranslatef(0, 1, 0);
        glScalef(3, 1, 1);
        glutSolidCube(1);
    glPopMatrix();
}

static void drawItetromino(){
    GLfloat materialDiffuseI[] = {0, 0, 1, 1};
    GLfloat materialAmbientI[] = {0, 0, 0.9, 1};
    GLfloat materialSpecularI[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseI);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientI);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularI);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glTranslatef(0, 0.5, -0.5); //placing tetromino in center of integer coordinates 
        glScalef(4, 1, 1);
        glutSolidCube(1);
    glPopMatrix();
}

static void drawLtetromino(){
    GLfloat materialDiffuseL[] = {0.9, 0, 0.9, 1};
    GLfloat materialAmbientL[] = {0.8, 0, 0.8, 1};
    GLfloat materialSpecularL[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularL);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glTranslatef(-0.5, -0.5, -0.5); //placing tetromino in center of integer coordinates 
        glutSolidCube(1);
        glTranslatef(1, 1, 0);
        glScalef(3, 1, 1);
        glutSolidCube(1);
    glPopMatrix();
}

static void drawJtetromino(){
    GLfloat materialDiffuseJ[] = {0.5, 1, 0.2, 1};
    GLfloat materialAmbientJ[] = {0.4, 0.8, 0.1, 1};
    GLfloat materialSpecularJ[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseJ);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientJ);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularJ);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glTranslatef(-0.5, 0.5, -0.5); //placing tetromino in center of integer coordinates 
        glutSolidCube(1);
        glTranslatef(1, -1, 0);
        glScalef(3, 1, 1);
        glutSolidCube(1);
    glPopMatrix();
}

static void drawStetromino(){
    GLfloat materialDiffuseS[] = {0, 0.9, 0.9, 1};
    GLfloat materialAmbientS[] = {0, 0.8, 0.8, 1};
    GLfloat materialSpecularS[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularS);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glTranslatef(0, -0.5, -0.5); //placing tetromino in center of integer coordinates 
        glPushMatrix();
        glScalef(2, 1, 1);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(1, 1, 0);
        glScalef(2, 1, 1);
        glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

static void drawZtetromino(){
    GLfloat materialDiffuseZ[] = {0.9, 0.9, 0, 1};
    GLfloat materialAmbientZ[] = {0.8, 0.8, 0, 1};
    GLfloat materialSpecularZ[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuseZ);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbientZ);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularZ);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glTranslatef(0, -0.5, -0.5); //placing tetromino in center of integer coordinates 
        glPushMatrix();
        glScalef(2, 1, 1);
        glutSolidCube(1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-1, 1, 0);
        glScalef(2, 1, 1);
        glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

void chooseTetromino(){
    glPushMatrix();
        moveTetrominoToSpawn();
        switch(randArray[randCounter]){
             case 0:
                 drawOtetromino();
                 break;
             case 1:
                 drawTtetromino();
                 break;
             case 2:
                 drawItetromino();
                 break;
             case 3:
                 drawLtetromino();
                 break;
             case 4:
                 drawJtetromino();
                 break;
             case 5:
                 drawStetromino();
                 break;
             case 6:
                 drawZtetromino();
                 break;
             }
         randCounter++;
         if(randCounter == MAX_ARRAY_SIZE-1){
             randCounter = 0;
        }
    glPopMatrix();
}

static void moveTetrominoToSpawn(){
    glTranslatef(0, 0, Z_TO+1);
}
    
