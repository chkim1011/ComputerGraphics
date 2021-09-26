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

static int year = 0;
static double a = 0;


void display(void)
{
    // clear all pixels 
    glClear (GL_COLOR_BUFFER_BIT);
    
    //draw white polygon
    
    //glLoadIdentity();
    glPushMatrix();
        glColor3f (0.7,0.0,0.0);
        glutWireSphere(1.0, 100, 100);
        
        glColor3f (0.0,0.0,0.7);
        glRotatef((GLfloat) year,0.0,1.0,0.0);
        glTranslatef(2.0,(GLfloat) cos(a),(GLfloat) cos(a));
        //glScalef(1.0,2.0,1.0);
        glutWireSphere(0.5, 50, 50);


    glPopMatrix();
    glutSwapBuffers(); //start processing buffered OpenGL routines

} 

void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
{
    case 'd':
        year = (year + 10) % 360;
        a = a + 1;
        std::cout << year << std::endl;

        glutPostRedisplay();
        break;
    case 'f':
        a = a + 1;
        std::cout << a << std::endl;
        glutPostRedisplay();
        break;        
}
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
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}