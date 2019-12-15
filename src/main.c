#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

/* declaration of initializing functions */
static void openGLinit(void);

/* declaration of callback functions */
static void on_display(void);

/* declaration of tetrominos */
// static void drawLtetromino(void);

int main(int argc, char **argv){

    /* initializing glut */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    /* initializing window */
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(660, 240);
    glutCreateWindow("3D Tetris");

    /* initializing callback functions */
    glutDisplayFunc(on_display);

    /* initializing openGL */
    openGLinit();

    /* main loop */
    glutMainLoop();

    return 0;
}

static void openGLinit(void){
    glClearColor(1, 0, 1, 0);
}

static void on_display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0, 1, 1);
    glBegin(GL_POLYGON);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(0.6, 0.6, 1);
        glVertex3f(-0.5, 0.5, 0.0);
    glEnd();

    glutSwapBuffers();
}


