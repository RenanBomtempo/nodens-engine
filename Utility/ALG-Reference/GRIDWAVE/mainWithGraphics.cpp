//main.cpp

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
 
#include "Grid.h"
#include "functions.h"
         
void myInit(void)
{
    glClearColor (1.0, 1.0, 1.0, 1.0); //set white background color
	glClear(GL_COLOR_BUFFER_BIT);      //clear the screen
	glMatrixMode (GL_PROJECTION);      //set coordinate system
	glLoadIdentity();
	gluOrtho2D (0.0, 400.0, 0.0, 400.0);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);      //clear the screen
	
	Grid *grid;
    grid = new Grid;                                    
    grid->solveWave( true );
    //grid->print();
    cout << "End of Program." << "\n\n";
    glFlush();                      //send all output to display
}     

int main(int argc, char** argv)
{  
   glutInit(&argc, argv);    //initialize the toolkit
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); //set display mode
   glutInitWindowSize (800, 800);  //set window size
   glutInitWindowPosition (10, 10); //set window position on screen
   glutCreateWindow ("Malha Adaptativa do Problema da Onda");//open the screen window
   
   myInit();                   
   glutDisplayFunc(myDisplay);   
   glutMainLoop();
	
   return(0); 
}
