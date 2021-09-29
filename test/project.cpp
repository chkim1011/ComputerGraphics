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
    gluQuadricDrawStyle(obj, GLU_LINE);

}
//modeling
void drawupper_body();
void drawlower_body();
void drawhead();
void drawright_leg();
void drawleft_leg();
void drawright_arm();
void drawleft_arm();


//animation



void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-50.0,0.0,0.0);
        glVertex3f(50.0,0.0,0.0);
        glVertex3f(0.0,-50.0,0.0);
        glVertex3f(0.0,50.0,0.0);
    glEnd();

    glColor3f(0.9,0.9,0.9);

    
    
    glPushMatrix(); //total body 
    
    glPushMatrix(); //upper_body
    drawupper_body();
    
    glPushMatrix(); //head
    glTranslatef(0.0,15.0,0.0);
    drawhead();
    glPopMatrix(); //head done
    
    
    glPushMatrix(); //right_arm
    glColor3f(0.9,0.9,0.9);
    glTranslatef(-4.0,8.2,0.0);  
    drawright_arm();
    glPopMatrix(); //right_arm done
    
    
    glPushMatrix(); //left_arm
    glColor3f(0.9,0.9,0.9);
    glTranslatef(4.0,8.2,0.0);   
    drawleft_arm();
    glPopMatrix(); //left_arm done

    glPopMatrix(); //upper_body done
    
    
    glPushMatrix(); //lower_body
    drawlower_body();

    glPushMatrix(); //right_leg
    glColor3f(0.9,0.9,0.9);
    glTranslatef(-2.0,-5.0,0.0);   
    drawright_leg();
    glPopMatrix(); //right_leg done
    
    glPushMatrix(); //left_leg
    glColor3f(0.9,0.9,0.9);
    glTranslatef(2.0,-5.0,0.0);
    drawleft_leg();
    glPopMatrix(); //left_leg done

    glPopMatrix(); //lower_body done

    glPopMatrix(); //total body done


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
    gluLookAt(-1.0,1.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);


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

void drawright_arm() 
{
    glPushMatrix();
    //upper_arm
    glPushMatrix();
    gluSphere(obj,1.4,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.4,1.0,6.0,10,10);
    glPopMatrix();

    //lower_arm
    glTranslatef(0.0,-6.0,0.0);
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.5, 5.5,10,10);
    glPopMatrix();

    glPopMatrix();
}

void drawleft_arm() 
{
    glPushMatrix();
    //upper_arm
    glPushMatrix();
    gluSphere(obj,1.4,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.4,1.0,6.0,10,10);
    glPopMatrix();

    //lower_arm
    glTranslatef(0.0,-6.0,0.0);
    glPushMatrix();
    glRotatef(-90,1.0,0.0,0.0);
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.5, 5.5,10,10);
    glPopMatrix();

    glPopMatrix();
}



void drawright_leg()
{
    glPushMatrix();
    //upper_leg
    glPushMatrix();
    gluSphere(obj,2.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,2.0,1.0,10.0,10,10);
    glPopMatrix();

    //lower_leg
    glTranslatef(0.0,-10.0,0.0);
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.7, 8.0,10,10);
    glPopMatrix();

    //foot

    glPopMatrix();

}

void drawleft_leg()
{
    glPushMatrix();
    //upper_leg
    glPushMatrix();
    gluSphere(obj,2.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,2.0,1.0,10.0,10,10);
    glPopMatrix();

    //lower_leg
    glTranslatef(0.0,-10.0,0.0);
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.7, 8.0,10,10);
    glPopMatrix();

    //foot

    glPopMatrix();

}

void drawupper_body()
{
    
    
    glScalef(1.0,1.0,0.7);
    
    //wrist
    glRotatef(-90.0,1.0,0.0,0.0);
    //gluSphere(obj,3.0,20,20);
    gluCylinder(obj,3.0,4.0,6.0,10,10);
    //chest
    glTranslatef(0.0,0.0,6.0);
    gluCylinder(obj, 4.0, 4.0,3.6 ,10,10);
    glTranslatef(0.0,0.0,3.6);
    gluCylinder(obj,4.0,1.5,1.5,10,10);
    //neck
    glTranslatef(0.0,0.0,1.5);
    gluCylinder(obj,1.5, 1.2,1.8,10,10);


}

void drawlower_body()
{  
    
    //wrist_bot
    glScalef(1.0,1.0,0.7);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj, 3.0,4.0,5.0,10,10);
    
}

void drawhead()
{
    glPushMatrix();
    glRotatef(30,1.0,0.0,0.0);
    glScalef(0.9,0.9,1.1);
    gluSphere(obj,2.5,10,10);
    glPopMatrix();
}

