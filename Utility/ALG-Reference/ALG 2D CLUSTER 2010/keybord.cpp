#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <iostream>

#include "functions.h"
extern bool zoomIn;
extern bool zoomOut;
extern bool drawGrid;
extern bool drawHilbertCurve;
extern bool drawBlackPoints;
extern bool drawWhitePoints;
extern bool drawLineOfGroups;
extern bool printSuperGroups;
extern bool printNumberOfSuperGroups;
extern bool drawBlackCurve;
extern float scale;
extern float mouseSensibility;
extern float pointSize;

/*______________________________________________________________________________
________________________________________________________________________________

                          Control through keyboard

Para dar mais zoom:   = ou +
Para das menos zoom:  _ ou -
Para pôr ou retirar o desenho da curva de hilbert: h ou H 
Para pôr ou retirar o desenho do grid: g ou G
Para pôr ou retirar o desenho dos pontos de ocorrencia: o ou O
Para pôr ou retirar o desenho das linhas que formam grupos (clusters): c ou C 
Para sair do programa: ESC

________________________________________________________________________________
______________________________________________________________________________*/
void keyboard( unsigned char key, int x, int y )
{
	switch(key)
	{
		// Amplification
		case '=':
		case '+':
		{
             zoomIn = true;
             zoomOut = ~zoomIn;
			 scale *= 1.1;
			 mouseSensibility *= 0.95;
		 	 pointSize /= 0.95;
			 break;
		}

		// shrink
		case '-':
		case '_':
		{
             zoomOut = true;
             zoomIn = ~zoomOut;
		     scale /= 1.1;
		     mouseSensibility /= 0.95;
		  	 pointSize *= 0.95;
			 break;
		}

		// Draws Hilbert's curve, if it is not drawn, or erases it if it has
		// been painted.
		case 'h':
		case 'H':
		{
			drawHilbertCurve = !drawHilbertCurve;
			break;
		}

        // Draws grid, if it is not drawn, or erases it if it has
		// been painted.
		case 'g':
		case 'G':
		{
			drawGrid = !drawGrid;
			break;
		}
		
		// Draws BlackPoints of occurrence, if they are not drawn, or erases them if 
        // they have been painted.
		case 'b':
		case 'B':
		{
			drawBlackPoints = !drawBlackPoints;
			break;
		}
		
        // Draws WhitePoints of occurrence, if they are not drawn, or erases them if 
        // they have been painted.
		case 'w':
		case 'W':
		{
			drawWhitePoints = !drawWhitePoints;
			break;
		}

        // Draws lines that conect cells of the same group, if they are not drawn,
        // or erases them if they have been painted.
        case 'l':
        case 'L':
        {
             drawLineOfGroups = !drawLineOfGroups;
             break;
        }

        case 'c':
        case 'C':
        {
             printSuperGroups = !printSuperGroups;
             break;
        }
        
        case 'n':
        case 'N':
        {
             printNumberOfSuperGroups = !printNumberOfSuperGroups;
             break;
        }
        
        case '2':
        {
             drawBlackCurve = !drawBlackCurve;
             break;
        }

		// Finishes the program when the key ESC is pushed.
		case 27:
		{
			exit (0);
			break;
		}

		default:
		{
			break;
		}
	}

	glutPostRedisplay();
}
