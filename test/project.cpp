#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


static GLfloat top_wrist , bot_wrist, right_shoulder, 
                left_shoulder, right_arm, left_arm, 
                right_hip, left_hip, right_knee, 
                left_knee, head = 0;
static GLfloat dt;
static GLfloat x,y,z = 0;
static GLUquadricObj* obj;




void init (void)
{
    glClearColor (0.0,0.0,0.0,0.0); //background color
    glShadeModel(GL_FLAT);

    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);

}
//modeling
void drawhuman();
void drawupper_body();
void drawlower_body();
void drawhead();
void drawright_leg();
void drawleft_leg();
void drawright_arm();
void drawleft_arm();


//animation
void run();


void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex3f(-50.0,0.0,0.0);
        glVertex3f(50.0,0.0,0.0);
        glVertex3f(0.0,-50.0,0.0);
        glVertex3f(0.0,50.0,0.0);
        glVertex3f(0.0,0.0,-50.0);
        glVertex3f(0.0,0.0,50.0);

    glEnd();

    glColor3f(0.9,0.9,0.9);

    
    drawhuman();



    glutSwapBuffers(); //start processing buffered OpenGL routines

} 

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'f':
        return;
    }
}

void timer(int value) 
{  
    
    dt += 0.1;
    run();

    //if (joint <=0 || joint >= 180) dt *=-1;
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) glOrtho(-100.0*w/h,100.0*w/h,-100.0,100.0,-100.0,100.0);
    else glOrtho(-100.0,100.0,-100.0*h/w,100.0*h/w,-100.0,100.0);
    //gluPerspective(45,1.0,10.0,20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,1.0,-1.0,0.0,0.0,0.0,0.0,1.0,0.0);


}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB);
    glutInitWindowPosition (600,400);
    glutInitWindowSize (800,800);
    glutCreateWindow ("Just Practice");   

    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    gluDeleteQuadric(obj);
    return 0;
}

void drawright_arm() 
{
    glPushMatrix();
    
    //upper_arm
    glTranslatef(0.5,0.3,0.0);   
    glRotatef(right_shoulder,1.0,0.0,0.0); //shoulder_joint
    glTranslatef(-0.5,-0.3,0.0);     
    glPushMatrix();
    gluSphere(obj,1.4,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.4,1.0,6.0,10,10);
    glPopMatrix();

    //lower_arm
    glTranslatef(0.0,-6.0,0.0);
    glRotatef(right_arm,1.0,0.0,0.0); //arm_joint
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.5, 5.5,10,10);
    glPopMatrix();

    //hand
    glTranslatef(0.0,-5.5,0.0);
    gluSphere(obj,0.5,10,10);

    glPopMatrix();
}

void drawleft_arm() 
{
    glPushMatrix();

    //upper_arm
    glTranslatef(-0.5,0.3,0.0);   
    glRotatef(left_shoulder,1.0,0.0,0.0); //shoulder_joint
    glTranslatef(0.5,-0.3,0.0);   
    glPushMatrix();
    gluSphere(obj,1.4,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.4,1.0,6.0,10,10);
    glPopMatrix();

    //lower_arm
    glTranslatef(0.0,-6.0,0.0);
    glRotatef(left_arm,1.0,0.0,0.0); //arm_joint
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.5, 5.5,10,10);
    glPopMatrix();

    //hand
    glTranslatef(0.0,-5.5,0.0);
    gluSphere(obj,0.5,10,10);

    glPopMatrix();
}

void drawright_leg()
{
    glPushMatrix();
    //upper_leg
    glTranslatef(0.9,0.8,0.0);
    glRotatef(right_hip,1.0,0.0,0.0); //hip_joint
    glTranslatef(-0.9,-0.8,0.0);
    glPushMatrix();
    gluSphere(obj,2.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,2.0,1.0,10.0,10,10);
    glPopMatrix();

    //lower_leg
    glTranslatef(0.0,-10.0,0.0);
    glRotatef(right_knee,1.0,0.0,0.0); //knee_joint
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.7, 8.0,10,10);
    glPopMatrix();

    //foot
    glTranslatef(0.0,-8.0,0.0);
    gluSphere(obj,0.7,5,5);


    glPopMatrix();

}

void drawleft_leg()
{
    glPushMatrix();
    //upper_leg
    glTranslatef(-0.9,0.8,0.0);
    glRotatef(left_hip,1.0,0.0,0.0); //hip_joint
    glTranslatef(0.9,-0.8,0.0);
    glPushMatrix();
    gluSphere(obj,2.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,2.0,1.0,10.0,10,10);
    glPopMatrix();

    //lower_leg
    glTranslatef(0.0,-10.0,0.0);
    glRotatef(left_knee,1.0,0.0,0.0); //knee_joint
    glPushMatrix();
    gluSphere(obj, 1.0,20,20);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj,1.0, 0.7, 8.0,10,10);
    glPopMatrix();

    //foot
    glTranslatef(0.0,-8.0,0.0);
    gluSphere(obj,0.7,5,5);

    glPopMatrix();

}

void drawhead()
{
    glPushMatrix();
    glRotatef(head,0.0,1.0,0.0);
    glRotatef(30,1.0,0.0,0.0);
    glScalef(0.9,0.9,1.1);
    gluSphere(obj,2.5,10,10);
    glPopMatrix();
}

void drawupper_body()
{
    
    glRotatef(top_wrist,0.0,1.0,0.0); //top_wrist rotate
    glPushMatrix();
    
    glScalef(1.0,1.0,0.7);
    
    //wrist
    glPushMatrix();
    glScalef(1.0,0.7,1.0);
    gluSphere(obj,3.0,10,10);
    glPopMatrix();

    glRotatef(-90.0,1.0,0.0,0.0);
    gluCylinder(obj,3.0,4.0,6.0,10,10);
    //chest
    glTranslatef(0.0,0.0,6.0);
    gluCylinder(obj, 4.0, 4.0,3.6 ,10,10);
    glTranslatef(0.0,0.0,3.6);
    gluCylinder(obj,4.0,1.5,1.5,10,10);
    //neck
    glTranslatef(0.0,0.0,1.5);
    gluCylinder(obj,1.5, 1.2,1.8,10,10);
    glPopMatrix();


}

void drawlower_body()
{  
    glRotatef(bot_wrist,0.0,1.0,0.0); //bot_wrist rotate 
    glPushMatrix();
    glScalef(1.0,1.0,0.7);
    glRotatef(90,1.0,0.0,0.0);
    gluCylinder(obj, 3.0,4.0,5.0,10,10);
    glPopMatrix();
    
}

void drawhuman()
{
    glPushMatrix(); //total body 

    
    glTranslatef(0.0,23.7,0.0); //to the ground
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

}






void run() 
{   

    head = -10*cos(dt);
    top_wrist = 10*cos(dt);
    bot_wrist = 10*cos(dt+M_PI);

    right_shoulder = 52.5*cos(dt+M_PI)+7.5;
    left_shoulder = 52.5*cos(dt)+7.5;
    
    right_arm = -15*cos(dt+M_PI)-105;
    left_arm = -15*cos(dt)-105;

    right_hip = -65*cos(dt+M_PI)-5;
    left_hip = -65*cos(dt)-5;

    right_knee = -50*sin(dt+M_PI)+70;
    left_knee = -50*sin(dt)+70;

    std::cout << left_hip << std::endl;
}