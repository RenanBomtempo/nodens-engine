#include "graphics.h"
#include "opengl.h"
#include "gl2ps.h"
#include <string>

using std::string;

/*==============================================================================

                            TABLE OF COLORS

    Table of colors which is used to set the color of a drawing.
==============================================================================*/
static float tableOfColors[10][3] =  {
									 {0.0,  0.0,  0.0},  // 0 = black
									 {1.0,  0.0,  0.0},  // 1 = red
									 {1.0,  1.0,  0.0},  // 2 = yellow
									 {0.0,  1.0,  0.0},  // 3 = green
									 {0.0,  1.0,  1.0},  // 4 = ligth blue
									 {0.0,  0.0,  1.0},  // 5 = blue
									 {1.0,  0.0,  1.0},  // 6 = violet
									 {1.0,  1.0,  1.0},  // 7 = white
									 {1.0,  0.5,  0.0},  // 8 = orange
									 {0.75, 0.75, 0.75}  // 9 = gray
									};

/*==============================================================================

                           FUNCTION setColor()

    Sets the color used in a drawing according to the table of color defined
    previously.
==============================================================================*/
void setColor( int color ) {
	glColor3f(tableOfColors[color][0],
			  tableOfColors[color][1],
			  tableOfColors[color][2]);
}

/*==============================================================================

                           FUNCTION setLineWidth()

    Sets the line width of a drawing.
==============================================================================*/
void setLineWidth( float width ) {
	glLineWidth( width );
}

/*==============================================================================

                          FUNCTION drawString()

    Draws a string on screen given its font and the initial position.
==============================================================================*/
void drawString( const char* stringText, void* font, float x0, float y0, float z0 ) {

    string stringFont;
    int fontSize = 0;
    if( font == GLUT_BITMAP_HELVETICA_10 ) {
         stringFont = "Helvetica";
         fontSize = 10;
    }
    else if( font == GLUT_BITMAP_HELVETICA_12 ) {
         stringFont = "Helvetica";
         fontSize = 12;
    }
    else if( font == GLUT_BITMAP_HELVETICA_18 ) {
         stringFont = "Helvetica";
         fontSize = 18;
    }
    else if( font == GLUT_BITMAP_TIMES_ROMAN_10 ) {
         stringFont = "Times-Roman";
         fontSize = 10;
    }
    else if( font == GLUT_BITMAP_TIMES_ROMAN_24 ) {
         stringFont = "Times-Roman";
         fontSize = 24;
    }
    else if( font == GLUT_BITMAP_8_BY_13 ) {
         stringFont = "Helvetica";
         fontSize = 10;
    }
    else if( font == GLUT_BITMAP_9_BY_15 ) {
         stringFont = "Helvetica";
         fontSize = 12;
    }

    //Sets initial position.
	glRasterPos3f( x0, y0, z0 );

    /* call gl2psText before the glut function since glutBitmapCharacter
     * changes the raster position. It has been put here in order to make the
     * function drawString() be able to print strings even on image file, like ps,
     * eps, svg, pdf, and all file format supported by the library gl2ps.
     */
    gl2psText(stringText, stringFont.c_str(), fontSize);

	//Prints each character of string.
	char* character = (char*) stringText;
	while( *character != '\0')
		glutBitmapCharacter( font, *character++ );
}

/*==============================================================================

                          FUNCTION drawPoint()

    Paints a node given its coordinates and size.
==============================================================================*/
void drawPoint( float x, float y, float z, float size ) {
	glPointSize( size );
	glEnable( GL_POINT_SMOOTH );
		glBegin( GL_POINTS );
    	    glVertex3f( x, y, z );
		glEnd ();
	glDisable( GL_POINT_SMOOTH );
}

/*==============================================================================

                               FUNCTION drawLine ()

   Draws  a  line  which  starts  at  the  point (x0, y0, z0)  and  finishes  at
   (x1, y1, z1).
==============================================================================*/
void drawLine( float x0, float y0, float z0, float x1, float y1,  float z1 ) {
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glBegin( GL_LINES );
		glVertex3d( x0, y0, z0 );
		glVertex3d( x1, y1, z1 );
	glEnd();
}

/*==============================================================================

                              FUNCTION drawCube()

   Draws a cube given the coordinates of the right upper corner  of  its  front
   face, and  the left  lower  corner  of  its  back  face, as  shown   in  the
   illustration below.

                                  _____________
                                /|            /|
                              /  |          /  |    P0:  (x0, y0, z0)
                            /____|_______P1    |    P1:  (x1, y1, z1)
                            |    |       |     |
                            |    |       |     |
                            |    |_______|_____|
                            |   /P0      |    /
                            | /          |  /
                            |/___________|/

==============================================================================*/
void drawCube( float x0, float y0, float z0, float x1, float y1, float z1 ) {
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	// Front face.
	glBegin(GL_LINE_LOOP);
		glVertex3d( x1, y0, z0 );
		glVertex3d( x1, y1, z0 );
		glVertex3d( x1, y1, z1 );
		glVertex3d( x1, y0, z1 );
	glEnd();

	// Back face.
	glBegin(GL_LINE_LOOP);
		glVertex3d( x0, y0, z0 );
		glVertex3d( x0, y1, z0 );
		glVertex3d( x0, y1, z1 );
		glVertex3d( x0, y0, z1 );
	glEnd();

	// Lines linking correspondent vertexes of the two faces just created.
	glBegin(GL_LINES);
		glVertex3d( x0, y0, z0 );
		glVertex3d( x1, y0, z0 );

		glVertex3d( x0, y0, z1 );
		glVertex3d( x1, y0, z1 );

		glVertex3d( x0, y1, z1 );
		glVertex3d( x1, y1, z1 );

		glVertex3d( x0, y1, z0 );
		glVertex3d( x1, y1, z0 );
	glEnd();
}

/*==============================================================================

                            FUNCTION drawSolidCube()

   Draws a solid cube given the coordinates of the right upper  corner  of  its
   front face, and  the left  lower  corner  of its back  face, as shown in the
   illustration below.

                                  _____________
                                /|            /|
                              /  |          /  |    P0:  (x0, y0, z0)
                            /____|_______P1    |    P1:  (x1, y1, z1)
                            |    |       |     |
                            |    |       |     |
                            |    |_______|_____|
                            |   /P0      |    /
                            | /          |  /
                            |/___________|/

==============================================================================*/
void drawSolidCube( float x0, float y0, float z0, float x1, float y1, float z1 ) {
	// Front face.
	glBegin( GL_POLYGON );
		glNormal3f( 1.0, 0.0, 0.0 );
		glVertex3d( x1, y0, z0 );
		glVertex3d( x1, y1, z0 );
		glVertex3d( x1, y1, z1 );
		glVertex3d( x1, y0, z1 );
		glVertex3d( x1, y0, z0 );
	glEnd();

	// Right face.
	glBegin( GL_POLYGON );
		glNormal3f( 0.0, -1.0, 0.0 );
		glVertex3d( x0, y0, z0 );
		glVertex3d( x1, y0, z0 );
		glVertex3d( x1, y0, z1 );
		glVertex3d( x0, y0, z1 );
		glVertex3d( x0, y0, z0 );
	glEnd();

	// Down face.
	glBegin( GL_POLYGON );
		glNormal3f( 0.0, 0.0, -1.0 );
		glVertex3d( x0, y0, z0 );
		glVertex3d( x0, y1, z0 );
		glVertex3d( x1, y1, z0 );
		glVertex3d( x1, y0, z0 );
		glVertex3d( x0, y0, z0 );
	glEnd();

	// Back face.
	glBegin( GL_POLYGON );
		glNormal3f( -1.0, 0.0, 0.0 );
		glVertex3d( x0, y0, z0 );
		glVertex3d( x0, y0, z1 );
		glVertex3d( x0, y1, z1 );
		glVertex3d( x0, y1, z0 );
		glVertex3d( x0, y0, z0 );
	glEnd();

	// Left face.
	glBegin( GL_POLYGON );
		glNormal3f( 0.0, 1.0, 0.0 );
		glVertex3d( x0, y1, z0 );
		glVertex3d( x0, y1, z1 );
		glVertex3d( x1, y1, z1 );
		glVertex3d( x1, y1, z0 );
		glVertex3d( x0, y1, z0 );
	glEnd();

	// Top face.
	glBegin( GL_POLYGON );
		glNormal3f( 0.0, 0.0, 1.0 );
		glVertex3d( x0, y0, z1 );
		glVertex3d( x1, y0, z1 );
		glVertex3d( x1, y1, z1 );
		glVertex3d( x0, y1, z1 );
		glVertex3d( x0, y0, z1 );
	glEnd();
}

/*==============================================================================

                            FUNCTION enableLighting

    Enables lighting.
==============================================================================*/
void enableLighting() {
	 glEnable( GL_LIGHTING );
}

/*==============================================================================

                            FUNCTION disableLighting

    Disables lighting.
==============================================================================*/
void disableLighting() {
	 glDisable( GL_LIGHTING );
}

/*==============================================================================

                          FUNCTION drawSolidSphere()

    Paints a solid node given its coordinates and size.
==============================================================================*/
void drawSphere( float x, float y, float z, float size ) {
	glPushMatrix();
	glTranslatef( x, y, z );
         glutSolidSphere( size/500.0, 8, 8 );
	glPopMatrix();
}

