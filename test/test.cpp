#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

void display(void)
{
    // clear all pixels 
    glClear (GL_COLOR_BUFFER_BIT);
    
    //draw white polygon
    glColor3f (1.0,1.0,1.0);
    glBegin(GL_POLYGON);
        glVertex3f (0.25,0.25,0.0);
        glVertex3f (0.75,0.25,0.0);
        glVertex3f (0.25,0.75,0.0);
        glVertex3f (0.75,0.75,0.0);
    glEnd();

    glFlush(); //start processing buffered OpenGL routines

}

void init (void)
{
    glClearColor (0.0,0.0,0.0,0.0); //background color

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE |GLUT_RGB);
    glutInitWindowSize (250,250);
    glutInitWindowPosition (100,100);
    glutCreateWindow ("hello");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}