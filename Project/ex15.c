/*Final Project */
#include "CSCIx229.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415927

double dim = 3.0; //  Size of world
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double asp=1;     //  Aspect ratio
const float len=2.0; //Length of axis

//test variables
int used = 0;

int light=1;      //  Lighting
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   6;  // Elevation of light

float xpos = 1, ypos = 1.2, zpos = 5;
float xrot = 0;
float yrot = 0;

unsigned int texture[3];  //  Texture names
int texMap[4][2] = {{0,0}, {1, 0}, {1, 1}, {0, 1}};

//Min/Max xyz for each collidable poly manually incremented
float ColMax[14][3]; 
float ColMin[14][3]; 

//AABB zones to use objects
double objMax[3][2];//Different types to avoid dumbass bug where setting value in one array would affect entry in another for no reason
float objMin[3][2];
//List of toggle state of objects
int objectUsed[3] = {0};

//Usable objects
//use1 vs use2 is object min vs max, different types to match use for objMin/objMax, see above.
float d1_use1[2] = {-1.5,.5};
double d1_use2[2] = {-0.5,3.5};

float d2_use1[2] = {1,-6.5};
double d2_use2[2] = {3,-4.5};

float b1_use1[2] = {-2.4, -8};
double b1_use2[2] = {-1, -5.5};
//Polygon Arrays
//Collidables: w for walls, d for doors, e for empty
float e1[4][3] = {0};//Used for removed collidables

float d1[4][3] = {
	{-.5,.1,2},
	{-1.5,.1,2},
	{-1.5,2,2},
	{-.5,2,2}
};
//first wall right of door
float w1[4][3] = {
	{0,0,2},
	{0,2,2},
	{0,2,-1},
	{0,0,-1}
};
//second wall right of door
float w2[4][3] = {
	{0,0,-1},
	{2,0,-1},
	{2,2,-1},
	{0,2,-1}
};
//panel right of door
float w3[4][3] = {
	{0,0,2},
	{-.5,0,2},
	{-.5,2,2},
	{0,2,2}
};
//panel left of door
float w4[4][3] = {
	{-1.5,0,2},
	{-2,0,2},
	{-2,2,2},
	{-1.5,2,2}
};
//first wall left of door
float w5[4][3] = {
	{-2,0,2},
	{-2,2,2},
	{-2,2,-1},
	{-2,0,-1}
};
//second wall left of door
float w6[4][3] = {
	{-2,0,-1},
	{-4,0,-1},
	{-4,2,-1},
	{-2,2,-1}
};
//left long wall
float w7[4][3] = {
	{-4,0,-1},
	{-4,2,-1},
	{-4,2,-4.5},
	{-4,0,-4.5}
};
//right long wall
float w8[4][3] = {
	{2,0,-1},
	{2,2,-1},
	{2,2,-5},
	{2,0,-5}
};
//right wall pt 2.
float w11[4][3] = {
	{-4,0,-6},
	{-4,2,-6},
	{-4,2,-8},
	{-4,0,-8}
};
//right wall bottom
float w12[4][3] = {
	{-4,0,-4.5},
	{-4,.5,-4.5},
	{-4,.5,-6},
	{-4,0,-6}
};
//right wall top
float w13[4][3] = {
	{-4,1.5,-4.5},
	{-4,2,-4.5},
	{-4,2,-6},
	{-4,1.5,-6}
};
//horizontal bar
float w14[4][3] = {
	{-4,1.05,-4.5},
	{-4,.95,-4.5},
	{-4,.95,-6},
	{-4,1.05,-6}
};
float w15[4][3] = {
	{-4,1.5,-5.30},
	{-4,1.05,-5.30},
	{-4,1.05,-5.2},
	{-4,1.5,-5.2}
};
float w16[4][3] = {
	{-4,.95,-5.30},
	{-4,.5,-5.30},
	{-4,.5,-5.2},
	{-4,.95,-5.2}
};
float d2[4][3] = {
	{2,0,-5},
	{2,2,-5},
	{2,2,-6},
	{2,0,-6}
};
//right long wall pt2
float w9[4][3] = {
	{2,0,-6},
	{2,2,-6},
	{2,2,-8},
	{2,0,-8}
};
//back wall
float w10[5][3] = {
	{2,0,-8},
	{2,2,-8},
	{-1,4.5,-8},
	{-4,2,-8},
	{-4,0,-8}
};
//NonCollidables r for roof, f for floor, b for bed
//top of matress
float b1[4][3] = {
	{-3.7,0.65,-7.7},
	{-2.5,0.65,-7.7},
	{-2.5,0.65,-5.7},
	{-3.7,0.65,-5.7}
};
//matress left
float b2[4][3] = {
	{-3.7,0.65,-7.7},
	{-3.7,0.35,-7.7},
	{-3.7,0.35,-5.7},
	{-3.7,0.65,-5.7},
};
//mattress back
float b3[5][3] = {
	{-3.7,0.65,-7.7},
	{-3.7,0.35,-7.7},
	{-2.4,0.35,-7.7},
	{-2.4,0.55,-7.7},
	{-2.5,0.65,-7.7}
};
//mattress round edge
float b4[4][3] = {
	{-2.5,0.65,-7.7},
	{-2.4,0.55,-7.7},
	{-2.4,0.55,-5.7},
	{-2.5,0.65,-5.7}
};
//mattress right
float b5 [4][3] = {
	{-2.4,0.55,-5.7},
	{-2.4,0.35,-5.7},
	{-2.4,0.35,-7.7},
	{-2.4,0.55,-7.7}
};
//matress front
float b6[5][3] = {
	{-2.4,0.35,-5.7},
	{-2.4,0.55,-5.7},
	{-2.5,0.65,-5.7},
	{-3.7,0.65,-5.7},
	{-3.7,0.35,-5.7}
};
//indoor floor
float f1[4][3] = {
	{0,.1,2},
	{0,.1,-1},
	{-2,.1,-1},
	{-2,.1,2}
};
float f2[4][3] = {
	{2,.1,-1},
	{2,.1,-8},
	{-4,.1,-8},
	{-4,.1,-1}
};
float f3[4][3] = {
	{-.5,0,2},
	{-.5,.1,2},
	{-1.5,.1,2},
	{-1.5,0,2}
};
//outdoor ground
float f4[4][3] = {
	{-15,0,-15},
	{-15,0,15},
	{15,0,15},
	{15,0,-15}
};
//front door triangle
float r1[3][3] = {
	{0,2,2},
	{-2,2,2},
	{-1,3,2}
};
//small roof right
float r2[4][3] = {
	{0,2,-1},
	{0,2,2},
	{-1,3,2},
	{-1,3,-1}
};
//small roof left
float r3[4][3] = {
	{-1,3,2},
	{-1,3,-1},
	{-2,2,-1},
	{-2,2,2}
};
//front large triangle
float r4[3][3] = {
	{-4,2,-1},
	{2,2,-1},
	{-1,4.5,-1}
};
//big roof right
float r5[4][3] = {
	{2,2,-.5},
	{2,2,-8.5},
	{-1,4.5,-8.5},
	{-1,4.5,-.5}
};
//big roof left
float r6[4][3] = {
	{-1,4.5,-.5},
	{-1,4.5,-8.5},
	{-4,2,-8.5},
	{-4,2,-.5}
};
void setObj(float min[2], double max[2], int pos){
	for (int i = 0; i < 2; i++){
		objMin[pos][i] = min[i];
		objMax[pos][i] = max[i];
	}
}

int canUseObj(){
	int iObj = sizeof(objMin) / sizeof(objMin[0]);
	for (int i = 0; i <= iObj; i++){
		if ((xpos >= objMin[i][0] && xpos <= objMax[i][0]) &&
		(zpos >= objMin[i][1] && zpos <= objMax[i][1])){
			return i;
		}
	}
	return -1;
}

void collidable(int v, float verts[v][3], int pos){ //v is number of verticies, pos is point of insertion
	float min[3] = {verts[0][0], verts[0][1], verts[0][2]};
	float max[3] = {verts[0][0], verts[0][1], verts[0][2]};
	
	for (int i = 0; i < v; i++){ //iterates through verticies
		for (int q = 0; q <= 2; q++){ //iterates through xyz
			if (verts[i][q] < min[q]){
				min[q] = verts[i][q];
			}else if (verts[i][q] > max[q]){
				max[q] = verts[i][q];
			}
		}
	}
		
	//Set AABB in ColMax, ColMin
	for (int a = 0; a <= 2; a++){
		ColMax[pos][a] = max[a];
		ColMin[pos][a] = min[a];
	}
	
	//If AABB is flat, add width greater than max movable distance
	if (ColMax[pos][0] == ColMin[pos][0]){ 
		ColMax[pos][0] += 0.4;
		ColMin[pos][0] -= 0.4;}
	if (ColMax[pos][2] == ColMin[pos][2]){
		ColMax[pos][2] += 0.4;
		ColMin[pos][2] -= 0.4;}	
}

void objCase(int obj){
	objectUsed[obj] = !(objectUsed[obj]);
	if (obj>=0 && obj<=1){
		if (objectUsed[obj])
			collidable(4,e1,obj);
		else if(obj == 0)
			collidable(4,d1,obj);
		else
			collidable(4,d2,obj);
	}else if(obj == 2){
		if (objectUsed[obj]){
			xpos = -3.0;
			zpos = -6.0;
			ypos = .75;
		}else{
			xpos = -2.0;
			ypos = 1.2;
		}
	}
}

int isInsideAABB(float xPoint, float zPoint){
	int cPoly = sizeof(ColMax) / sizeof(ColMax[0]);
	for (int i = 0; i < cPoly; i++){
		if ((xPoint >= ColMin[i][0] && xPoint <= ColMax[i][0]) &&
		(zPoint >= ColMin[i][2] && zPoint <= ColMax[i][2]))
			return 1;
	}
	return 0;
}

void drawPoly(int v, float verts[v][3]){ //v is number of verticies
	int prev;
	int next;
	float vector1[3];
	float vector2[3];
	glBegin(GL_POLYGON);
	
	for (int i = 0; i < v; i++){
		prev = i-1;
		next = i+1;
		if (i == 0){
			prev = v-1;
		}else if (i == v-1)
		    next = 0;
			
		//Get vectors from vertices
		vector1[0] = verts[prev][0]-verts[i][0];
		vector1[1] = verts[prev][1]-verts[i][1];
		vector1[2] = verts[prev][2]-verts[i][2];
		vector2[0] = verts[next][0]-verts[i][0];
		vector2[1] = verts[next][1]-verts[i][1]; 
		vector2[2] = verts[next][2]-verts[i][2];
		//calculate normal from cross product of vectors
		glNormal3f(-((vector1[1]*vector2[2])-(vector1[2]*vector2[1])), -((vector1[2]*vector2[0])-(vector1[0]*vector2[2])), verts[i][2]);
		if (i < 4){
			glTexCoord2f(texMap[i][0], texMap[i][1]);}
		//Draw verticies
		glVertex3f(verts[i][0],verts[i][1],verts[i][2]);
		glDisable(GL_TEXTURE_2D);
	} 
	glEnd();
}

static void ball(float x,float y,float z,float r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

void draw_cylinder(float radius, float height)
{
    float x              = 0.0;
    float y              = 0.0;
    float angle          = 0.0;
    float angle_stepsize = 0.1;

    /** Draw the tube */
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */

    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}

void draw_branch(){
   glPushMatrix();
   ball(0,0,1,.05);
   draw_cylinder(.05, 1);
   glTranslatef(0,0,.75);
   glRotatef(-80, 1, 1, 1);
   draw_cylinder(.05, 1);
   glRotatef(80, 1, 1, 1);
   glTranslatef(0,0,.25);
   glRotatef(80, 1, 1, 1);
   draw_cylinder(.05, 1);
   glTranslatef(0,0,.25);
   glRotatef(-30, 1, 1, 1);
   draw_cylinder(.05, 1);
   glPopMatrix(); 
}

void draw_tree(float x, float z){
   glPushMatrix();
   glTranslatef(x,0,z);
   glRotatef(-90, 1.0, 0.0, 0.0);
   ball(0,0,1.8,.12);
   draw_cylinder(.12, 1.8);
   glTranslatef(0,0,1.8);
   glRotatef(-80, 1, 1, 1);
   draw_branch();
   glRotatef(160, 1, 1, 1);
   draw_branch();
   glRotatef(-110, 0, 1, 1);
   draw_branch();
   glPopMatrix();
}

void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the  x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_FOG);
   glClearColor(0.5,0.5,0.5,1);
   //  Set perspective
   glLoadIdentity();
   
   GLfloat fogColor[] = {0.5,0.5,0.5,1};
   glFogfv(GL_FOG_COLOR, fogColor);
   glFogi(GL_FOG_MODE, GL_LINEAR);
   glFogf(GL_FOG_START, 6.5f);
   glFogf(GL_FOG_END, 15.0f);

   camera();
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   //Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //Draw Polygons
   drawPoly(4,w1);//Collidable
   drawPoly(4,w2);
   drawPoly(4,w3);
   drawPoly(4,w4);
   drawPoly(4,w5);
   drawPoly(4,w6);
   drawPoly(4,w7);
   drawPoly(4,w8);
   drawPoly(3,r1);
   drawPoly(4,r2);
   drawPoly(4,r3);
   drawPoly(3,r4);
   drawPoly(4,r5);
   drawPoly(4,r6);
   drawPoly(4,w9);
   drawPoly(5,w10);
   drawPoly(4,w11);
   drawPoly(4,w12);
   drawPoly(4,w13);
   drawPoly(4,w14);
   drawPoly(4,w15);
   drawPoly(4,w16);
   drawPoly(4,f1);
   drawPoly(4,f2);
   drawPoly(4,f3);
   drawPoly(4,b1);
   drawPoly(4,b2);
   drawPoly(5,b3);
   drawPoly(4,b4);
   drawPoly(4,b5);
   drawPoly(5,b6);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   drawPoly(4,f4);
   //door texture
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   if (!objectUsed[0])
      drawPoly(4,d1);
   if (!objectUsed[1])
      drawPoly(4,d2);
   draw_tree(4,4);
   draw_tree(-4,3);
   draw_tree(-7,0);
   draw_tree(3.5,-1);
   draw_tree(1,6);
   //  Draw axes - no lighting or textures from here on
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   
   glLoadIdentity();
   glBegin(GL_LINES);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(len,0.0,0.0);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(0.0,len,0.0);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(0.0,0.0,len);
   glEnd();
   //  Label axes
   glRasterPos3d(len,0.0,0.0);
   Print("X");
   glRasterPos3d(0.0,len,0.0);
   Print("Y");
   glRasterPos3d(0.0,0.0,len);
   Print("Z");
   
   //glWindowPos2i(5,25);
   //Print("ColMinMax=%i", objectUsed[1]);
   //glWindowPos2i(5,5);
   //Print("ColMinMax=%f,%f,%f,%f", ColMin[10][0], ColMin[10][1], ColMax[10][0], ColMax[10][1]);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   float t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   else if (ch=='f')
	{
		int useObj = canUseObj();
		if (useObj != -1){
			objCase(useObj);
		}
	}
   else if (ch=='q')
    {
		xrot += .5;
		if (xrot >360) xrot -= 360;
    }

    else if (ch=='z')
    {
		xrot -= .5;
		if (xrot < -360) xrot += 360;
    }

    else if (ch=='w')
    {
		float yrotrad;
		float xposTemp = xpos;
		float zposTemp = zpos;
		
		yrotrad = (yrot / 180 * 3.141592654f);
		//xrotrad = (xrot / 180 * 3.141592654f); 
		xpos += 0.1*(sin(yrotrad)) ;
		zpos -= 0.1*(cos(yrotrad)) ;
		//If move causes collision, revert it
		if (isInsideAABB(xpos, zpos) || objectUsed[2]){
			xpos = xposTemp;
			zpos = zposTemp;
		}
    }

    else if (ch=='s')
    {
		float yrotrad;
		float xposTemp = xpos;
		float zposTemp = zpos;
		
		yrotrad = (yrot / 180 * 3.141592654f);
		//xrotrad = (xrot / 180 * 3.141592654f); 
		xpos -= 0.1*(sin(yrotrad));
		zpos += 0.1*(cos(yrotrad)) ;
		//If move causes collision, revert it
		if (isInsideAABB(xpos, zpos) || objectUsed[2]){
			xpos = xposTemp;
			zpos = zposTemp;
		}
    }

    else if (ch=='d')
    {
		yrot += 1.5;
		if (yrot >360) yrot -= 360;
    }

    else if (ch=='a')
    {
		yrot -= 1.5;
		if (yrot < -360)yrot += 360;
    }
   //  Reproject
   Project(45,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(45,asp,dim);
}


/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Final Project");
   
   //Set Collidable surfaces
   collidable(4,d1,0);
   collidable(4,d2,1);
   collidable(4,w1,2);
   collidable(4,w2,3);
   collidable(4,w3,4);
   collidable(4,w4,5);
   collidable(4,w5,6);
   collidable(4,w6,7);
   collidable(4,w7,8);
   collidable(4,w8,9);
   collidable(4,w9,10);
   collidable(5,w10,11);
   collidable(4,w11,12);
   collidable(4,w12,13);
   //Set Object use zones
   setObj(d1_use1, d1_use2, 0);
   setObj(d2_use1, d2_use2, 1);
   setObj(b1_use1, b1_use2, 2);
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   
   //Load Textures
   texture[0] = LoadTexBMP("wood.bmp");
   texture[1] = LoadTexBMP("crate.bmp");
   texture[2] = LoadTexBMP("img4.bmp");
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
