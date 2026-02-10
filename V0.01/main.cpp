#include <stdio.h>
#include <stdlib.h>
#include "libraries/glut.h"
#include <math.h>
#define PI 3.14159265358
#define RAD 0.0174533 //1 degree in radians

float px, py, pdx, pdy, pa;//player position, d = delta

//------------------Map------------------

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
//------------------------------------------



//------------------Player------------------

void drawRays()
{
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo;
    // ray angle = player angle
   	ra = -pa-RAD*30;

	//drawing multiple rays
	if(ra<0){ra+=2*PI;}
	if(ra>2*PI){ra-=2*PI;}
	//---------------------

    float epsilon = 0.00001f;
    
    for(r=0; r<60; r++)
	{
    	
		
	    if (ra < 0)      ra += 2 * PI;
	    if (ra > 2 * PI) ra -= 2 * PI;
	
	    float hDist = 1e9, hx = px, hy = py;
	    float vDist = 1e9, vx = px, vy = py;

	    // ---------------- HORIZONTAL ----------------
	    dof = 0;
	    float aTan = -1 / (tan(ra) + epsilon);
	
	    if (fabs(-sin(ra)) < epsilon)
	    {
	        hDist = 1e9;
	    }
	    else if (-sin(ra) > 0) // looking down (screen space)
	    {
	        ry = (((int)py >> 6) << 6) + 64;
	        rx = px + (ry - py) * aTan;
	        yo = 64;
	        xo = yo * aTan;
	    }
	    else // looking up
	    {
	        ry = (((int)py >> 6) << 6) - 0.0001;
	        rx = px + (ry - py) * aTan;
	        yo = -64;
	        xo = yo * aTan;
	    }
	
	    while (dof < 8 && fabs(-sin(ra)) >= epsilon)
	    {
	        mx = (int)(rx) >> 6;
   	    	my = (int)(ry) >> 6;
	        mp = my * mapx + mx;

	        if (mp >= 0 && mp < mapx * mapy && map[mp] == 1)
	        {
	            hx = rx;
	            hy = ry;
	            hDist = sqrt((hx - px) * (hx - px) + (hy - py) * (hy - py));
	            break;
	        }
	        else
	        {
	            rx += xo;
	            ry += yo;
	            dof++;
	        }
	    }
	
	    // ---------------- VERTICAL ----------------
	    dof = 0;
	    float nTan = -tan(ra + epsilon);
	
	    if (fabs(cos(ra)) < epsilon)
	    {
	        vDist = 1e9;
	    }
	    else if (cos(ra) > 0) // looking right
	    {
	        rx = (((int)px >> 6) << 6) + 64;
	        ry = py + (rx - px) * nTan;
	        xo = 64;
	        yo = xo * nTan;
	    }
	    else // looking left
	    {
	        rx = (((int)px >> 6) << 6) - 0.0001;
	        ry = py + (rx - px) * nTan;
	        xo = -64;
	        yo = xo * nTan;
	    }
	
	    while (dof < 8 && fabs(cos(ra)) >= epsilon)
	    {
	        mx = (int)(rx) >> 6;
	        my = (int)(ry) >> 6;
	        mp = my * mapx + mx;
	
	        if (mp >= 0 && mp < mapx * mapy && map[mp] == 1)
	        {
	            vx = rx;
	            vy = ry;
	            vDist = sqrt((vx - px) * (vx - px) + (vy - py) * (vy - py));
            	break;
	        	}
	        else
	        {
	            rx += xo;
	            ry += yo;
	            dof++;
	        }
	    }
	
	    // choose closest hit
	    if (vDist < hDist)
	    {
	        rx = vx;
	        ry = vy;
	    }
	    else
	    {
	        rx = hx;
	        ry = hy;
	    }
	
	    // draw ray
	    glColor3f(1, 0, 0);
	    glLineWidth(2);
	    glBegin(GL_LINES);
	    glVertex2i(px, py);
	    glVertex2i(rx, ry);
	    glEnd();
	    // ray angle INCREASES
	   	ra += RAD;
		if(ra<0){ra+=2*PI;}
		if(ra>2*PI){ra-=2*PI;}
		//---------------------
	}
}


void turningKeys(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT)//checks if a was pressed and rotates left
	{
		pa-=0.1; 
		if(pa<   0)
		{
			pa+=2*PI;
		}
		pdx=cos(pa)*5; 
		pdy=sin(pa)*5;
	}
	
	if(key==GLUT_KEY_RIGHT)//checks if d was pressed and rotates right
	{
		pa+=0.1; 
		if(pa>2*PI)
		{
			pa-=2*PI;
		}
		pdx=cos(pa)*5; 
		pdy=sin(pa)*5;
	}
	
	glutPostRedisplay();
	
}

void buttons(unsigned char key, int x, int y)
{
	if(key=='w'){px+=pdx; py+=pdy;}//checks if w was pressed and goes forwards
	if(key=='s'){px-=pdx; py-=pdy;}//checks if s was pressed and goes backwards
	if(key=='a'){py-=pdx; px+=pdy;}//checks if w was pressed and goes left
	if(key=='d'){py+=pdx; px-=pdy;}//checks if s was pressed and goes right
	glutPostRedisplay();//redraws screen
}

void drawPlayer()
{
	glColor3f(1, 1, 0); //sets a colour to the player
	glPointSize(8);			//sets the size of each point
	glBegin(GL_POINTS);		//specifies what object is displayed and starts primative render
	glVertex2i(px,py);		//places player at a specific point on screen
	glEnd();				//closes the primative renderer
	
	/*tells player direction*/
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2i(px,py);
	glVertex2i(px+pdx*5, py+pdy*5);
	glEnd();
}
//------------------------------------------



//---------------main progam/s--------------
void display()
{
 glClear(GL_COLOR_BUFFER_BIT);//clears colour buffer
 drawMap2d(); //draws map
 drawRays(); // draws rays
 drawPlayer(); //displays the player
 glutSwapBuffers();//swaps back and front buffers
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
 glutSpecialFunc(turningKeys);
 glutMainLoop();//starts GLUT events loop
}
