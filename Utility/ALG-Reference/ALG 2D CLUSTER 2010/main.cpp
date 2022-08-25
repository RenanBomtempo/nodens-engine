#include <GL/glut.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;
#include <math.h>    
#include "functions.h"
#include "Graph.h"
#include "auxiliarMain.h"


// Window measurements
#define WIN_WIDTH 700
#define WIN_HEIGHT 700

double MAX, SUPERMAX;
    
bool statisticScan;
bool writeCellsNumber;
bool drawHilbertCurve;
bool drawGrid;
bool drawBlackPoints;
bool drawWhitePoints;
bool drawLineOfGroups;
bool printSuperGroups;
bool printNumberOfSuperGroups;
bool drawBlackCurve;
float zoomIn;
float zoomOut;
float scale;
float deltaX;
float deltaY;
float pointSize;
float mouseSensibility;

/*  DECLARACAO DAS ESTRUTURAS USADAS 
   
   grid: cria dinamicamente uma variavel do tipo Grid
   
   Table blackTable & whiteTable: BlackTable contem as coordenadas dos pontos
   "criticos", que correspondem à populacao com alguma anomalia (se aparecerem em
   grupos podem determinar a existencia de um cluster). WhiteTable contem os pontos
   "nao-criticos", que corresponderiam à populacao normal (estes nao interferem
   no refinamento e na formacao de grupos do grid).
   
   Grafo: nele serao armazenados os grupos que pertencem a um mesmo SuperGroup
   
*/
Grid *grid;
Graph *graph;

Table *blackTable;
Table *whiteTable;

void zoom() 
{
   	 if( zoomIn || zoomOut ) {
         glScalef( scale, scale, scale );
	 }
}

void display( void )
{
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D( -100.0, 100.0, -100.0, 100.0 ); //intervalo do sistema de coordenadas
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     zoom();
     glTranslatef( -75,-75, 0 );
     glTranslatef( deltaX, deltaY, 0 );

     draw_DATA( BLACK );

     if( writeCellsNumber )
     	 grid->writeCellsNumber( BLACK );//desenha na tela os numeros das celulas.
     
     if( drawGrid )
     	 grid->drawGrid( BLUE );//desenha na tela o grid azul
     	 
   	 if( drawHilbertCurve )
   	 	 grid->drawHilbertCurve( RED );//desenha na tela a curva de hilbert vermelha

  	 if( drawWhitePoints )
       	 whiteTable->draw( VIOLET, 3.0 );//desenha na tela os pontos roxos
  
  	 if( drawBlackPoints )
       	 blackTable->draw( BLACK, 3.5 );//desenha na tela os pontos pretos

  	 if (drawLineOfGroups)
  	     grid->drawLineOfGroups( YELLOW );//desenha em amarelo na tela as linhas que formam os grupos

  	 if (printNumberOfSuperGroups)
  	     grid->printNumberOfSuperGroups( BLACK );//escreve em preto na tela os numeros dos supergroups das celulas

  	 if (printSuperGroups)
  	     grid->drawSuperGroups( GREEN );//desenha em verde transparente na tela o fundo das celulas que formam os grupos

  	 if (drawBlackCurve)
  	     grid->drawBlackCurve( BLACK );//desenha em preto na tela a BlackCurve.

     if (statisticScan)
     	 graph->writeStatisticScan( RED );//desenha na tela os valores de estatistica scan dos super-grupos.
   
     glutSwapBuffers();//desenha na tela somente quando o desenho ja esta pronto
}

/**
 * Initializes parameters of program.
 */
void init( void )
{ 
    
    srand( time(NULL) );
    
    //_________________________________________________________________________
 	// Initialization of openGL parameters.
	glClearColor( 1.0, 1.0, 1.0, 0); //Sets background color to light-gray.
	glShadeModel( GL_SMOOTH );
	
    //_________________________________________________________________________
 	// Initialization of global variables.
 	scale            = 1.0;
 	deltaX           = 0.0;
 	deltaY           = 0.0;
	pointSize        = 3.0;
 	mouseSensibility = 0.2;
 	statisticScan = false;
 	writeCellsNumber = false;
	drawHilbertCurve = false;
	drawGrid         = true;
	drawBlackPoints  = true;
	drawWhitePoints  = true;
 	drawLineOfGroups = false;
 	printSuperGroups      = true;
 	printNumberOfSuperGroups = false;
 	drawBlackCurve = false;

    simulation();	    

}


/**
 * Main.
 */
int main (int argc, char** argv )
{   
	glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );
    glutInitWindowPosition( 10, 10 );
    glutCreateWindow( "Autonomous Leaves Graph (ALG)  -  Adaptive Mesh Refinement" );

	init();
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc( movingMouse );
    glutMainLoop();
    
   	return(0); 
}
