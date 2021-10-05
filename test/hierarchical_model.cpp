#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static GLfloat dt(0.1);
static GLfloat ds(0);
static float a(200);
static float cam_x(0);
static float cam_y(1);
static float cam_z(-1);
static float theta(45);
static float phi(30);
static float pole_x, pole_y, pole_z = 0;
static float fly_x, fly_y, fly_z = 0;
static bool tf = true;

static GLUquadricObj* obj;
static GLUquadricObj* obj1;




void drawgrid()
{
    glBegin(GL_LINES);
    glColor3f(0.1,0.1,0.1);


        glVertex3f(-100.0,0.0,100.0);
        glVertex3f(100.0,0.0,100.0);
        glVertex3f(-100.0,0.0,75.0);
        glVertex3f(100.0,0.0,75.0);
        glVertex3f(-100.0,0.0,50.0);
        glVertex3f(100.0,0.0,50.0);
        glVertex3f(-100.0,0.0,25.0);
        glVertex3f(100.0,0.0,25.0);
        glVertex3f(-100.0,0.0,0.0);
        glVertex3f(100.0,0.0,0.0);
        glVertex3f(100.0,0.0,-25.0);
        glVertex3f(-100.0,0.0,-25.0);
        glVertex3f(100.0,0.0,-50.0);
        glVertex3f(-100.0,0.0,-50.0);
        glVertex3f(100.0,0.0,-75.0);
        glVertex3f(-100.0,0.0,-75.0);
        glVertex3f(100.0,0.0,-100.0);
        glVertex3f(-100.0,0.0,-100.0);

        glVertex3f(100.0,0.0,-100.0);
        glVertex3f(100.0,0.0,100.0);
        glVertex3f(75.0,0.0,-100.0);
        glVertex3f(75.0,0.0,100.0);
        glVertex3f(50.0,0.0,-100.0);
        glVertex3f(50.0,0.0,100.0);
        glVertex3f(25.0,0.0,-100.0);
        glVertex3f(25.0,0.0,100.0);
        glVertex3f(0.0,0.0,0.-100);
        glVertex3f(0.0,0.0,100.0);
        glVertex3f(-25.0,0.0,100.0);
        glVertex3f(-25.0,0.0,-100.0);
        glVertex3f(-50.0,0.0,100.0);
        glVertex3f(-50.0,0.0,-100.0);
        glVertex3f(-75.0,0.0,100.0);
        glVertex3f(-75.0,0.0,-100.0);
        glVertex3f(-100.0,0.0,100.0);
        glVertex3f(-100.0,0.0,-100.0);
    glEnd();
}

void drawwall()
{
    glBegin(GL_QUAD_STRIP);
        glColor3f(0.8235,0.8824,0.9176);
        glVertex3f(-100.0,0.0,100.0);
        glVertex3f(-100.0,100.0,100.0);
        glVertex3f(-100.0,0.0,-100.0);
        glVertex3f(-100.0,100.0,-100.0);
        glVertex3f(100.0,0.0,-100.0);
        glVertex3f(100.0,100.0,-100.0);
    glEnd();
}


class Human 
{
public:
    GLfloat top_wrist[3] , bot_wrist[3], right_shoulder[3], 
            left_shoulder[3], right_arm[3], left_arm[3], 
            right_hip[3], left_hip[3], right_knee[3], 
            left_knee[3], head[3], barbell1[3], barbell2[3];
    
    Human()
    {
        top_wrist[3], bot_wrist[3], right_shoulder[3], 
        left_shoulder[3], right_arm[3], left_arm[3], 
        right_hip[3], left_hip[3], right_knee[3], 
        left_knee[3], head[3], barbell1[3], barbell2[3] = 0;   
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

    void drawhuman_ex()
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
        drawright_arm_ex();
        glPopMatrix(); //right_arm done
        
        
        glPushMatrix(); //left_arm
        glColor3f(0.9,0.9,0.9);
        glTranslatef(4.0,8.2,0.0);   
        drawleft_arm_ex();
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

//animation

    void run(GLfloat dt, float v) 
    {   
        dt = v*dt;

        head[1] = -10*cos(dt);
        top_wrist[1] = 10*cos(dt);
        bot_wrist[1] = 10*cos(dt+M_PI);

        right_shoulder[0] = 52.5*cos(dt+M_PI)+7.5;
        left_shoulder[0] = 52.5*cos(dt)+7.5;
        
        right_arm[0] = -15*cos(dt+M_PI)-105;
        left_arm[0] = -15*cos(dt)-105;

        right_hip[0] = -65*cos(dt+M_PI)-5;
        left_hip[0] = -65*cos(dt)-5;

        right_knee[0] = -50*sin(dt+M_PI)+70;
        left_knee[0] = -50*sin(dt)+70;

    }

    void walk(GLfloat dt, float v)
    {
        dt = v*dt;

        head[1] = -5*cos(dt);
        top_wrist[1] = 5*cos(dt);
        bot_wrist[1] = 5*cos(dt+M_PI);

        right_shoulder[0] = 20*cos(dt+M_PI)+2.5;
        left_shoulder[0] = 20*cos(dt)+2.5;
        
        right_arm[0] = -15*cos(dt+M_PI)-15;
        left_arm[0] = -15*cos(dt)-15;

        right_hip[0] = -30*cos(dt+M_PI)-10;
        left_hip[0] = -30*cos(dt)-10;

        right_knee[0] = -20*sin(dt+M_PI)+40;
        left_knee[0] = -20*sin(dt)+40;

    }

    void exercise(GLfloat dt, float v)
    {
        dt = v*dt;
        
        right_shoulder[0] = 45*cos(dt)-45;
        left_shoulder[0] = 45*cos(dt)-45;

        right_arm[0] = -45*cos(dt)-45;
        left_arm[0] = -45*cos(dt)-45;


    }
     
    void pole(GLfloat dt, float v)
    {
        dt = v*dt;
        
        pole_z = -5.5*sin((180+left_shoulder[0]+left_arm[0])*M_PI/180) - 6*sin((-left_shoulder[0])*M_PI/180);
        pole_y = 11.5 - 5.5*cos((180+left_shoulder[0]+left_arm[0])*M_PI/180) + 6*cos((-left_shoulder[0])*M_PI/180);
        
        bot_wrist[0] = 5;

        right_shoulder[0] = -75*cos(dt)-105;
        left_shoulder[0] = -75*cos(dt)-105;

        right_arm[0] = 70*cos(dt)-70;
        left_arm[0] = 70*cos(dt)-70;

        right_hip[0] = 10;
        left_hip[0] = 10;
        right_knee[0] = 60; 
        left_knee[0] = 60;
    }
 
    void fly() 
    {
       
        if (fly_y >= 100 ) {tf = false;}
        if (fly_y <= -1 ) {ds = (int)ds % 10; tf =true;}        

        if (tf) 
        {
            fly_y = (abs(sin(ds))+ ds) * 5; 
            
            right_shoulder[2] = pow(13*sin(ds),2) - 169;
            left_shoulder[2] = 169 - pow(13*sin(ds),2);

            right_arm[2] = abs(60*sin(ds)) - 60;
            left_arm[2] = 60 - abs(60*sin(ds));

            right_hip[2] =  abs(120*sin(ds)) - 120;
            left_hip[2] = 120 - abs(120*sin(ds));

            right_knee[2] = 120 - abs(120*sin(ds));
            left_knee[2] = abs(120*sin(ds)) - 120;
            
        }
        else if (!tf) 
        {   
            fly_y -= 0.1*ds; 

            right_shoulder[2] = pow(13*sin(3*ds),2) - 169;
            left_shoulder[2] = 169 - pow(13*sin(3*ds),2);


            right_arm[2] = abs(60*sin(3*ds)) - 60;
            left_arm[2] = 60 - abs(60*sin(3*ds));

            right_hip[2] =  abs(100*sin(3*ds)) - 100;
            left_hip[2] = 100 - abs(100*sin(3*ds));

            right_knee[2] = 100 - abs(100*sin(3*ds));
            left_knee[2] = abs(100*sin(3*ds)) - 100;

        }

        std::cout << fly_y << std::endl;

        //std::cout << dt << std::endl;
        ds += 0.1;


    }

private:

    void rotate(GLfloat joint[])
    {
        glRotatef(joint[0],1.0,0.0,0.0);
        glRotatef(joint[1],0.0,1.0,0.0);
        glRotatef(joint[2],0.0,0.0,1.0);
    
    }

    void drawright_arm() 
    {   
        glColor3f(0.9765,0.7765,0.8118);

        glPushMatrix();
        
        //upper_arm
        glTranslatef(0.5,0.3,0.0);   
        rotate(right_shoulder); //shoulder_joint
        glTranslatef(-0.5,-0.3,0.0);     
        glPushMatrix();
        gluSphere(obj,1.4,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.4,1.0,6.0,10,10);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(right_arm); //arm_joint
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
        rotate(left_shoulder); //shoulder_joint
        glTranslatef(0.5,-0.3,0.0);   
        glPushMatrix();
        gluSphere(obj,1.4,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.4,1.0,6.0,10,10);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(left_arm); //arm_joint
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
        rotate(right_hip); //hip_joint
        glTranslatef(-0.9,-0.8,0.0);
        glPushMatrix();
        gluSphere(obj,2.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,2.0,1.0,10.0,10,10);
        glPopMatrix();

        //lower_leg
        glTranslatef(0.0,-10.0,0.0);
        rotate(right_knee); //knee_joint
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
        rotate(left_hip); //hip_joint
        glTranslatef(0.9,-0.8,0.0);
        glPushMatrix();
        gluSphere(obj,2.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,2.0,1.0,10.0,10,10);
        glPopMatrix();

        //lower_leg
        glTranslatef(0.0,-10.0,0.0);
        rotate(left_knee); //knee_joint
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
        rotate(head);
        glRotatef(30,1.0,0.0,0.0);
        glScalef(0.9,0.9,1.1);
        gluSphere(obj,2.5,10,10);
        glPopMatrix();
    }

    void drawupper_body()
    {
        glColor3f(0.9765,0.7765,0.8118);
        
        rotate(top_wrist); //top_wrist rotate
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

        rotate(bot_wrist); //bot_wrist rotate 
        glPushMatrix();
        glScalef(1.0,1.0,0.7);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj, 3.0,4.0,5.0,10,10);
        glPopMatrix();
        
    }

    void drawright_arm_ex() 
    {   
        glColor3f(0.9765,0.7765,0.8118);

        glPushMatrix();
        
        //upper_arm
        glTranslatef(0.5,0.3,0.0);   
        rotate(right_shoulder); //shoulder_joint
        glTranslatef(-0.5,-0.3,0.0);     
        glPushMatrix();
        gluSphere(obj,1.4,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.4,1.0,6.0,10,10);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(right_arm); //arm_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.5, 5.5,10,10);
        glPopMatrix();

        //hand
        glTranslatef(0.0,-5.5,0.0);
        gluSphere(obj,0.5,10,10);

        //barbell
        glTranslatef(4.0,0.0,0.0);
        rotate(barbell2);
        glRotatef(-90,0.0,1.0,0.0);

        glColor3f(0.5,0.5,0.5);
        gluCylinder(obj,0.5,0.5,10,20,10);
        glTranslatef(0.0,0.0,10.0);
    
        glColor3f(0.0,0.0,0.0);
        gluDisk(obj,0.5,3,20,10);
        gluCylinder(obj,3,3,0.5,20,10);
        glTranslatef(0.0,0.0,0.5);    
        gluDisk(obj,0.5,3,20,10);



        glPopMatrix();
    }
   
    void drawleft_arm_ex()
    {
        glColor3f(0.9765,0.7765,0.8118);

        glPushMatrix();

        //upper_arm
        glTranslatef(-0.5,0.3,0.0);   
        rotate(left_shoulder); //shoulder_joint
        glTranslatef(0.5,-0.3,0.0);   
        glPushMatrix();
        gluSphere(obj,1.4,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.4,1.0,6.0,10,10);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(left_arm); //arm_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.5, 5.5,10,10);
        glPopMatrix();

        //hand
        glTranslatef(0.0,-5.5,0.0);
        gluSphere(obj,0.5,10,10);

        //barbell
        glTranslatef(-4.0,0.0,0.0);
        rotate(barbell1);
        glRotatef(90,0.0,1.0,0.0);

        glColor3f(0.5,0.5,0.5);
        gluCylinder(obj,0.5,0.5,10,20,10);
        glTranslatef(0.0,0.0,10.0);
    
        glColor3f(0.0,0.0,0.0);
        gluDisk(obj,0.5,3,20,10);
        gluCylinder(obj,3,3,0.5,20,10);
        glTranslatef(0.0,0.0,0.5);    
        gluDisk(obj,0.5,3,20,10);
        
        
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

Human human1, human2, human3, human4, human5;
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
    
    //background
    //drawwall();
    glPushMatrix();
    glTranslatef(0.0,-25.0,0.0);
    drawgrid();

    //walking human    
    glPushMatrix();  
    glTranslatef(-50.0,0.0,-30.0);
    glScalef(2.5,2.5,2.5);
    glRotatef(0,0.0,1.0,0.0);
    wheel2.drawwheel();
    human2.drawhuman();
    glPopMatrix();
    
    
    
    //running human
    glPushMatrix();
    glTranslatef(-50.0,125.0,-30.0);
    glRotatef(180,0.0,1.0,0.0);
    human1.top_wrist[0] = 10;
    human1.bot_wrist[0] = 10;
    human1.drawhuman();
    wheel1.drawwheel();
    glPopMatrix();
       
  
  
    //pole human
    glPushMatrix();
    glTranslatef(50.0,0.0,-50.0);
    glScalef(1.5,1.5,1.5);

    glPushMatrix();
    glTranslatef(pole_x,pole_y,pole_z);
    human4.drawhuman();
    glPopMatrix();

    
    glPushMatrix();
    glColor3f(0.5,0.5,0.5);
    glTranslatef(0.0,43.5,0.0);
    glRotatef(90.0,0.0,1.0,0.0);
    glTranslatef(0.0,0.0,-20.0);
    gluCylinder(obj,0.5,0.5,40,20,10);
    glPopMatrix();
    
    
    glPushMatrix();
    glColor3f(0.0,0.25,0.25);
    glRotatef(-90,1.0,0.0,0.0);
    glTranslatef(20.0,0.0,0.0);
    gluCylinder(obj, 1.0,1.0,50,20,10);
    glTranslatef(-40.0,0.0,0.0);
    gluCylinder(obj, 1.0,1.0,50,20,10);
    glPopMatrix();

    
    glPopMatrix();


    //fly human

    glPushMatrix();
    glTranslatef(fly_x,fly_y,fly_z);  
    glTranslatef(-50.0,0.0,100.0);
    glRotatef(90,0.0,1.0,0.0);
    glScalef(2.0,2.0,2.0);
    human5.drawhuman();
    glPopMatrix();

    //exercise human
    glPushMatrix();
    glTranslatef(50.0,0.0,75.0);
    glScalef(2.0,2.0,2.0);
    
    glPushMatrix();
    glColor3f(0.2,0.2,0.2);
    glTranslatef(0.0,7.0,-25.0);
    glScalef(0.3,1.0,1.0);
    glRotatef(90,1.0,0.0,0.0);
    gluDisk(obj,1,15,20,20);
    gluCylinder(obj,15,15,7,20,20);
    glPopMatrix();

    glPushMatrix();
    human3.left_knee[0] = 90;
    human3.right_knee[0] = 90;
    human3.left_hip[2] = 45;
    human3.right_hip[2] = -45;
    glTranslatef(0.0,9.0,0.0);
    glRotatef(-90,1.0,0.0,0.0);
    human3.drawhuman_ex();
    glPopMatrix();
    
    glPopMatrix();
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
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    GLint w = dims[2];
    GLint h = dims[3];
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) glOrtho(-a*w/h,a*w/h,-a,a,-a,a);
    else glOrtho(-a,a,-a*h/w,a*h/w,-a,a);   
}

void timer(int value) 
{  
    dt += 0.1;
    
    
    human5.fly();
    
    human4.pole(dt,1);

    human3.exercise(dt,1);
 


    wheel2.roll(dt,40);
    human2.walk(dt,2);

    wheel1.roll(dt,100);
    human1.run(dt,5);

    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
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


