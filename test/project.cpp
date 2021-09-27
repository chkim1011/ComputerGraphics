#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void init (void)
{
    glClearColor (0.0,0.0,0.0,0.0); //background color
    glShadeModel(GL_FLAT);
}


GLfloat x = 0.1;
GLboolean y = true;


void display(void)
{
    // clear all pixels 
    glClear (GL_COLOR_BUFFER_BIT);

    glPushMatrix(); 

    
    glRotatef(30*x,1.0,1.0,1.0);
    glutWireTeapot(x);
 
    glPopMatrix();
   
    
    
    glutSwapBuffers(); //start processing buffered OpenGL routines

} 

void keyboard(unsigned char key, int x, int y);

void timer(int value) 
{  
    if (y) x += 0.1;
    if (!y) x-= 0.1;
    if (x >= 5.0 || x <= 0.0) y = !y; 

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-1.0,1.0,-1.0,1.0,1.5,20.0);
    gluPerspective(45.0,1.0,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    gluLookAt(0.0,0.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);


}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB);
    glutInitWindowPosition (600,400);
    glutInitWindowSize (800,800);
    glutCreateWindow ("Just Practice");   
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    
    return 0;
}