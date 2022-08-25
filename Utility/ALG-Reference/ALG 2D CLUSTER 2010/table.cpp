#include <iostream>
#include <GL/glut.h>

#include "table.h"
#include "functions.h"

using namespace std;

/**
 * Constructor
 */
Table::Table( char symbolOfPoints, char typeOfPoints ){
   symbol = symbolOfPoints;
   type = typeOfPoints;
}

/**
 * Destructor
 */
Table::~Table() {	
    delete [] xPoint;
    delete [] yPoint;
}

/*
    Creates a Table of size N
*/
void Table::creates( int N ){
     this->xPoint = new double[N];
     this->yPoint = new double[N];
     this->numberOfPoints = N;
}

void Table::setSymbol( char typeOfPoint ){
     symbol = typeOfPoint;
}

/*
    Returns the number of points of a Table
*/
int Table::getNumberOfPoints(){
    return numberOfPoints;
}

/*
    Fills a Table with points' coordinates x (in xTable) and y (in yTable)
*/
void Table::fills( double* xTable, double* yTable ){
     int i,v;
     
     for (i = 0; i < numberOfPoints; i++){
         xPoint[i] = xTable[i];
         yPoint[i] = yTable[i];
     }
}

// Returns the vector of X coordinates
double* Table::getsXPoint(){
        return xPoint;
}
	  
// Returns the vector of Y coordinates
double* Table::getsYPoint(){
        return yPoint;
}
	  
/**
_______________________________________________________________________________________

                              FUNCTION getOccurrences()

   Returns the number of points existent in an area described by a square, that 
   represents a cell. It receives as parameter the square's center coordinates and
   the square's side length.
 
   Um ponto localizado sobre o perímetro de um quadrado da malha (se a coordenada
   x está sobre uma das linhas verticais ou/e a coordenada y está sobre uma das 
   linhas horizontais do quadrado) será considerado como pertencente ao quadrado 
   mais próximo à sudoeste do ponto. Isso evita que, ao se contar o numero de 
   ocorrencias em uma area, um mesmo ponto (que esteja na divisa entre duas celulas)
   seja incluido em diferentes celulas.
   Deve-se, no entanto, tratar 4 casos: 
           1) Quando o quadrado em questão não está ao extremo oeste do grid nem ao
           extremo sul. Nesse caso a politica de contagem dos pontos que pertecem
           à área do quadrado segue como foi descrito anteriormente
           2) Quando o quadrado em questão está ao extremo oeste do grid e não
           está ao extremo sul. Nesse caso, se um ponto estiver sobre a linha 
           vertical esquerda que limita o quadrado (x=0, y!=0), não é possível 
           considerar este ponto como pertencente ao quadrado à oeste, já que 
           NAO EXISTE quadrado à oeste. Para este caso, portanto, os pontos sobre
           linhas são sempre pertencentes ao quadrado mais próximo ao sul do ponto.
           3) Quando o quadrado em questão não está ao extremo oeste do grid e está
           ao extremo sul. É o inverso do caso anterior.
           4) Quando o quadrado em questão está ao extremo oeste e ao extremo sul.
           É a união dos casos 3 e 4.

____________________________________________________________________________________*/
 
int Table::getOccurrences( double squareCenterX, double squareCenterY, double squareSide)
{								  		 
    int occurrences = 0;
    double halfSide = squareSide / 2.0;
    int i = 0;
	
	 // Case 1
     if ( (squareCenterX - halfSide) != 0 &&
          (squareCenterY - halfSide) != 0)
     {
         // point's coordenates are x != 0 and y != 0
         for(i = 0; i<numberOfPoints; i++ )
         {   if ( (squareCenterX - halfSide < xPoint[i]) &&
                  (xPoint[i] <= squareCenterX + halfSide) )
             {  
                 if ( (squareCenterY - halfSide < yPoint[i]) &&
                      (yPoint[i] <= squareCenterY + halfSide) )
                      occurrences++;
             }
         }
     }
     else
     {
        // Case 2
        if ( (squareCenterX - halfSide) == 0 &&
             (squareCenterY - halfSide) != 0)
        {
            // x can be 0 and y != 0 
            for(i=0; i<numberOfPoints; i++)
            {   if ( (squareCenterX - halfSide <= xPoint[i]) &&
                     (xPoint[i] <= squareCenterX + halfSide) )
                {  
                    if ( (squareCenterY - halfSide < yPoint[i]) &&
                         (yPoint[i] <= squareCenterY + halfSide) )
                       occurrences++;
                }
            }
        }
        
        else
        {  // Case 3
           if ( (squareCenterX - halfSide) != 0 &&
                (squareCenterY - halfSide) == 0)
           {
               // x != 0 and y can be 0
               for(i=0; i<numberOfPoints; i++)
               {  if ( (squareCenterX - halfSide < xPoint[i]) &&
                       (xPoint[i] <= squareCenterX + halfSide) )
                  {  
                      if ( (squareCenterY - halfSide <= yPoint[i]) &&
                           (yPoint[i] <= squareCenterY + halfSide) )
                         occurrences++;
                  }
               }
           }

           else
           { // Case 4
             if ( (squareCenterX - halfSide) == 0 &&
                  (squareCenterY - halfSide) == 0)
             {   
                 //x can be 0 and y can be 0
                 for( int i=0; i<numberOfPoints; i++)
                 {   if ( (squareCenterX - halfSide <= xPoint[i]) &&
                          (xPoint[i] <= squareCenterX + halfSide) )
                     {  
                        if ( (squareCenterY - halfSide <= yPoint[i]) &&
                             (yPoint[i] <= squareCenterY + halfSide) )
                            occurrences++;
                     }
                 }
             }     
           }
        }
     }
	return occurrences;  	 
} 

/*______________________________________________________________________________
                           FUNCTION searchPoints()
                           
         Returns a vector with the positions in Table of the points which are
         inside a cell. The informations of the cell are its center
         (squareCenterX, squareCenterY) and the length of its side (squareSide).
         The number of points inside the cell (the length of the vector) is also
         given (length).

*/
long int* Table::searchPoints(double squareCenterX,double squareCenterY,double squareSide, int length)
{
    double halfSide = squareSide / 2.0;
    int i, j=0;
    long int *vector;

	vector = new long int[length];

	 // Case 1
     if ( (squareCenterX - halfSide) != 0 &&
          (squareCenterY - halfSide) != 0)
     {   // point's coordenates: x != 0 e y != 0
         for(i = 0; i<numberOfPoints; i++ )
         {   if ( (squareCenterX - halfSide < xPoint[i]) &&
                  (xPoint[i] <= squareCenterX + halfSide) )
                 if ( (squareCenterY - halfSide < yPoint[i]) &&
                      (yPoint[i] <= squareCenterY + halfSide) )
                 {  vector[j]=i;
                    j++;
                    if (j==length) return vector;
                 }
         }
     }
     else
     {
        // Case 2
        if ( (squareCenterX - halfSide) == 0 &&
             (squareCenterY - halfSide) != 0)
        {   // x = 0 e y != 0 
            for(i=0; i<numberOfPoints; i++)
            {   if ( (squareCenterX - halfSide <= xPoint[i]) &&
                     (xPoint[i] <= squareCenterX + halfSide) )
                    if ( (squareCenterY - halfSide < yPoint[i]) &&
                         (yPoint[i] <= squareCenterY + halfSide) )
                    {  vector[j]=i;
                       j++;
                       if (j==length) return vector;
                    }
            }
        }
        else
        {  // Case 3
           if ( (squareCenterX - halfSide) != 0 &&
                (squareCenterY - halfSide) == 0)
           {   // x != 0 e y = 0
               for(i=0; i<numberOfPoints; i++)
               {  if ( (squareCenterX - halfSide < xPoint[i]) &&
                       (xPoint[i] <= squareCenterX + halfSide) )
                      if ( (squareCenterY - halfSide <= yPoint[i]) &&
                           (yPoint[i] <= squareCenterY + halfSide) )
                      {  vector[j]=i;
                         j++;
                         if (j==length) return vector;
                      }
               }
           }
           else
           { // Case 4
             if ( (squareCenterX - halfSide) == 0 &&
                  (squareCenterY - halfSide) == 0)
             {   //x = 0 e y = 0
                 for( int i=0; i<numberOfPoints; i++)
                 {   if ( (squareCenterX - halfSide <= xPoint[i]) &&
                          (xPoint[i] <= squareCenterX + halfSide) )  
                        if ( (squareCenterY - halfSide <= yPoint[i]) &&
                             (yPoint[i] <= squareCenterY + halfSide) )
                        {  vector[j]=i;
                           j++;
                           if (j==length) return vector;
                        }
                 }
             }     
           }
        }
     }
     cerr << "\nERRO on function Table::searchPoints() :"
             "\nThe points were not founded in the cell with center"
             " (x,y)=("<<squareCenterX<<", "<<squareCenterY<<") and"
             " side="<<squareSide<<".\nX has to be between "<<squareCenterX - squareSide<<
             " and "<<squareCenterX + squareSide<<", and Y has to be between "<<
             squareCenterY - squareSide<<" and "<<squareCenterY + squareSide<<
             "\nThe number of points searched is "<<length<<"\n";
     system("Pause");
     exit(1);
     return vector;
}

/**
 * Sets the value of all points into the Table using different functions for each
 * coordinate. These functions are passed as parameter, where the first one
 * computes the values of X coordinates, and the second one computes the value
 * of Y coordinates.
 */
void Table::fillTable( double Fx(), double Fy()) 
{    int i;
	 for( i=0; i < numberOfPoints; i++ ) {
	 	  xPoint[i] = Fx();
	 	  yPoint[i] = Fy();
  	 }
}

/*
    Makes a copy of the auxTable, which has the coordenates of the original points
*/
void Table::copyTable( Table *auxTable)
{    int i;

     if ( numberOfPoints!= auxTable->numberOfPoints){
        cout<<"ERROR on function Table::copyTable(): tables have no the same dimension";
        system("pause");
        exit(1);   
     }
	 for( i=0; i < numberOfPoints; i++ ) {
	 	  xPoint[i] = auxTable->xPoint[i];
	 	  yPoint[i] = auxTable->yPoint[i];
  	 }
}


/*
    Create a file with all the points' coordenates
*/
void Table::createFile (char *nome)
{
     int i;
     FILE *Arq;
     
     Arq = fopen(nome,"w");
     fprintf(Arq, "%d\n", numberOfPoints);
     for (i=0; i<numberOfPoints; i++)
     {   
         fprintf(Arq, "%f %f", xPoint[i], yPoint[i]);
         if (i< numberOfPoints-1)
            fprintf(Arq, "\n");
     }
     fclose(Arq);
}

/*
   Muda as coordenadas dos pontos da malha: faz um zoom (diminutivo) com as coordenadas,
   mas mantendo a proporcao da distancia entre os pontos.
*/
void Table::zoomTable ( double w )
{    int i;

     for (i=0; i<numberOfPoints; i++)
     {
         xPoint[i] = w + (1 - 2*w)*xPoint[i];
         yPoint[i] = w + (1 - 2*w)*yPoint[i];   
     }
     
}

/*
    Desloca as coordenadas dos pontos, variando a direção de deslocamento.
    0, para usar as coordenadas sem deslocamentos
    1, para usar as coordenadas deslocadas 'a norte
    2, para usar as coordenadas deslocadas 'a noroeste    
    3, para usar as coordenadas deslocadas 'a oeste
    4, para usar as coordenadas deslocadas 'a sudoeste
    5, para usar as coordenadas deslocadas 'a sul
    6, para usar as coordenadas deslocadas 'a sudeste                     
    7, para usar as coordenadas deslocadas 'a leste         
    8, para usar as coordenadas deslocadas 'a nordeste
    
*/
void Table::coordinatesShift (int desloc, double w)
{    int i; 
     switch (desloc)
     {
            case 0:
            {    break;
            }
            case 1:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + 0.0;
                     yPoint[i] = yPoint[i] + w;
                 }
                 break;
            }
            case 2:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + -w;
                     yPoint[i] = yPoint[i] + w;
                 }
                 break;
            }
            case 3:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + -w;
                     yPoint[i] = yPoint[i] + 0.0;
                 }
                 break;
            }
            case 4:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + -w;
                     yPoint[i] = yPoint[i] + -w;
                 }
                 break;
            }                        
            case 5:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + 0.0;
                     yPoint[i] = yPoint[i] + -w;
                 }
                 break;
            }
            case 6:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + w;
                     yPoint[i] = yPoint[i] + -w;
                 }
                 break;
            }
            case 7:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + w;
                     yPoint[i] = yPoint[i] + 0.0;
                 }
                 break;
            }
            case 8:
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = xPoint[i] + w;
                     yPoint[i] = yPoint[i] + w;
                 }
                 break;
            }
            default:
		    {
                break;
		    }
     }
     
}

/**
 * Desloca as coordenadas dos pontos de acordo com a rotação escolhida
 */
void Table::coordinatesRotacao( int graus ){
     int i;
     double x;
     switch (graus)
     {
            case (0):
            {
                 break;
            }                
            case (1):
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   x = xPoint[i];
                     xPoint[i] = yPoint[i];
                     yPoint[i] = 1 - x;
                 }
                 break;
            }                 
            case (2):
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = 1 - xPoint[i];
                     yPoint[i] = 1 - yPoint[i];
                 }
                 break;
            }                 
            case (3):
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   x = xPoint[i];
                     xPoint[i] = 1 - yPoint[i];
                     yPoint[i] = x;
                 }
                 break;
            }                 
            default:
		    {
                break;
		    }     
     }
     
}

/**
 *  Turn back the position of points (before the rotation have been done)
 */
void Table::turnBackPointsPosition( int graus ){
     int i;
     double x;
     switch (graus)
     {
            case (0):
            {
                 break;
            }                
            case (1):
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   x = xPoint[i];
                     xPoint[i] = 1 - yPoint[i];
                     yPoint[i] = x;
                 }
                 break;
            }                 
            case (2):
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   xPoint[i] = 1 - xPoint[i];
                     yPoint[i] = 1 - yPoint[i];
                 }
                 break;
            }                 
            case (3):
            {
                 for (i=0; i<numberOfPoints; i++)
                 {   x = xPoint[i];
                     xPoint[i] = yPoint[i];
                     yPoint[i] = 1 - x;
                 }
                 break;
            }                 
            default:
		    {
                break;
		    }     
     }
}

void Table::drawNPoints( double x, double y ){
    char nPoints[10];
    char str[50];
    sprintf( nPoints, "%d", numberOfPoints );
    if (type == 'w')
       strcpy(str, "Number of white points: ");
    else
        strcpy(str, "Number of black points: ");
    strcat(str, nPoints);
    drawString( str, GLUT_BITMAP_HELVETICA_12, x, y);
    
}

/**
 * Draws all points of a Table.
 */
void Table::draw( int color, double size ) {
	 setColor( color );
	 
	 if (symbol == 'o'){
	    for ( int i = 0; i < numberOfPoints; i++ ) {
            drawString( "o", GLUT_BITMAP_HELVETICA_12, xPoint[i], yPoint[i]); 
  	    }
     }
	 
     if (symbol == 'x'){
	    for ( int i = 0; i < numberOfPoints; i++ ) {
            drawString( "x", GLUT_BITMAP_HELVETICA_12, xPoint[i], yPoint[i]); 
  	    }
     }
          
     if (symbol == '.'){
          for ( int i = 0; i < numberOfPoints; i++ ) {
	 	      drawPoint( xPoint[i], yPoint[i], size );
  	      }
     }
}
