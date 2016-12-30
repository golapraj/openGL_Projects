#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include<iostream>
using namespace std;



GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;

int rh=5,rw=5,rl=7;
float ex=3,ey=4,ez=13,lx=3,ly=4,lz=12;


static GLfloat v_room[8][3] =
{
    {0.0, 0.0, 0.0},  //point index 0
    {0.0, 0.0, rw},  //point index 1
    {rl, 0.0, rw},  //point index 2
    {rl, 0.0, 0.0},  //point index 3
    {0.0, rh, 0.0},   //point index 4
    {0.0,rh,rw},    //point index 5
    {rl,rh,rw},     //point index 6
    {rl,rh,0.0}     //point index 7
};

static GLubyte quadIndices[5][4] =
{
    {0, 3, 2, 1},
    {0, 1, 5, 4},
    {0, 4, 7, 3},
    {2, 6, 7, 3},
    {4, 5, 6, 7}
};  // indeces for drawing the quad plane


static GLfloat colors[5][3] =
{
    {0.0, 0.0, 1.0},  //color for point index 0
    {0.5, 0.0, 1.0},  //color for point index 1
    {1.0, 1.0, 0.0},  //color for point index 2
    {0.0, 1.0, 1.0},  //color for point index 3
    {1.0, 0.0, 1.0}   //color for point index 4
};


void drawroom()
{

    glBegin(GL_QUADS);
    for (GLint i = 0; i <5; i++)
    {
        glColor3fv(&colors[quadIndices[i][1]][0]);
        glVertex3fv(&v_room[quadIndices[i][0]][0]);
        glColor3fv(&colors[quadIndices[i][1]][0]);
        glVertex3fv(&v_room[quadIndices[i][1]][0]);
        glColor3fv(&colors[quadIndices[i][2]][0]);
        glVertex3fv(&v_room[quadIndices[i][2]][0]);
        glColor3fv(&colors[quadIndices[i][3]][1]);
        glVertex3fv(&v_room[quadIndices[i][3]][0]);

    }
    glEnd();
}


void draw_sofa(float x, float y, float z)
{
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x,y,z);
    glScalef(3,0.75,1.25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1.0,0.5,0.5,1);
    glTranslatef(x,y+0.01,z+0.01);
    glScalef(2.75,.75,1.25);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    glColor4f(1,0.0784314,0.576471,1);
    glTranslatef(x-1.40,y+0.50,z);
    glScalef(0.20,0.25,1.25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,0.0784314,0.576471,1);
    glTranslatef(1.40+x,y+0.50,z);
    glScalef(0.20,0.25,1.25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,0.0784314,0.576471,1);
    glTranslatef(0+x,y+0.50,z-.575);
    //cout<<z<<endl;
    glScalef(2.50,1.50,0.10);
    glutSolidCube(1);
    glPopMatrix();

}

void draw_table(float x, float y, float z)
{
    glPushMatrix();
    glColor4f(0,0,1,1);
    glTranslatef(x+1,y+.625,z+0.50);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0,0,1,1);
    glTranslatef(x+2,y+.625,z+0.50);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0,0,1,1);
    glTranslatef(x+1,y+.625,z);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0,0,1,1);
    glTranslatef(x+2,y+.625,z);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x+1.50,y+1.25,z);
    glScalef(2,0.10,1.0);
    glutSolidCube(1);
    glPopMatrix();
}

void draw_shahidminar(float x, float y, float z)
{
    glPushMatrix();
   glColor4f(0.80,0.80,1,1);
    glTranslatef(x-.50,0.5+y,z);
    //cout<<x<<endl;
    glScalef(1.50,0.10,0.50);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    glColor4f(0.698039,0.133333,0.133333,1);
    glTranslatef(x-1.0,.75+y,z);
    //cout<<y<<endl;
    glRotated(60,0,1,0);
    glScalef(0.30,.50,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.598039,0.103333,0.103333,1);
    glTranslatef(x-.60,0.80+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.598039,0.103333,0.103333,1);
    glTranslatef(x-.80,0.80+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.598039,0.103333,0.103333,1);
    glTranslatef(x-.40,0.80+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.698039,0.133333,0.133333,1);
    glTranslatef(x-.60,1.40+y,z);
    //cout<<y<<endl;
    glRotated(60,1,0,0);
    glScalef(0.50,.50,0.10);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    glColor4f(0.698039,0.133333,0.133333,1);
    glTranslatef(x-.20,.75+y,z);
    //cout<<y<<endl;
    glRotated(-60,0,1,0);
    glScalef(0.30,.50,0.10);
    glutSolidCube(1);
    glPopMatrix();


}



void draw_alm(float x, float y, float z)
{
    glPushMatrix();
    glColor4f(0.80,0.80,1,1);
    glTranslatef(x+1.75,1.50+y,z-2);
    //cout<<x<<endl;
    glScalef(0.50,3,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,0.270588,0,1);
    glTranslatef(x+1.50,1.5+y,z-2.50);
    //cout<<x<<endl;
    glScalef(0.10,3,.85);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1,0.270588,0,1);
    glTranslatef(x+1.50,1.5+y,z-1.50);
    //cout<<x<<endl;
    glScalef(0.10,3,.85);
    glutSolidCube(1);
    glPopMatrix();
}

void ball(float x, float y, float radius)
{
    int i;
    int triangleAmount = 20;

    float twicePi = 2.0f * 3.1416;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(x, y); // center of circle
    for(i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (radius * cos(i *  twicePi / triangleAmount)),
            y + (radius * sin(i * twicePi / triangleAmount))
        );
    }
    glEnd();
}
int i = 0;

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum(-5,5,-5,5, 4, 50);
    gluPerspective(65,1,0.50,20);
    //glOrtho(-5,5,-5,5, 4, 50);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt(ex,ey,ez, lx,ly,lz, 0,1,0);

    glViewport(0,0,800,800);


    glPushMatrix();

    glRotatef( theta, 1, 0, 0.0 );
    glRotatef( alpha, 0, 1, 0.0 );

    drawroom();

    draw_sofa(3.0,0.375,0.625);
    glTranslatef(0.0,0.0,5);
    glRotatef(270,0,1,0);
    glRotatef(180,0,1,0);
    draw_sofa(1.5,0.375,0.625);

    glRotatef(-270,0,1,0);
    glRotatef(-180,0,1,0);

    draw_table(2,0,-2);

    draw_shahidminar(4,0.80,-2.0);

    draw_alm(5,0,0);

    glTranslatef(1.375,-.25,-2);
    ball(2,2,.25);

    glPopMatrix();
    glFlush();
    glutSwapBuffers();

}

float mm,xx,yy,zz;
float cam_angle = 270/180.0f * 3.1416;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        cam_angle = cam_angle - 0.01;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);
        cout<<ex<<" "<<ey<<" "<<ez<<endl;
        cout<<lx<<" "<<ly<<" "<<lz<<endl;
        break;
    case 'k':
        cam_angle = cam_angle + 0.01;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);
        cout<<ex<<" "<<ey<<" "<<ez<<endl;
        cout<<lx<<" "<<ly<<" "<<lz<<endl;
        break;

    case 't':
        ex = ex + cos(cam_angle) * 0.5;
        ez = ez + sin(cam_angle) * 0.5;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);
        cout<<ex<<" "<<ey<<" "<<ez<<endl;
        cout<<lx<<" "<<ly<<" "<<lz<<endl;
        break;
    case 'g':
        ex = ex - cos(cam_angle) * 0.5;
        ez = ez - sin(cam_angle) * 0.5;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);
        cout<<ex<<" "<<ey<<" "<<ez<<endl;
        cout<<lx<<" "<<ly<<" "<<lz<<endl;
        break;

    case 's':
    case 'S':
        bRotate = !bRotate;
        uRotate = false;
        axis_x=0.0;
        axis_y=1.0;
        break;

    case 'r':
    case 'R':
        uRotate = !uRotate;
        bRotate = false;
        axis_x=1.0;
        axis_y=0.0;
        break;

    case 27:	// Escape key
        exit(1);
    }
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.1;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.1;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }
    glutPostRedisplay();

}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(800,800);
    glutCreateWindow("myRoom");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(myKeyboardFunc);

    glutDisplayFunc(display);

    glutIdleFunc(animate);

    glEnable(GL_NORMALIZE);

    glutMainLoop();

    return 0;
}
