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

static int shoulder= 0;
static int elbow = 0, thumb = 0, index = 0;


void drawfinger(void) 
{
   glPushMatrix();
      glScalef(0.3,0.2,0.3);
      glutWireCube(1.0);
   glPopMatrix();

}

void display(void)
{
    // clear all pixels 
    glClear (GL_COLOR_BUFFER_BIT);

   glPushMatrix(); 
   
   glTranslatef(-2.0,0.0,0.0);
   
   
   glRotatef((GLfloat) shoulder, 0.0,0.0,1.0);
   glTranslatef(1.0,0.0,0.0);
   glPushMatrix();
      glScalef(2.0,0.4,1.0);
      glutWireCube(1.0);
   glPopMatrix();
   
   glTranslatef(1.0,0.2,0.0);
   glRotatef((GLfloat) elbow, 0.0,0.0,1.0);
   glTranslatef(1.0,-0.2,0.0);
   glPushMatrix();
      glScalef(2.0,0.4,1.0);
      glutSolidCube(1.0);
   glPopMatrix();


   //glPushMatrix();
   
   glPushMatrix();
      glTranslatef(1.0, 0.2,-0.35);
      glRotatef((GLfloat) thumb, 0.0,0.0,1.0);
      glTranslatef(0.15,-0.1,0.0);
      drawfinger();
   glPopMatrix();
  
   
   
   glPushMatrix();
      glTranslatef(1.0, 0.2,0.0);
      glRotatef((GLfloat) thumb, 0.0,0.0,1.0);
       glTranslatef(0.15,-0.1,0.0);
      drawfinger();
   glPopMatrix();
  
   
   
   glPushMatrix();
      glTranslatef(1.0, 0.2,0.35);
      glRotatef((GLfloat) thumb, 0.0,0.0,1.0);
      glTranslatef(0.15,-0.1,0.0);
      drawfinger();
   glPopMatrix();
   //glPopMatrix();
   
  
   
   glPopMatrix();
   
   glutSwapBuffers(); //start processing buffered OpenGL routines

} 

void keyboard(unsigned char key, int x, int y)
{
    switch (key) 
{
   case 'd':
      shoulder = (shoulder + 10) % 360;
      
      glutPostRedisplay();
      break;
   case 'f':
      elbow = (elbow + 10) % 360;
      glutPostRedisplay();
      break;        
   case 'v':
      if (elbow > 0 )
      {
      elbow = (elbow - 10) % 360;
      glutPostRedisplay();
      }
      break;        
   case 'q':
      thumb = (thumb + 10) % 360;
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
    gluLookAt(2.0,5.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);


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