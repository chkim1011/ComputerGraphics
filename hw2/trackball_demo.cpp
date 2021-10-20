
#include <iostream>
#include <cmath>
#include <GL/glut.h>


static unsigned int width = 1000;
static unsigned int height = 1000;

static bool mouseRotatePressed = false;
static bool mouseMovePressed   = false;
static bool mouseZoomPressed   = false;
static bool mouseDollyPressed   = false;
static bool mouseSeekPressed   = false;
static bool fullScreen = false;

static float lookat_x = 0.0,lookat_y = 0.0, lookat_z = 0.0;
static float lastlookat_x = 0.0, lastlookat_y = 0.0, lastlookat_z = 0.0;
static float lastx =0.0, lasty = 0.0;
static float cam_x = 0.0,cam_y= 0.0, cam_z = 1.0;
static float vx = 0.0, vy = 1.0, vz = 0.0;
static float lastvx =0.0, lastvy = 0.0, lastvz = 0.0;
static float fov = 45.0, lastfov = 45.0;
static float cam_dist = 6.0, lastcam_dist = 6.0;


struct Quaternion {
    
    float w;
    float v[3];

};
float innerproduct(const float a[],const float b[])
{
    return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2]);
}
void normalize(float *a)
{
    float len =  sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    a[0] /= len; a[1] /= len; a[2] /= len;
    return; 
}
void crossproduct(const float a[],const float b[], float c[])
{
    c[0] = a[1]*b[2] - a[2]*b[1];
    c[1] = -a[0]*b[2] + a[2]*b[0];
    c[2] = a[0]*b[1] - a[1]*b[0];
    return;
}
Quaternion qproduct(const Quaternion &p, const Quaternion &q)
{
    Quaternion res;
    float c[3];
    res.w = p.w*q.w - innerproduct(p.v,q.v);
    crossproduct(p.v,q.v,c);
    res.v[0] = p.w*q.v[0] + q.w*p.v[0] + c[0];
    res.v[1] = p.w*q.v[1] + q.w*p.v[1] + c[1];
    res.v[2] = p.w*q.v[2] + q.w*p.v[2] + c[2];

    return res;
}
Quaternion qinverse(Quaternion &p)
{
    p.v[0] *= -1;
    p.v[1] *= -1;
    p.v[2] *= -1;

    return p;
}
void normalize(Quaternion &p)
{
    float len = sqrt(p.w*p.w + p.v[0]*p.v[0] + p.v[1]*p.v[1] + p.v[2]*p.v[2]);
    p.w /= len; p.v[0] /= len; p.v[1] /= len; p.v[2] /= len;   
    return;
}
Quaternion qrotate(Quaternion &orient, Quaternion &rotate)
{
    Quaternion res;
    res.w = 0;
    float cross1[3];
    float cross2[3];

    crossproduct(rotate.v,orient.v,cross1);
    crossproduct(rotate.v,cross1,cross2);

    res.v[0] = rotate.w*rotate.w*orient.v[0] + innerproduct(orient.v,rotate.v)*rotate.v[0] + 2*rotate.w*cross1[0] + cross2[0];
    res.v[1] = rotate.w*rotate.w*orient.v[1] + innerproduct(orient.v,rotate.v)*rotate.v[1] + 2*rotate.w*cross1[1] + cross2[1];
    res.v[2] = rotate.w*rotate.w*orient.v[2] + innerproduct(orient.v,rotate.v)*rotate.v[2] + 2*rotate.w*cross1[2] + cross2[2];
    normalize(res);

    return res;
}

static Quaternion lastorient, currorient, rotate;
static float lastpoint[3] = {0.0,0.0,0.0}; 
static float currpoint[3]={0.0,0.0,0.0}; 
static float angle, c_norm;
static GLUquadricObj* obj;

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
        gluCylinder(obj,1.4,1.0,6.0,30,30);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(right_arm); //arm_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.5, 5.5,30,30);
        glPopMatrix();

        //hand
        glTranslatef(0.0,-5.5,0.0);
        gluSphere(obj,0.5,30,30);

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
        gluCylinder(obj,1.4,1.0,6.0,30,30);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(left_arm); //arm_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.5, 5.5,30,30);
        glPopMatrix();

        //hand
        glTranslatef(0.0,-5.5,0.0);
        gluSphere(obj,0.5,30,30);

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
        gluCylinder(obj,2.0,1.0,10.0,30,30);
        glPopMatrix();

        //lower_leg
        glTranslatef(0.0,-10.0,0.0);
        rotate(right_knee); //knee_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.7, 8.0,30,30);
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
        gluCylinder(obj,2.0,1.0,10.0,30,30);
        glPopMatrix();

        //lower_leg
        glTranslatef(0.0,-10.0,0.0);
        rotate(left_knee); //knee_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.7, 8.0,30,30);
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
        gluSphere(obj,2.5,30,30);
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
        gluCylinder(obj,3.0,4.0,6.0,30,30);
        //chest
        glTranslatef(0.0,0.0,6.0);
        gluCylinder(obj, 4.0, 4.0,3.6 ,30,30);
        glTranslatef(0.0,0.0,3.6);
        gluCylinder(obj,4.0,1.5,1.5,30,30);
        //neck
        glTranslatef(0.0,0.0,1.5);
        gluCylinder(obj,1.5, 1.2,1.8,30,30);
        glPopMatrix();


    }

    void drawlower_body()
    {  
        glColor3f(0.7529,0.7529,0.7529);

        rotate(bot_wrist); //bot_wrist rotate 
        glPushMatrix();
        glScalef(1.0,1.0,0.7);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj, 3.0,4.0,5.0,30,30);
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
        gluCylinder(obj,1.4,1.0,6.0,30,30);
        glPopMatrix();

        //lower_arm
        glTranslatef(0.0,-6.0,0.0);
        rotate(right_arm); //arm_joint
        glPushMatrix();
        gluSphere(obj, 1.0,20,20);
        glRotatef(90,1.0,0.0,0.0);
        gluCylinder(obj,1.0, 0.5, 5.5,30,30);
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


void drawSomething()
{
// Draw something Here
    glBegin(GL_POINT);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glEnd();
    glColor3f(1,1,1);
	glutSolidCone(1.0, 1.2, 20, 1 );
    
}


void reshape(int w, int h)
{
    width = glutGet(GLUT_WINDOW_WIDTH);
    height = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, w, h );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	float aspectRatio = (float)w/(float)h;
	gluPerspective( fov /*field of view angle*/,
					aspectRatio,
					0.1 /*near clipping plane*/,
					1000.0 /* far clipping plane */ );
   
	glMatrixMode( GL_MODELVIEW );
	float x0 = cam_dist*cam_x;
	float y0 = cam_dist*cam_y;
	float z0 = cam_dist*cam_z;    
    glLoadIdentity(); 
	gluLookAt( x0,y0,z0, 
               lookat_x,lookat_y,lookat_z, 
                vx,vy,vz );


}

static Human human1;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glPushMatrix();
    glTranslatef(0.0,-2.0,0.0);
    glScalef(0.1,0.1,0.1);
    human1.drawhuman();
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void keyboardCB(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow(width,height);
            fullScreen = false;
        } else {
            glutFullScreen();
            fullScreen = true;
        }
        break;
    case 'q':
        exit(0);
        break;
    case 'r':

        cam_dist = 6;
        cam_x= 0; cam_y = 0; cam_z = 1;
        lookat_x = 0; lookat_y = 0;
        fov = 45;
        vx = 0; vy = 1; vz = 0;
        reshape(width, height);
        break;
    case 'p':
        //peak
        //peaking coordinate -> look at point
        break;
    }
    glutPostRedisplay();
}

void mouseCB(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseRotatePressed = false;
        mouseMovePressed   = false;
        mouseZoomPressed   = false;
        mouseDollyPressed  = false;
        mouseSeekPressed   = false;

    } else {
        if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_SHIFT==glutGetModifiers())
        {
			// do something here
			//std::cout << "translate click" << std::endl;
            lastx = x;
            lasty = y;
            lastlookat_x = lookat_x;
            lastlookat_y = lookat_y;


            mouseRotatePressed = false;
            mouseMovePressed   = true;
            mouseZoomPressed   = false;
            mouseDollyPressed  = false;
            mouseSeekPressed   = false;
                   
        }
        else if (button==GLUT_LEFT_BUTTON && GLUT_ACTIVE_CTRL==glutGetModifiers())
        {
			// do something here
			//std::cout << "zoom click" << std::endl;
            lastx = x;
            lasty = y;
            lastfov = fov;

            mouseRotatePressed = false;
            mouseMovePressed   = false;
            mouseZoomPressed   = true;
            mouseDollyPressed  = false;
            mouseSeekPressed   = false;
        
        }
        else if (button==GLUT_RIGHT_BUTTON && GLUT_ACTIVE_CTRL==glutGetModifiers())
        {
			// do something here
			//std::cout << "dolly click" << std::endl;
            lastx = x;
            lasty = y;
            
            lastcam_dist = cam_dist;

            mouseRotatePressed = false;
            mouseMovePressed   = false;
            mouseZoomPressed   = false;
            mouseDollyPressed  = true;
            mouseSeekPressed   = false;
        
        }
        
        else if (button==GLUT_LEFT_BUTTON)
        {   
            float len;

            //projection
            lastpoint[0] = (2.0*x - width)/width;
            lastpoint[1] = (height - 2.0*y)/height;
            len = lastpoint[0]*lastpoint[0] + lastpoint[1]*lastpoint[1];
			if (len > 1)
            {
                lastpoint[0] /= sqrt(len); lastpoint[1] /= sqrt(len);
                lastpoint[2] = 0;
            }
            else 
            { 
                lastpoint[2] = sqrt(1.0 - lastpoint[0]*lastpoint[0] - lastpoint[1]*lastpoint[1]); 
            }

            lastorient.w = 0;
            lastorient.v[0] = cam_x; lastorient.v[1] = cam_y; lastorient.v[2] =cam_z;
            normalize(lastorient);
            
            lastvx = vx; lastvy = vy; lastvz = vz;

            mouseMovePressed   = false;
            mouseRotatePressed = true;
            mouseZoomPressed   = false;
            mouseDollyPressed  = false;
            mouseSeekPressed   = false;
        
        }
    }
    glutPostRedisplay();
}



void motionCB(int x, int y) {
    if (mouseRotatePressed == true)
	{
        float len;
        float axis[3];

        currpoint[0] = (2.0*x - width)/width;
        currpoint[1] = (height - 2.0*y)/height;
        len = currpoint[0]*currpoint[0] + currpoint[1]*currpoint[1];
		if (len> 1)
        {
            currpoint[0] /= sqrt(len); currpoint[1] /= sqrt(len);
            currpoint[2] = 0;
        }
        else { currpoint[2] = sqrt(1.0 - currpoint[0]*currpoint[0] - currpoint[1]*currpoint[1]); }
       
        crossproduct(currpoint,lastpoint,axis);   
        len = sqrt((pow(axis[0],2))+(pow(axis[1],2))+(pow(axis[2],2)));      
        angle = atan2(len, innerproduct(lastpoint,currpoint));      
        normalize(axis); 



        rotate.w = cos(angle/2);
        rotate.v[0] = axis[0]*sin(angle/2);
        rotate.v[1] = axis[1]*sin(angle/2); 
        rotate.v[2] = axis[2]*sin(angle/2); 

        currorient = qrotate(lastorient,rotate);


        normalize(currorient);

        
        cam_x = currorient.v[0]; cam_y = currorient.v[1]; cam_z = currorient.v[2];
        
        
        float look[3] = {cam_x,cam_y,cam_z};
        float worldUp[3] = {0.0,1.0,0.0};
        float right[3], up[3];
        crossproduct(worldUp,look,right);
        crossproduct(look, right,up);
        normalize(up);
        vx = up[0]; vy = up[1]; vz = up[2];

        // float normal[3];
        // float upvec[3] = {vx,vy,vz};
        // crossproduct(currorient.v,axis,normal);
        // normalize(normal);
        // float a = innerproduct(axis, upvec);
        // float b = sqrt(1-a*a);
        // upvec[0] = a*axis[0] + b*normal[0];
        // upvec[1] = a*axis[1] + b*normal[1];
        // upvec[2] = a*axis[2] + b*normal[2];
        // normalize(upvec);
        // vx = upvec[0]; vy = upvec[1]; vz = upvec[2];


        
        std::cout <<"x:" <<vx <<" ,"<< "y:" <<vy <<" ,"<<"z:" << vz<< std::endl;



		reshape(width, height);
	}
    else if (mouseMovePressed == true)
    {

    
        lookat_x = lastlookat_x + (float)(lastx-x)*0.005;       
        lookat_y = lastlookat_y + (float)(y-lasty)*0.005;

        reshape(width, height);

    }
    else if (mouseZoomPressed == true)
    {

        fov = lastfov + (float)(y - lasty)*0.01;
		reshape(width, height);

    }
    else if (mouseDollyPressed == true)
    {

        cam_dist = lastcam_dist + (float)(y - lasty)*0.01;
		reshape(width, height);

    }

    glutPostRedisplay();
}

void idle() { glutPostRedisplay(); }

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("3D viewer");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    reshape(width, height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_SMOOTH);
    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_POINT);

    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCB);
    glutReshapeFunc(reshape);
    glutMotionFunc(motionCB);
    glutMouseFunc(mouseCB);

    glutMainLoop();
    gluDeleteQuadric(obj);
    return 0;
}
