#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

GLUquadricObj* obj;

void init (void)
{
    glClearColor (0.0,0.0,0.0,0.0); //background color
    glShadeModel(GL_FLAT);

    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);

}
//modeling
void drawbody();
void drawhead();
void drawleg();
void drawarm();

//animation



void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

/**
    glPushMatrix();




    glTranslatef(); //above ground
    drawbody();

    //neck & head
    glPushMatrix();
    glTranslatef(somewhere);
    drawleg();
    glPopMatrix();

    //right leg    
    glPushMatrix();
    glTranslatef(somewhere);
    drawleg();
    glPopMatrix();

    
    //left leg
    glPushMatrix();
    glTranslatef(somewhere);
    drawleg();
    glPopMatrix();
    
    
    //right arm
    glPushMatrix();
    glTranslatef(somewhere);
    drawarm();
    glPopMatrix();

    //left arm
    glPushMatrix();
    glTranslatef(somewhere);
    drawarm();
    glPopMatrix();

    glPopMatrix();
*/
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-50.0,0.0,0.0);
        glVertex3f(50.0,0.0,0.0);
        glVertex3f(0.0,-50.0,0.0);
        glVertex3f(0.0,50.0,0.0);

    glEnd();

    glColor3f(0.9,0.9,0.9);
    drawbody();
    
    glPushMatrix();
    glTranslatef(-5.8,7.8,0.0);
    drawarm();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.8,7.8,0.0);
    drawarm();
    glPopMatrix();

    //right_leg
    glPushMatrix();
    glTranslatef(-2.0,-5.0,0.0);
    drawleg();
    glPopMatrix();

    //left_leg
    glPushMatrix();
    glTranslatef(2.0,-5.0,0.0);
    drawleg();
    glPopMatrix();

    glutSwapBuffers(); //start processing buffered OpenGL routines

} 

void keyboard(unsigned char key, int x, int y)
{
    return;
}

void timer(int value) 
{  
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0,50.0,-50.0,50.0,-50.0,50.0);
    //gluPerspective(45,1.0,10.0,20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);


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
    //glutTimerFunc(0, timer, 0);
    glutMainLoop();

    gluDeleteQuadric(obj);
    return 0;
}

void drawarm() 
{
    glPushMatrix();
    //upper_arm
    //gluRotatef(shoulder_joint)
    glPushMatrix();
    gluSphere(obj,1.8,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.6,1.0,6.0,10,10);
    glPopMatrix();

    //lower_arm
    glTranslatef(0.0,-6.0,0.0);
    //gluRotatef(arm_joint)
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.5, 5.5,10,10);
    glPopMatrix();

    glPopMatrix();
}

void drawleg()
{
    glPushMatrix();
    //upper_leg
    //gluRotatef(hip_joint)
    glPushMatrix();
    gluSphere(obj,2.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,2.0,1.0,10.0,10,10);
    glPopMatrix();

    //lower_leg
    glTranslatef(0.0,-10.0,0.0);
    //gluRotatef(knee_joint)
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.7, 8.0,10,10);
    glPopMatrix();

    //foot

    glPopMatrix();

}

void drawbody()
{


    glPushMatrix();
    glRotatef(-90.0,1.0,0.0,0.0);
    gluCylinder(obj,3.0,4.0,6.0,10,10);
    gluSphere(obj,3.0,20,20);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj, 3.0,4.0,5.0,10,10);
    glPopMatrix();

}

