#include <stdio.h>
#include <stdlib.h>
#include "libraries/glut.h"
#include <math.h>
#define PI 3.14159265358

float px, py, pdx, pdy, pa;//player position, d = delta

void drawPlayer()
{
	glColor3f(255, 255, 0); //sets a colour to the player
	glPointSize(8);			//sets the size of each point
	glBegin(GL_POINTS);		//specifies what object is displayed and starts primative render
	glVertex2i(px,py);		//places player at a specific point on screen
	glEnd();				//closes the primative renderer
	
	//tells player direction
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2i(px,py);
	glVertex2i(px+pdx*5, py+pdy*5);
	glEnd();
}

int mapx = 8, mapy = 8, maps = 64;//8 x 8 map size with 64 boxes per unit
int map[] =
{
 1,1,1,1,1,1,1,1,
 1,0,0,0,0,0,0,1,
 1,0,1,0,1,0,0,1,
 1,0,0,0,1,1,0,1,
 1,0,1,0,0,0,0,1,
 1,0,1,1,0,0,0,1,
 1,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1,
};

void drawMap2d()
{
	int x, y, xo, yo;
	for(x=0; x<mapx; x++)
	{
		for(y=0; y<mapy; y++)
		{
			if(map[y*mapx+x]==1){glColor3f(1,1,1);} else{glColor3f(0,0,0);}
			xo=x*maps; yo=y*maps;
			glBegin(GL_QUADS);
			glVertex2i(xo	  +1,yo		+1);
			glVertex2i(xo	  +1,yo+maps-1);
			glVertex2i(xo+maps-1,yo+maps-1);
			glVertex2i(xo+maps-1,yo		+1);
			glEnd();
		}
	}
}

void display()
{
 glClear(GL_COLOR_BUFFER_BIT);//clears colour buffer
 drawMap2d(); //draws map
 drawPlayer(); //displays the player
 glutSwapBuffers();//swaps back and front buffers
}

void buttons(unsigned char key, int x, int y)
{
	if(key=='a'){pa-=0.1; if(pa<   0){pa+=2*PI;}pdx=cos(pa)*5; pdy=sin(pa)*5;}//checks if a was pressed and rotates left
	if(key=='d'){pa+=0.1; if(pa>2*PI){pa-=2*PI;}pdx=cos(pa)*5; pdy=sin(pa)*5;}//checks if d was pressed and rotates right
	if(key=='w'){px+=pdx; py+=pdy;}//checks if w was pressed and goes forwards
	if(key=='s'){px-=pdx; py-=pdy;}//checks if s was pressed and goes backwards
	glutPostRedisplay();//redraws screen
}

void init()
{
	glClearColor(0.3, 0.3, 0.3, 0); //sets window background colour to gray
	gluOrtho2D(0, 1024, 512, 0); //sets window dimentions to 1024 x 512px
	px=300; py=300;pdx=cos(pa)*5; pdy=sin(pa)*5;
}

int main(int argc, char** argv)
{
 glutInit(&argc, argv); //initialises the GLUT library
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);//sets how window will render
 glutInitWindowSize(1024,512); //sets window size
 glutCreateWindow("Raycaster Test");//creates window and names it
 init(); //runs int() Function
 glutDisplayFunc(display);//registers display callback
 glutKeyboardFunc(buttons);//allows keyboard input
 glutMainLoop();//starts GLUT events loop
}
