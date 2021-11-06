
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


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glPushMatrix();
    drawSomething();
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

    return 0;
}
