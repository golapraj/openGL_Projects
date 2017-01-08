#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include<iostream>
#include<stdio.h>
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
using namespace std;


GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;

int rh=5,rw=5,rl=7;
float ex=3.5,ey=2.5,ez=9.5,lx=3.5,ly=2.5,lz=10.5;

struct Image_Data
{
    unsigned long Size_X;
    unsigned long Size_Y;
    unsigned char *Data;
};

int Load_Bitmap(char* Filename, Image_Data* Out_Image_Data)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3

    FILE * file = fopen(Filename,"rb");
    if (!file)
    {
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 )  // If not 54 bytes read : problem
    {
        return 0;
    }

    if ( header[0]!='B' || header[1]!='M' )
    {
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    Out_Image_Data->Data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(Out_Image_Data->Data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    Out_Image_Data->Size_X = width;
    Out_Image_Data->Size_Y = height;

    return 1;
}

void Add_Texture(int ID, Image_Data* Data)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Data->Size_X,Data->Size_Y,0,GL_BGR,GL_UNSIGNED_BYTE,Data->Data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
}

GLuint Textures[10];


GLfloat ambient[]    = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat diffuse[]    = { 1.0, 1.0, 1.0, 1.0};
GLfloat specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat shininess[] = { 50.0f };

bool ambient_on = true;
bool specular_on = true;
bool diffuse_on = true;

bool light_ambient_on = true;
bool light_diffuse_on = true;
bool light_specular_on = true;

bool light1_on = true;
bool light2_on = true;
bool light3_on = true;


float linear_attenuation = 0.1f;
float constant_attenuation = 2.0f;


//Light 0
GLfloat light0_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat light0_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light0_position[] = { 1, 3, 5, 1.0f };

//Light 1
GLfloat light1_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat light1_diffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light1_position[] = {2.5f, 4, 5, 1.0f };

//Light 2
GLfloat light2_ambient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat light2_diffuse[]  = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat light2_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light2_position[] = { 4, 5,5, 1.0f };

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



void drawroom()
{
    glEnable(GL_TEXTURE_2D);



    for (GLint i = 0; i <5; i++)
    {
        if(i==0)
        {
            //glColor3fv(&colors[quadIndices[i][0]][0]);
            GLfloat diffuse9[]  = {1.0, 1.0, 1.0, 1.0};
            set_color(ambient,diffuse9,specular,shininess);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBegin(GL_QUADS);
        }

        else if(i==1)
        {
            //glColor3fv(&colors[quadIndices[i][1]][0]);
            GLfloat diffuse11[]    = {1.0, 1.0, 1.0, 1.0};
            set_color(ambient,diffuse11,specular,shininess);
            glBindTexture(GL_TEXTURE_2D, 9);
            glBegin(GL_QUADS);
        }

        else if(i==2||i==3)
        {

            //glColor3fv(&colors[quadIndices[i][1]][0]);
            GLfloat diffuse11[]    = {1.0, 1.0, 1.0, 1.0};
            set_color(ambient,diffuse11,specular,shininess);
            glBindTexture(GL_TEXTURE_2D, 2);
            glBegin(GL_QUADS);

        }

        else if(i==4)
        {
            //glColor3fv(&colors[quadIndices[i][4]][0]);
            GLfloat diffuse13[]    = {1.0, 1.0, 1.0, 1.0};
            set_color(ambient,diffuse13,specular,shininess);
            glBindTexture(GL_TEXTURE_2D, 1);
            glBegin(GL_QUADS);
        }

        glTexCoord2f(0,0);
        glVertex3fv(&v_room[quadIndices[i][0]][0]);
        //glColor3fv(&colors[quadIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_room[quadIndices[i][1]][0]);
        //glColor3fv(&colors[quadIndices[i][2]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_room[quadIndices[i][2]][0]);
        //glColor3fv(&colors[quadIndices[i][3]][1]);
        glTexCoord2f(0,1);
        glVertex3fv(&v_room[quadIndices[i][3]][0]);
        glEnd();
    }


    glDisable(GL_TEXTURE_2D);
}

void draw_solid_cube(GLfloat size)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float x4,y4,z4;

    x1 = -size/2;
    y1 = -size/2;
    z1 = -size/2;

    x2 = size/2;
    y2 = -size/2;
    z2 = -size/2;

    x3 = size/2;
    y3 = -size/2;
    z3 = size/2;

    x4 = -size/2;
    y4 = -size/2;
    z4 = size/2;

    float x5,y5,z5;
    float x6,y6,z6;
    float x7,y7,z7;
    float x8,y8,z8;

    x5 = -size/2;
    y5 = size/2;
    z5 = -size/2;

    x6 = size/2;
    y6 = size/2;
    z6 = -size/2;

    x7 = size/2;
    y7 = size/2;
    z7 = size/2;

    x8 = -size/2;
    y8 = size/2;
    z8 = size/2;

    glBegin(GL_QUADS);
    //Quad 1

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(1,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,1);
    glVertex3f(x1,y1,z1);
    //Quad 4

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,1);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,1);
    glVertex3f(x3,y3,z3);
    //Quad 5
    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    //Quad 6
    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(1,1);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);

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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //glutSolidCube(1);//outer
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1.0,0.5,0.5,1);
    GLfloat diffuse3[]= {1.0,1.0,1.0,1};
    set_color(ambient,diffuse3,specular,shininess);
    glTranslatef(x,y+0.01,z+0.01);
    glScalef(2.75,.75,1.25);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1);//inner
    glPopMatrix();


    glPushMatrix();
    //glColor4f(1,0.0784314,0.576471,1);
    GLfloat diffuse4[]= {1,1.0,1.0,1};
    set_color(ambient,diffuse4,specular,shininess);
    glTranslatef(x-1.40,y+0.50,z);
    glScalef(0.20,0.25,1.25);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glutSolidCube(1); //right hand
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.0784314,0.576471,1);
    set_color(ambient,diffuse4,specular,shininess);
    glTranslatef(1.40+x,y+0.50,z);
    glScalef(0.20,0.25,1.25);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1); //left hand
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.0784314,0.576471,1);
    set_color(ambient,diffuse4,specular,shininess);
    glTranslatef(0+x,y+0.50,z-.575);
    //cout<<z<<endl;
    glScalef(2.50,1.50,0.10);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1);//back
    glPopMatrix();

}

void draw_table(float x, float y, float z)
{
    glPushMatrix();
    //glColor4f(0,0,1,1);
    GLfloat diffuse6[]    = { 0.63,0.29,0.18,1 };
    set_color(diffuse6,diffuse6,diffuse6,shininess);
    glTranslatef(x+1,y+.625,z+0.30);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0,0,1,1);
    set_color(diffuse6,diffuse6,diffuse6,shininess);
    glTranslatef(x+2,y+.625,z+0.30);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0,0,1,1);
    set_color(diffuse6,diffuse6,diffuse6,shininess);
    glTranslatef(x+1,y+.625,z);
    glScalef(0.10,1.25,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0,0,1,1);
    set_color(diffuse6,diffuse6,diffuse6,shininess);
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

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1);
    glPopMatrix();
}

void draw_shahidminar(float x, float y, float z)
{
    glPushMatrix();
    //glColor4f(0.80,0.80,1,1);
    GLfloat diffuse[]    = { 0.60f, 0.60f, 0.6f, 1.0f };
    set_color(ambient,diffuse,specular,shininess);
    glTranslatef(x-.6,0.5+y,z);
    //cout<<x<<endl;
    glScalef(1.5,0.10,0.50);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    GLfloat diffuse1[]    = { 0.698039f, 0.133333f, 0.133333f, 1.0f };
    set_color(diffuse1,diffuse1,diffuse1,shininess);
    //glColor4f(0.698039,0.133333,0.133333,1);
    glTranslatef(x-1.0,.75+y,z);
    //cout<<y<<endl;
    glRotated(60,0,1,0);
    glScalef(0.30,.65,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    set_color(diffuse1,diffuse1,diffuse1,shininess);
    //glColor4f(0.598039,0.103333,0.103333,1);
    glTranslatef(x-.60,1.0+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0.598039,0.103333,0.103333,1);
    set_color(diffuse1,diffuse1,diffuse1,shininess);
    glTranslatef(x-.80,1.0+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0.598039,0.103333,0.103333,1);
    set_color(diffuse1,diffuse1,diffuse1,shininess);
    glTranslatef(x-.40,1.0+y,z);
    //cout<<y<<endl;
    glScalef(0.10,1.0,0.10);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(0.698039,0.133333,0.133333,1);
    set_color(diffuse1,diffuse1,diffuse1,shininess);
    glTranslatef(x-.60,1.55+y,z+0.20);
    //cout<<y<<endl;
    glRotated(60,1,0,0);
    glScalef(0.50,.50,0.10);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    //glColor4f(0.698039,0.133333,0.133333,1);
    set_color(diffuse1,diffuse1,diffuse1,shininess);
    glTranslatef(x-.20,.75+y,z);
    //cout<<y<<endl;
    glRotated(-60,0,1,0);
    glScalef(0.30,.65,0.10);
    glutSolidCube(1);
    glPopMatrix();


}



void draw_alm(float x, float y, float z)
{

    glPushMatrix();
    //glColor4f(0.80,0.80,1,1);
    GLfloat diffuse7[]    = { 1,1,1,1 };
    set_color(ambient,diffuse7,specular,shininess);
    glTranslatef(x+1.75,1.50+y,z-2);
    //cout<<x<<endl;
    glScalef(0.50,3,2);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.270588,0,1);
    GLfloat diffuse8[]    = {1,1,1,1 };
    set_color(ambient,diffuse8,specular,shininess);
    glTranslatef(x+1.50,1.5+y,z-2.50);
    //cout<<x<<endl;
    glScalef(0.10,3,.85);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    //glColor4f(1,0.270588,0,1);
    set_color(ambient,diffuse8,specular,shininess);
    glTranslatef(x+1.50,1.5+y,z-1.50);
    //cout<<x<<endl;
    glScalef(0.10,3,.85);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //glutSolidCube(1);
    glPopMatrix();
}

void ball(float x, float y, float radius)
{
    int i;
    int triangleAmount = 20;

    float twicePi = 2.0f * 3.1416;

    glBegin(GL_TRIANGLE_FAN);
    GLfloat diffuse5[]= {1.0,0.0,0.0};
    set_color(diffuse5,diffuse5,diffuse5,shininess);
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

    glRotatef(-90,0,1,0);
    draw_alm(5,0,0);

    glTranslatef(0.0,0.0,1.0);
    draw_table(2,0,-2);

    draw_shahidminar(4,0.80,-2.0);



    glTranslatef(1.40,-.25,-2.05);
    ball(2,2.15,.25);

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

    gluLookAt( lx,ly,lz,ex,ey,ez,0,1,0);

    set_light(0,light1_on,light0_ambient,light0_diffuse,light0_specular,light0_position);
    set_light(1,light2_on,light1_ambient,light1_diffuse,light1_specular,light1_position);
    set_light(2,light3_on,light2_ambient,light2_diffuse,light2_specular,light2_position);
    glViewport(0,0,800,800);
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

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glGenTextures(10,Textures);

    Image_Data* Bitmap_floor = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\floor.bmp", Bitmap_floor) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_roof = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\roof.bmp", Bitmap_roof) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_wall = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\wall.bmp", Bitmap_wall) == 0)
    {
        return 0;
    }

        Image_Data* Bitmap_wall1 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\wall1.bmp", Bitmap_wall1) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_alm = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\alm.bmp", Bitmap_alm) == 0)
    {
        return 0;
    }


    Image_Data* Bitmap_almd = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\almd.bmp", Bitmap_almd) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_sofa = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\sofa.bmp", Bitmap_sofa) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_sofao = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\sofao.bmp", Bitmap_sofao) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_sofah = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\sofah.bmp", Bitmap_sofah) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_table = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\table.bmp", Bitmap_table) == 0)
    {
        return 0;
    }

    Add_Texture(0,Bitmap_floor);
    Add_Texture(1,Bitmap_roof);
    Add_Texture(2,Bitmap_wall);
    Add_Texture(3,Bitmap_alm);
    Add_Texture(4,Bitmap_almd);
    Add_Texture(5,Bitmap_sofa);
    Add_Texture(6,Bitmap_sofao);
    Add_Texture(7,Bitmap_sofah);
    Add_Texture(8,Bitmap_table);
     Add_Texture(9,Bitmap_wall1);
    glutMainLoop();

    return 0;
}
