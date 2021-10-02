#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static GLfloat dt(0.1);
static float a(200);
static float cam_x(0);
static float cam_y(1);
static float cam_z(-1);
static float theta(45);
static float phi(30);

static GLUquadricObj* obj;
static GLUquadricObj* obj1;

class Human {
public:
    GLfloat top_wrist , bot_wrist, right_shoulder, 
            left_shoulder, right_arm, left_arm, 
            right_hip, left_hip, right_knee, 
            left_knee, head;
    
    Human()
    {
        top_wrist , bot_wrist, right_shoulder, 
        left_shoulder, right_arm, left_arm, 
        right_hip, left_hip, right_knee, 
        left_knee, head = 0;   
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

    void run(GLfloat dt, float v) 
    {   
        dt = v*dt;

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

    }


private:
    void drawright_arm() 
    {   
        glColor3f(0.9765,0.7765,0.8118);

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
        glColor3f(0.9765,0.7765,0.8118);

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
        glColor3f(0.7529,0.7529,0.7529);
        
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
        glColor3f(0.7529,0.7529,0.7529);

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
        glColor3f(0.9765,0.7765,0.8118);
        
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
        glColor3f(0.7529,0.7529,0.7529);

        glRotatef(bot_wrist,0.0,1.0,0.0); //bot_wrist rotate 
        glPushMatrix();
        glScalef(1.0,1.0,0.7);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj, 3.0,4.0,5.0,10,10);
        glPopMatrix();
        
    }

            
};

class Wheel
{
public:
    void drawwheel()
    {
        glPushMatrix();
            glColor3f(0.0,0.0,0.0);
            glTranslatef(0.0,25.0,0.0);
            glRotatef(wheel,1.0,0.0,0.0);
            glRotatef(90.0,0.0,1.0,0.0);
            glTranslatef(0.0,0.0,-5.0);
            glPushMatrix();
                glTranslatef(0.0,0.0,0.0);
                
                gluDisk(obj,22,25,10,20);
            glPopMatrix();
            
            glPushMatrix();
                glTranslatef(0.0,0.0,10.0);
                
                gluDisk(obj,22,25,10,10);
            glPopMatrix();

            glColor3f(0.9,0.9,0.9);
            gluCylinder(obj1,22,22,10,10,10);
            gluCylinder(obj1,25,25,10,10,10);
        glPopMatrix();     
    }

    void roll(GLfloat dt, float v)
    {
        wheel = v * dt;
    }

private:
    GLfloat wheel = 0;
};
    
Human human1, human2;
Wheel wheel1, wheel2;


void init (void)
{
    glClearColor (1.0,1.0,1.0,0.0); //background color
    glShadeModel(GL_SMOOTH);

    obj = gluNewQuadric();
    obj1 = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricDrawStyle(obj1, GLU_LINE);


}



void display(void)
{   
    glClear (GL_COLOR_BUFFER_BIT);      
    
    
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUAD_STRIP);
        glColor3f(0.8235,0.8824,0.9176);
        glVertex3f(-50.0,0.0,50.0);
        glVertex3f(-50.0,50.0,50.0);
        glVertex3f(-50.0,0.0,-50.0);
        glVertex3f(-50.0,50.0,-50.0);
        glVertex3f(50.0,0.0,-50.0);
        glVertex3f(50.0,50.0,-50.0);
    glEnd();
    
   
    
    glPushMatrix();
    glTranslatef(-25.0,0.0,0.0);
    glRotatef(0,0.0,1.0,0.0);
    human1.drawhuman();
    wheel1.drawwheel();
    glPopMatrix();

    
     glPushMatrix();
    glTranslatef(25.0,0.0,0.0);
    glRotatef(0,0.0,1.0,0.0);
    wheel2.drawwheel();
    human2.drawhuman();
    glPopMatrix();
    


    glutSwapBuffers(); //start processing buffered OpenGL routines

} 

void orientMe()
{   
    cam_x = sin(theta*M_PI/180)*cos(phi*M_PI/180);
    cam_y = sin(phi*M_PI/180);
    cam_z = cos(theta*M_PI/180)*cos(phi*M_PI/180);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cam_x,cam_y,cam_z, 0.0,0.0,0.0, -cam_x, 1/cam_y - cam_y, -cam_z);

}
void zoom()
{
    int w(800);
    int h(800);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) glOrtho(-a*w/h,a*w/h,-a,a,-a,a);
    else glOrtho(-a,a,-a*h/w,a*h/w,-a,a);   
}


void keyboard(unsigned char key, int x, int y)
{
    float da = 10;
    switch(key)
    {
        case 'r':
            dt += 0.1;
            human1.run(dt,5);
            wheel2.roll(dt,10);
            break;
        case 'w':
            phi += da;
            if (phi > 89) phi = 88;
            orientMe();
            std::cout << phi << std::endl;
             

            break;

        case 's':
            phi -= da;
            if (phi < 1) phi = 2;
            orientMe();
            std::cout << phi << std::endl;

            break;
        case 'a':
            theta -= da;
            if (theta > 360 || theta < 0) theta = abs(abs(theta)-360);
            orientMe();
            std::cout << theta << std::endl;

            break;

        case 'd':
            theta += da;
            if (theta > 360 || theta < 0) theta = abs(abs(theta)-360);
            orientMe();
            break;

        case 'q':
            a -= da;
            if (a <= 50) a = 50;
            zoom();
            
            std::cout << a << std::endl;

            break;

        case 'e':
            a += da;
            if (a >= 1000) a = 1000;
            zoom();

            std::cout << a << std::endl;

            break;
   
    }
    glutPostRedisplay();
}

void timer(int value) 
{  
    dt += 0.1;
    
    wheel1.roll(dt,100);
    human1.run(dt,5);

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) glOrtho(-a*w/h,a*w/h,-a,a,-a,a);
    else glOrtho(-a,a,-a*h/w,a*h/w,-a,a);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0,1.0,1.0, 0.0,0.0,0.0, 0.0, 1.0, 0.0);

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
    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshape);
    
    glutMainLoop();

    gluDeleteQuadric(obj);
    gluDeleteQuadric(obj1);

    return 0;
}


