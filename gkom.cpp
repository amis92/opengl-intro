/*
 * GKOM Lab. 3: OpenGL
 *
 * Program stanowi zaadaptowana wersje przykladu accnot.c.
 */

#include <windows.h>
#include <GL/gl.h>
#include "glut.h"

void init()
{
    GLfloat mat_ambient[]    = { 1.0, 1.0,  1.0, 1.0 };
    GLfloat mat_specular[]   = { 1.0, 1.0,  1.0, 1.0 };
    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    GLfloat lm_ambient[]     = { 0.2, 0.2,  0.2, 1.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialf( GL_FRONT, GL_SHININESS, 50.0 );
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lm_ambient );
    
    glShadeModel( GL_SMOOTH );

    glEnable( GL_LIGHTING );
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

    glDepthFunc( GL_LESS );
    glEnable( GL_DEPTH_TEST );
}

void displayObjects(int frame_no)
{
	GLfloat torus_diffuse[]  = { 0.7, 0.7, 0.0, 1.0 };
    GLfloat cube_diffuse[]   = { 0.0, 0.7, 0.7, 1.0 };
    GLfloat sphere_diffuse[] = { 0.7, 0.0, 0.7, 1.0 };
    GLfloat octa_diffuse[]   = { 0.7, 0.4, 0.4, 1.0 };

    glPushMatrix();
	{
		glRotatef(30.0, 1.0, 0.0, 0.0);
		glPushMatrix();
		{
			glRotatef(1.0 * frame_no, 0.0, 1.0, 0.0);
			glPushMatrix();
			{
			 glTranslatef(-1.5, 1.5, 0.0);
			 glRotatef(100.0, 1.0, 0.0, 0.0);
			 glRotatef(1.0 * frame_no, 1.0, 0.0, 0.0);
			 glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse);
			 glutSolidTorus(0.275, 0.85, 10, 10);
			}
			glPopMatrix();
			glPushMatrix();
			{
			 glTranslatef(1.5, 1.5, 0.0);
			 glRotatef(30.0, 1.0, 0.0, 0.0);
			 glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_diffuse);
			 glutSolidSphere(1.0, 10, 10);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glPushMatrix();
		{
		    glRotatef(-1.0 * frame_no, 0.0, 1.0, 0.0);
		    glPushMatrix();
		    {
				glTranslatef(-1.5, -1.5, 0.0);
		        glRotatef(45.0, 0.0, 0.0, 1.0);
		        glRotatef(45.0, 1.0, 0.0, 0.0);
		        glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
		        glutSolidCube(1.5);
		    }
		    glPopMatrix();
		    glPushMatrix();
		    {
		        glTranslatef(1.5, -1.5, 0.0);
		        glMaterialfv(GL_FRONT, GL_DIFFUSE, octa_diffuse);
		        glutSolidTeapot(1.0);
		    }
		    glPopMatrix();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_diffuse);
			glutSolidCube(1.5);
		}
		glPopMatrix();
	}
	glPopMatrix();	 
}

void display()
{
	static int frame_no = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glRotatef(1.0 * frame_no, 0.0, 1.0, 0.0);
		glMatrixMode(GL_MODELVIEW);
		displayObjects(frame_no);
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();

    glFlush();
	glutSwapBuffers();

	++frame_no;
	if (frame_no > 360)
	{
		frame_no = 0;
	}
}

void reshape(GLsizei w, GLsizei h)
{
    if( h > 0 && w > 0 ) {
      glViewport( 0, 0, w, h );
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      if( w <= h ) {
         //glOrtho( -2.25, 2.25, -2.25*h/w, 2.25*h/w, -10.0, 10.0 );
		  glFrustum(-1.5, 1.5, -1.5 * h / w, 1.5 * h / w, 1.0, 10.0);
      }
      else
	  {
		  //glOrtho(-2.25*w / h, 2.25*w / h, -2.25, 2.25, -10.0, 10.0);
		  glFrustum(-1.5 * w / h, 1.5 * w / h, -1.5, 1.5, 1.0, 10.0);
      }
	  glTranslatef(0.0, 0.0, -5.5);
      glMatrixMode( GL_MODELVIEW );
    }
}

int main(int argc, char** argv)
{
   glutInit( &argc, argv );

   glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

   glutInitWindowPosition( 0, 0 );
   glutInitWindowSize( 400, 400 );

   glutCreateWindow( "GPOB: OpenGL" );

   glutDisplayFunc( display );
   glutReshapeFunc( reshape );
   glutIdleFunc( display );

   init();

   glutMainLoop();
   
   return 0;
}
