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
float ex=3.5,ey=2.5,ez=5,lx=3.34607,ly=2.5,lz=8.99704;

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
    {1.0, 0.0, 0.0},  //color for point index 1
    {0.0, 1.0, 0.0},  //color for point index 2
    {0.0, 1.0, 1.0},  //color for point index 3
    {1.0, 0.0, 1.0}   //color for point index 4
};


GLfloat ambient[]    = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuse[]    = { 0.8, 0.8, 0.8, 1.0};
GLfloat specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat shininess[] = { 30.0f };

bool ambient_on = true;
bool specular_on = true;
bool diffuse_on = true;

bool light_ambient_on = true;
bool light_diffuse_on = true;
bool light_specular_on = true;

bool light1_ambient_on = true;
bool light1_diffuse_on = true;
bool light1_specular_on = false;

bool light2_ambient_on = false;
bool light2_diffuse_on = true;
bool light2_specular_on = true;

bool light3_ambient_on = false;
bool light3_diffuse_on = false;
bool light3_specular_on = true;

bool light1_on = true;
bool light2_on = true;
bool light3_on = true;


float linear_attenuation = 0.1f;
float constant_attenuation = 2.0f;


//Light 0
GLfloat light0_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat light0_diffuse[]  = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light0_position[] = { 2.5f, 2.5f, 2.5f, 1.0f };

//Light 1
GLfloat light1_ambient[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat light1_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light1_position[] = { 2.0f, 2.0f, 2.0f, 1.0f };

//Light 2
GLfloat light2_ambient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat light2_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light2_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light2_position[] = { 0.0f, 2.0f, 5.0f, 1.0f };

GLfloat no_light[] = {0.0f, 0.0f, 0.0f, 1.0f};

void set_color(GLfloat* ambient_col, GLfloat* diffuse_col, GLfloat* specular_col, GLfloat* shininess)
{
    if (ambient_on == true)
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_col);
    else
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_light);

    if (diffuse_on == true)
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  diffuse_col);
    else
        glMaterialfv(GL_FRONT, GL_DIFFUSE, no_light);

    if (specular_on == true)
        glMaterialfv(GL_FRONT, GL_SPECULAR,  specular_col);
    else
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_light);

    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

}

void set_light(int light_id, bool light_on, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* position)
{
    int id;
    if (light_id == 0)
        id = GL_LIGHT0;
    else if (light_id == 1)
        id = GL_LIGHT1;
    else if (light_id == 2)
        id = GL_LIGHT2;

    if (light_on == true)
        glEnable(id);
    else
        glDisable(id);

    if (light_ambient_on == true)
        glLightfv(id, GL_AMBIENT,  ambient);
    else
        glLightfv(id, GL_AMBIENT,  no_light);

    if (light_diffuse_on == true)
        glLightfv(id, GL_DIFFUSE,  diffuse);
    else
        glLightfv(id, GL_DIFFUSE,  no_light);

    if (light_specular_on == true)
        glLightfv(id, GL_SPECULAR, specular);
    else
        glLightfv(id, GL_SPECULAR, no_light);

    glLightfv(id, GL_POSITION, position);

    glLightf(id, GL_LINEAR_ATTENUATION, linear_attenuation );
    glLightf(id,GL_CONSTANT_ATTENUATION, constant_attenuation);
}

void set_light1(int light_id, bool light_on, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* position)
{
    int id;
    if (light_id == 0)
        id = GL_LIGHT0;
    else if (light_id == 1)
        id = GL_LIGHT1;
    else if (light_id == 2)
        id = GL_LIGHT2;

    if (light_on == true)
        glEnable(id);
    else
        glDisable(id);

    if (light1_ambient_on == true)
        glLightfv(id, GL_AMBIENT,  ambient);
    else
        glLightfv(id, GL_AMBIENT,  no_light);

    if (light1_diffuse_on == true)
        glLightfv(id, GL_DIFFUSE,  diffuse);
    else
        glLightfv(id, GL_DIFFUSE,  no_light);

    if (light1_specular_on == true)
        glLightfv(id, GL_SPECULAR, specular);
    else
        glLightfv(id, GL_SPECULAR, no_light);

    glLightfv(id, GL_POSITION, position);

    glLightf(id, GL_LINEAR_ATTENUATION, linear_attenuation );
    glLightf(id,GL_CONSTANT_ATTENUATION, constant_attenuation);
}

void set_light2(int light_id, bool light_on, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* position)
{
    int id;
    if (light_id == 0)
        id = GL_LIGHT0;
    else if (light_id == 1)
        id = GL_LIGHT1;
    else if (light_id == 2)
        id = GL_LIGHT2;

    if (light_on == true)
        glEnable(id);
    else
        glDisable(id);

    if (light2_ambient_on == true)
        glLightfv(id, GL_AMBIENT,  ambient);
    else
        glLightfv(id, GL_AMBIENT,  no_light);

    if (light2_diffuse_on == true)
        glLightfv(id, GL_DIFFUSE,  diffuse);
    else
        glLightfv(id, GL_DIFFUSE,  no_light);

    if (light2_specular_on == true)
        glLightfv(id, GL_SPECULAR, specular);
    else
        glLightfv(id, GL_SPECULAR, no_light);

    glLightfv(id, GL_POSITION, position);

    glLightf(id, GL_LINEAR_ATTENUATION, linear_attenuation );
    glLightf(id,GL_CONSTANT_ATTENUATION, constant_attenuation);
}

void set_light3(int light_id, bool light_on, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* position)
{
    int id;
    if (light_id == 0)
        id = GL_LIGHT0;
    else if (light_id == 1)
        id = GL_LIGHT1;
    else if (light_id == 2)
        id = GL_LIGHT2;

    if (light_on == true)
        glEnable(id);
    else
        glDisable(id);

    if (light3_ambient_on == true)
        glLightfv(id, GL_AMBIENT,  ambient);
    else
        glLightfv(id, GL_AMBIENT,  no_light);

    if (light3_diffuse_on == true)
        glLightfv(id, GL_DIFFUSE,  diffuse);
    else
        glLightfv(id, GL_DIFFUSE,  no_light);

    if (light3_specular_on == true)
        glLightfv(id, GL_SPECULAR, specular);
    else
        glLightfv(id, GL_SPECULAR, no_light);

    glLightfv(id, GL_POSITION, position);

    glLightf(id, GL_LINEAR_ATTENUATION, linear_attenuation );
    glLightf(id,GL_CONSTANT_ATTENUATION, constant_attenuation);
}


void drawroom()
{
    glBegin(GL_QUADS);
    for (GLint i = 0; i <5; i++)
    {
        if(i==0)
        {
            //glColor3fv(&colors[quadIndices[i][0]][0]);
            GLfloat diffuse9[]  = {0.0, 0.0, 1.0, 1.0};
            set_color(ambient,diffuse9,specular,shininess);
        }
        else if(i==1)
        {


            //glColor3fv(&colors[quadIndices[i][1]][0]);
            GLfloat diffuse10[]    = {1.0, 0.0, 0.0, 1.0};
            set_color(ambient,diffuse10,specular,shininess);
        }
        else if(i==2)
        {

            //glColor3fv(&colors[quadIndices[i][1]][0]);
            GLfloat diffuse11[]    = {0.0, 1.0, 0.0, 1.0};
            set_color(ambient,diffuse11,specular,shininess);
        }
        else if(i==3)
        {


            //glColor3fv(&colors[quadIndices[i][3]][0]);
            GLfloat diffuse12[]    = {0.0, 1.0, 1.0, 1.0};
            set_color(ambient,diffuse12,specular,shininess);
        }
        else if(i==4)
        {


            //glColor3fv(&colors[quadIndices[i][4]][0]);
            GLfloat diffuse13[]    = {1.0, 0.0, 1.0, 1.0};
            set_color(ambient,diffuse13,specular,shininess);
        }

        glVertex3fv(&v_room[quadIndices[i][0]][0]);
        //glColor3fv(&colors[quadIndices[i][1]][0]);
        glVertex3fv(&v_room[quadIndices[i][1]][0]);
        //glColor3fv(&colors[quadIndices[i][2]][0]);
        glVertex3fv(&v_room[quadIndices[i][2]][0]);
        //glColor3fv(&colors[quadIndices[i][3]][1]);
        glVertex3fv(&v_room[quadIndices[i][3]][0]);

    }
    glEnd();
}


void draw_sofa(float x, float y, float z)
{
    glPushMatrix();
    GLfloat diffuse2[]    = { 1, 1, 1, 1 };
    set_color(ambient,diffuse2,specular,shininess);
    //glColor4f(1,1,1,1);
    glTranslatef(x,y,z);
    glScalef(3,0.75,1.25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1.0,0.5,0.5,1);
    GLfloat diffuse3[]= {1.0,0.5,0.5,1};
    set_color(ambient,diffuse3,specular,shininess);
    glTranslatef(x,y+0.01,z+0.01);
    glScalef(2.75,.75,1.25);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    //glColor4f(1,0.0784314,0.576471,1);
    GLfloat diffuse4[]= {1,0.0784314,0.576471,1};
    set_color(ambient,diffuse4,specular,shininess);
    glTranslatef(x-1.40,y+0.50,z);
    glScalef(0.20,0.25,1.25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.0784314,0.576471,1);
    set_color(ambient,diffuse4,specular,shininess);
    glTranslatef(1.40+x,y+0.50,z);
    glScalef(0.20,0.25,1.25);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.0784314,0.576471,1);
    set_color(ambient,diffuse4,specular,shininess);
    glTranslatef(0+x,y+0.50,z-.575);
    //cout<<z<<endl;
    glScalef(2.50,1.50,0.10);
    glutSolidCube(1);
    glPopMatrix();

}

void draw_table(float x, float y, float z)
{
    glPushMatrix();
    //glColor4f(0,0,1,1);
    GLfloat diffuse6[]    = { 0,1,1,1 };
    set_color(ambient,diffuse6,specular,shininess);
    glTranslatef(x+1,y+.625,z+0.30);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0,0,1,1);
    set_color(ambient,diffuse6,specular,shininess);
    glTranslatef(x+2,y+.625,z+0.30);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0,0,1,1);
    set_color(ambient,diffuse6,specular,shininess);
    glTranslatef(x+1,y+.625,z);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0,0,1,1);
    set_color(ambient,diffuse6,specular,shininess);
    glTranslatef(x+2,y+.625,z);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,1,1,1);
    GLfloat diffuse7[]    = { 1,1,1,1 };
    set_color(ambient,diffuse7,specular,shininess);
    glTranslatef(x+1.50,y+1.25,z);
    glScalef(2,0.10,1.0);
    glutSolidCube(1);
    glPopMatrix();
}

void draw_shahidminar(float x, float y, float z)
{
    glPushMatrix();
    //glColor4f(0.80,0.80,1,1);
    GLfloat diffuse[]    = { 0.80f, 0.80f, 1.0f, 1.0f };
    set_color(ambient,diffuse,specular,shininess);
    glTranslatef(x-.50,0.5+y,z);
    //cout<<x<<endl;
    glScalef(1.50,0.10,0.50);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    GLfloat diffuse1[]    = { 0.698039f, 0.133333f, 0.133333f, 1.0f };
    set_color(ambient,diffuse1,specular,shininess);
    //glColor4f(0.698039,0.133333,0.133333,1);
    glTranslatef(x-1.0,.75+y,z);
    //cout<<y<<endl;
    glRotated(60,0,1,0);
    glScalef(0.30,.50,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    set_color(ambient,diffuse1,specular,shininess);
    //glColor4f(0.598039,0.103333,0.103333,1);
    glTranslatef(x-.60,0.80+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0.598039,0.103333,0.103333,1);
    set_color(ambient,diffuse1,specular,shininess);
    glTranslatef(x-.80,0.80+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0.598039,0.103333,0.103333,1);
    set_color(ambient,diffuse1,specular,shininess);
    glTranslatef(x-.40,0.80+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0.698039,0.133333,0.133333,1);
    set_color(ambient,diffuse1,specular,shininess);
    glTranslatef(x-.60,1.40+y,z+0.20);
    //cout<<y<<endl;
    glRotated(60,1,0,0);
    glScalef(0.50,.50,0.10);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    //glColor4f(0.698039,0.133333,0.133333,1);
    set_color(ambient,diffuse1,specular,shininess);
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
    //glColor4f(0.80,0.80,1,1);
    GLfloat diffuse7[]    = { 0,0,1,1 };
    set_color(ambient,diffuse7,specular,shininess);
    glTranslatef(x+1.75,1.50+y,z-2);
    //cout<<x<<endl;
    glScalef(0.50,3,2);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.270588,0,1);
    GLfloat diffuse8[]    = {1,0.270588,0,1 };
    set_color(ambient,diffuse8,specular,shininess);
    glTranslatef(x+1.50,1.5+y,z-2.50);
    //cout<<x<<endl;
    glScalef(0.10,3,.85);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.270588,0,1);
    set_color(ambient,diffuse8,specular,shininess);
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
    GLfloat diffuse5[]= {1.0,0.0,0.0};
    set_color(ambient,diffuse5,specular,shininess);
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

void builddesign()
{
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


}


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

//
//    set_light(0,light1_on,light0_ambient,light0_diffuse,light0_specular,light0_position);
//    set_light(1,light2_on,light1_ambient,light1_diffuse,light1_specular,light1_position);
//    set_light(2,light3_on,light2_ambient,light2_diffuse,light2_specular,light2_position);

    gluLookAt( lx,ly,lz,ex,ey,ez,0,1,0);

    set_light(0,light1_on,light0_ambient,light0_diffuse,light0_specular,light0_position);
    set_light(1,light2_on,light1_ambient,light1_diffuse,light1_specular,light1_position);
    set_light(2,light3_on,light2_ambient,light2_diffuse,light2_specular,light2_position);
    glViewport(0,0,400,400);
    builddesign();

    glViewport(0,400,400,400);
        set_light1(0,light1_on,light0_ambient,light0_diffuse,light0_specular,light0_position);
    set_light1(1,light2_on,light1_ambient,light1_diffuse,light1_specular,light1_position);
    set_light1(2,light3_on,light2_ambient,light2_diffuse,light2_specular,light2_position);
    builddesign();

    glViewport(400,0,400,400);
        set_light2(0,light1_on,light0_ambient,light0_diffuse,light0_specular,light0_position);
    set_light2(1,light2_on,light1_ambient,light1_diffuse,light1_specular,light1_position);
    set_light2(2,light3_on,light2_ambient,light2_diffuse,light2_specular,light2_position);
    builddesign();

    glViewport(400,400,400,400);
        set_light3(0,light1_on,light0_ambient,light0_diffuse,light0_specular,light0_position);
    set_light3(1,light2_on,light1_ambient,light1_diffuse,light1_specular,light1_position);
    set_light3(2,light3_on,light2_ambient,light2_diffuse,light2_specular,light2_position);
    builddesign();

    glFlush();
    glutSwapBuffers();
}

float mm,xx,yy,zz;
float cam_angle = 90/180.0f * 3.1416;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        cam_angle = cam_angle - 0.01;

        lx = ex + cos(cam_angle)*4;
        ly = ey;
        lz = ez + sin(cam_angle)*4;
        cout<<ex<<" "<<ey<<" "<<ez<<endl;
        cout<<lx<<" "<<ly<<" "<<lz<<endl;
        break;
    case 'k':
        cam_angle = cam_angle + 0.01;

        lx = ex + cos(cam_angle)*4;
        ly = ey;
        lz = ez + sin(cam_angle)*4;
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

    case 'f':
        bRotate = !bRotate;
        uRotate = false;
        axis_x=0.0;
        axis_y=1.0;
        break;

    case 'r':
        uRotate = !uRotate;
        bRotate = false;
        axis_x=1.0;
        axis_y=0.0;
        break;

    case 'a':
        light_ambient_on = !light_ambient_on;
        break;

    case 'd':
        light_diffuse_on = !light_diffuse_on;
        break;

    case 's':
        light_specular_on = !light_specular_on;
        break;

    case '1':
        ambient_on = !ambient_on;
        break;

    case '2':
        diffuse_on = !diffuse_on;
        break;

    case '3':
        specular_on = !specular_on;
        break;

    case 'z':
        light1_on = !light1_on;
        break;

    case 'x':
        light2_on = !light2_on;
        break;

    case 'c':
        light3_on = !light3_on;
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
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glutMainLoop();

    return 0;
}
