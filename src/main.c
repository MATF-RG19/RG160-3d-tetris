#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

#define TIMER_ID 0
#define TIMER_INTERVAL 20
int animation_ongoing = 0;
int animation_parameter = 0;

/* boundaries */
#define X_FROM (-5)
#define X_TO (5)
#define Y_FROM (-5)
#define Y_TO (5)
#define Z_FROM (-5)
#define Z_TO (10)

/* lighting */
GLfloat light_position[] = {15, 15, 15, 0};
GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1};
GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1};
GLfloat light_specular[] = {0.9, 0.9, 0.9, 1};
GLfloat shininess = 30;

/* declaration of initializing functions */
static void openGLinit(void);

/* declaration of callback functions */
static void onDisplay(void);
static void onKeyboard(unsigned char key, int x, int y);
static void onReshape(int width, int height);
static void onTimer(int value); 

/* draw grid */
static void drawGrid();

/* declaration of tetrominos */
static void drawOtetromino();

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

        case 'p':
        case 'P':
            if (!animation_ongoing) {
                animation_ongoing = 1;
                glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
            }
            break;

        case 's':
        case 'S':
            animation_ongoing = 0;
            break;
        
        case 'q':
        case 'Q':
            animation_parameter = 0;
            break;
        
        case 'e':
        case 'E':
            animation_parameter++;
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

    /* placing eye */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -10, 17, 0, 0, 0, 0, 1, 0);

    /* lighting */
    glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);


    glPushMatrix();
    drawGrid();
    glPopMatrix();

    drawOtetromino();

    glutSwapBuffers();
}

static void drawGrid(){
    /* dimesnsions: 10x10x15 */
    float x, y, z;
    glColor4f(1, 1, 1, 0.5);
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

}

static void drawOtetromino(){
    GLfloat Omaterial_diffuse[] = {1,0,0,1};
    GLfloat Omaterial_ambient[] = {0.8, 0, 0, 1};
    GLfloat Omaterial_specular[] = {0.9, 0.9, 0.9, 1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Omaterial_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Omaterial_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Omaterial_specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glNormal3f(0, 1, 0);
        glutSolidCube(1.5);
    glPopMatrix();
}

