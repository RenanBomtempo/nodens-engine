//graphics.cpp

#include <GL/gl.h>
#include <GL/glut.h>

#include "functions.h"

static float tableOfColors[9][4] =  {
									 {0.0, 0.0, 0.0, 1.0},  // 0 = black
									 {1.0, 0.0, 0.0, 1.0},  // 1 = red
									 {1.0, 1.0, 0.0, 1.0},  // 2 = yellow
									 {0.0, 0.7, 0.0, 0.2},  // 3 = green (transparente)
									 {0.0, 1.0, 1.0, 1.0},  // 4 = light blue
									 {0.0, 0.0, 1.0, 1.0},  // 5 = blue
									 {1.0, 0.0, 1.0, 1.0},  // 6 = violet
									 {1.0, 1.0, 1.0, 1.0},  // 7 = white
									 {1.0, 0.5, 0.0, 1.0}   // 8 = orange
									};

void setColor( int color )
{
    glColor4f(tableOfColors[color][0],
              tableOfColors[color][1], 
              tableOfColors[color][2],
              tableOfColors[color][3]);
}

void drawPoint( double x, double y, double size )
{
   glPointSize( size );
   glEnable( GL_POINT_SMOOTH ); //desenha os pontos de forma arredondada, suave
   glBegin(GL_POINTS);
     glVertex2d( FACTOR*x, FACTOR*y );//constante multiplicativa aumenta a escala do desenho
   glEnd();
   glDisable( GL_POINT_SMOOTH );
}

void setLineWidth (float width)
{
     glLineWidth(width);
}

void drawLine( double x0, double y0, double z0, double x1, double y1,  double z1 )
{
    glBegin(GL_LINES);       
        glVertex2d( FACTOR*x0, FACTOR*y0 );
        glVertex2d( FACTOR*x1, FACTOR*y1 );
    glEnd();
}

void drawRectangle( double x0, double y0, double x1, double y1 )
{
    glBegin(GL_LINE_LOOP);
        glVertex2d( FACTOR*x0, FACTOR*y0 );
        glVertex2d( FACTOR*x1, FACTOR*y0 );
        glVertex2d( FACTOR*x1, FACTOR*y1 );
        glVertex2d( FACTOR*x0, FACTOR*y1 );
    glEnd();
}

void printRectangle( double x0, double y0, double x1, double y1 )
{
    glBegin(GL_QUADS);
        glVertex2d( FACTOR*x0, FACTOR*y0 );
        glVertex2d( FACTOR*x1, FACTOR*y0 );
        glVertex2d( FACTOR*x1, FACTOR*y1 );
        glVertex2d( FACTOR*x0, FACTOR*y1 );
    glEnd();
}

void drawString( char* string, void* font, double x0, double y0)
{
	char* charactere = (char*) string;
	
	glRasterPos2f( FACTOR*x0, FACTOR*y0 );
	while( *charactere != '\0')
	{
		glutBitmapCharacter( font, *charactere++ );
	}
}
