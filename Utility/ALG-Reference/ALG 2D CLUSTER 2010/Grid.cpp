/* Grid.cpp
Implementation of class Grid. */

// Bibliotecas basicas
#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;

#include <new>
#include <fstream>
    using std::ofstream;

#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

// Arquivos do meu projeto
#include "Grid.h"
#include "functions.h"
#include "table.h"

using namespace std;

//Constructor
Grid::Grid():
    firstGridCell(0),
    sideLength( 0.0 ),
    numberOfCells(1),
    numberOfGroups(0)
{}

//Destructor
Grid::~Grid() {
   int i;
   for (i=0;i<=numberOfGroups;i++)
           delete Matrix[i];
   delete [] Matrix;
   
   Cell *gridCell = firstGridCell;

   while (gridCell!=0){
      Cell *aux = gridCell->next;
      delete gridCell;
      gridCell = aux;
   }
}


/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION initializeGrid()

    Initializes grid with four cells.
    
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::initializeGrid( )
{
    sideLength = (1.0);

    //Subcells.
    Cell *northeastCell = new Cell;
    Cell *northwestCell = new Cell;
    Cell *southeastCell = new Cell;
    Cell *southwestCell = new Cell;    
    
    //Ground transition nodes.     
    Cell *northTransitionNode = new Cell;	
    Cell *southTransitionNode = new Cell;
    Cell *eastTransitionNode  = new Cell;	
    Cell *westTransitionNode  = new Cell;
         
    //Grid geometry.
    double halfSideLength = sideLength/2.0,	
  		   quarterSideLength = sideLength/4.0;
  		   
       	
    //__________________________________________________________________________
    //					Initialization of ground transition nodes.
    //__________________________________________________________________________
  
  	eastTransitionNode->type = 'w';
   	eastTransitionNode->level = 1;
    eastTransitionNode->singleConnector = 0;
    eastTransitionNode->doubleConnector1 = northeastCell;
    eastTransitionNode->doubleConnector2 = southeastCell;
    
  	northTransitionNode->type = 'w';
 	northTransitionNode->level = 1; 
    northTransitionNode->singleConnector = 0; 
   	northTransitionNode->doubleConnector1 = northeastCell;
    northTransitionNode->doubleConnector2 = northwestCell;   
    
   	westTransitionNode->type = 'w';
   	westTransitionNode->level = 1;
    westTransitionNode->singleConnector = 0;
    westTransitionNode->doubleConnector1 = northwestCell;
    westTransitionNode->doubleConnector2 = southwestCell;
    
   	southTransitionNode->type = 'w';
  	southTransitionNode->level = 1;
    southTransitionNode->singleConnector = 0;
    southTransitionNode->doubleConnector1 = southeastCell;
    southTransitionNode->doubleConnector2 = southwestCell;
 
    //__________________________________________________________________________
    //					  Initialization of cell nodes.
    //__________________________________________________________________________
 
    //Northeast subcell initialization.  
 	northeastCell->type = 'b';
  	northeastCell->level = 1;
  	northeastCell->bunchNumber = 1;
  	northeastCell->faceLength = halfSideLength;
  	northeastCell->halfFaceLength = quarterSideLength;
  	
  	northeastCell->east = eastTransitionNode;
  	northeastCell->north = northTransitionNode;
  	northeastCell->west = northwestCell;
   	northeastCell->south = southeastCell;
   	northeastCell->previous = 0;
   	northeastCell->next = northwestCell;
   	
   	northeastCell->gridPosition = 0;
    northeastCell->hilbertShapeNumber = 0;
    
   	northeastCell->centerX = halfSideLength + quarterSideLength;
   	northeastCell->centerY = halfSideLength + quarterSideLength;

    //Northwest subcell initialization.  
  	northwestCell->type = 'b';
   	northwestCell->level = 1;
   	northwestCell->bunchNumber = 2;
  	northwestCell->faceLength = halfSideLength;
  	northwestCell->halfFaceLength = quarterSideLength;
  	
  	northwestCell->east = northeastCell;
  	northwestCell->north = northTransitionNode;
  	northwestCell->west = westTransitionNode;
   	northwestCell->south = southwestCell;
   	northwestCell->previous = northeastCell;
   	northwestCell->next = southwestCell;
 
	northwestCell->gridPosition = 1;
	northwestCell->hilbertShapeNumber = 1;
    
   	northwestCell->centerX = halfSideLength - quarterSideLength;
   	northwestCell->centerY = halfSideLength + quarterSideLength;
     
    //Southwest subcell initialization.  
  	southwestCell->type = 'b';
   	southwestCell->level = 1;
   	southwestCell->bunchNumber = 3;
  	southwestCell->faceLength = halfSideLength;
  	southwestCell->halfFaceLength = quarterSideLength;
  	
  	southwestCell->east = southeastCell;
  	southwestCell->north = northwestCell;
  	southwestCell->west = westTransitionNode;
   	southwestCell->south = southTransitionNode;
   	southwestCell->previous = northwestCell;
   	southwestCell->next = southeastCell;
   	
   	southwestCell->gridPosition = 2;
	southwestCell->hilbertShapeNumber = 2;
    
   	southwestCell->centerX = halfSideLength - quarterSideLength;
   	southwestCell->centerY = halfSideLength - quarterSideLength;
  
    //Southeast subcell initialization.  
  	southeastCell->type = 'b';
   	southeastCell->level = 1;
   	southeastCell->bunchNumber = 4;
  	southeastCell->faceLength = halfSideLength;
  	southeastCell->halfFaceLength = quarterSideLength;
  	
  	southeastCell->east = eastTransitionNode;
  	southeastCell->north = northeastCell;
  	southeastCell->west = southwestCell;
   	southeastCell->south = southTransitionNode;
   	southeastCell->previous = southwestCell;
   	southeastCell->next = 0;
   	
   	southeastCell->gridPosition = 3;
	southeastCell->hilbertShapeNumber = 3;
    
   	southeastCell->centerX = halfSideLength + quarterSideLength;
   	southeastCell->centerY = halfSideLength - quarterSideLength;
   	
    //Grid initialization
    firstGridCell = northeastCell;        
    numberOfCells = 4;
    
    orderGridCells();
    
}//end initializeGrid() function

/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION drawHilbertCurve()
                        	
    Draws Hilbert curve on the whole grid.
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::drawHilbertCurve( int color )
{
    Cell *gridCell = firstGridCell;
    setColor( color );
    while( gridCell->next != 0 )
    {
        drawLine( gridCell->centerX, gridCell->centerY,0,
                  gridCell->next->centerX, gridCell->next->centerY,0 );
        gridCell = gridCell->next;
    } 

}//end drawHilbertCurve() function.


/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION drawBlackCurve()
                        	
    Draws blackCurve on the whole grid.
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::drawBlackCurve( int color )
{
    Cell *gridCell = firstGridCell;
    setColor( color );
    while( gridCell->next != 0 )
    {
       if ( gridCell->uBlack == 1 && gridCell->next->uBlack == 1 ){
          drawLine( gridCell->xBlack, gridCell->yBlack, 0,
                    gridCell->next->xBlack, gridCell->next->yBlack, 0 );
       }
       if ( (gridCell->uBlack == 0 || gridCell->uBlack > 1) && gridCell->next->uBlack == 1 ){
          drawLine( gridCell->centerX, gridCell->centerY, 0,
                    gridCell->next->xBlack, gridCell->next->yBlack,0 );
       }
       if ( gridCell->uBlack == 1 && (gridCell->next->uBlack == 0 || gridCell->next->uBlack > 1)){
          drawLine( gridCell->xBlack, gridCell->yBlack, 0,
                    gridCell->next->centerX, gridCell->next->centerY,0 );
       }
       if ( gridCell->uBlack != 1 && gridCell->next->uBlack != 1 ){
          drawLine( gridCell->centerX, gridCell->centerY, 0,
                    gridCell->next->centerX, gridCell->next->centerY,0 );
       }
       
       gridCell = gridCell->next;
    } 

}//end drawBlackCurve() function.



/*______________________________________________________________________________
                           
                           FUNCTION drawLineOfGroups()
                        	
    Draws the lines that make conection between cells of the same group on
    the whole grid.
______________________________________________________________________________*/
void Grid::drawLineOfGroups( int color )
{
    Cell *gridCell = firstGridCell;;
    setColor( color );
    
    setLineWidth(1.5);
    while( gridCell->next != 0 )
    {
        if ( gridCell->group!=0 && gridCell->next->group == gridCell->group )
        {   drawLine( gridCell->centerX, gridCell->centerY,0,
                      gridCell->next->centerX, gridCell->next->centerY,0 );                     
        }
        gridCell = gridCell->next;
    }
    setLineWidth(1.0);
    
}//end drawLineOfGroups() function.


/*______________________________________________________________________________
                           
                           FUNCTION drawGroups()
    
    Colore o fundo das celulas que pertencem a um supergrupo.                   
    __________________________________________________________________________*/
void Grid::drawSuperGroups( int color )
{
    double x0,y0,x1,y1;
    Cell *gridCell = firstGridCell;
    setColor( color );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while( gridCell->next != 0 )
    {
        if ( gridCell->group!=0 )
        { 
            x0 = gridCell->centerX - gridCell->halfFaceLength;
            y0 = gridCell->centerY - gridCell->halfFaceLength;
            x1 = gridCell->centerX + gridCell->halfFaceLength;
            y1 = gridCell->centerY + gridCell->halfFaceLength;
            printRectangle(x0,y0,x1,y1 );
        }
        gridCell = gridCell->next;
    }
    
}//end drawGroups() function.


/*______________________________________________________________________________
                           
                           FUNCTION printNumberOfSuperGroups()
                        	
    Print the number of supergroup of each cell that belongs to a supergroup !=0
    on the whole grid.
______________________________________________________________________________*/
void Grid::printNumberOfSuperGroups( int color )
{
    char superGroup[5];
    Cell *gridCell = firstGridCell;;
    setColor( color );

    while( gridCell != 0 )
    {
        if( gridCell->superGroup != 0 )
        {  sprintf( superGroup, "%d", gridCell->superGroup );
           drawString( superGroup, GLUT_BITMAP_HELVETICA_10, gridCell->centerX, gridCell->centerY);
        }    
        gridCell = gridCell->next;
    }
    
}//end printNumberOfSuperGroups() function.


/*______________________________________________________________________________
                           
                           FUNCTION writeCellsNumber()
                        	
    Print the number of each cell on the whole grid, following Hilbert's Curve.
______________________________________________________________________________*/
void Grid::writeCellsNumber( int color )
{
    char cellNumber[5];
    Cell *gridCell = firstGridCell;;
    setColor( color );

    while( gridCell != 0 )
    {
        sprintf( cellNumber, "%d", gridCell->gridPosition );
        drawString( cellNumber, GLUT_BITMAP_HELVETICA_10, gridCell->centerX+0.0001, gridCell->centerY+0.0001); 
        gridCell = gridCell->next;
    }
    
}//end writeCellsNumber() function.




/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION drawGrid()
    Draws whole grid.
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::drawGrid( int color )
{
 	 setColor( color );
 	 Cell *gridCell = firstGridCell;
 	 while ( gridCell != 0 ){
	  	   gridCell->drawCell();
	  	   gridCell = gridCell->next;
	 }
}

/*______________________________________________________________________________
________________________________________________________________________________
                           
                            FUNCTION orderGridCells()
                        	
    Enumerates cells of grid according to the ordering given by Hilbert's curve.
    
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::orderGridCells()
{
    Cell *gridCell = firstGridCell;;
    
    int counter = 0;
    while( gridCell != 0 )
   	{
        gridCell->gridPosition = counter;
        gridCell = gridCell->next;
        counter++;   	    
    }
    
}//end orderGridCells() function.

/* _____________________________________________________________________________

                              FUNCTION getWhitePoints()
     
       Percorre a curva de Hilbert e, para cada cell, chama a função
       getOccurences() enviando as coordenadas dos centros de cell e
       o comprimento do lado de cell (faceLength). Será retornado a
       cada vez o número de pontos brancos que existem dentro daquela
       celula, que será armazenado em uWhite.
       
*/
void Grid::getWhitePoints(Table *whiteTable)
{    Cell *gridCell = firstGridCell;
     
     while( gridCell != 0 )
     {
        gridCell->uWhite = 
        whiteTable->getOccurrences( gridCell->centerX, gridCell->centerY, gridCell->faceLength );
        gridCell = gridCell->next;
     }
}


/*_________________________________________________________________________________ 

                                 FUNTION MatchPoints() 

   Percorre a curva de hilbert comparando as distancias entre as celulas consecutivas.
   Duas células consecutivas só se juntarão em um grupo se pelo menos umas delas tiver
   um ponto preto. Depois desta condição, compara-se a distancia entre as células.
   Para distancias menores ou iguais a MAX, celulas são 
   agrupadas de forma a pertencerem a um mesmo grupo. Duas celulas pertencem a um mesmo
   grupo se suas variaveis "group" indicarem o mesmo valor. Uma celula cujo "group" é
   zero nao pertence a grupo algum.
   Quando é encontrada uma distancia superior a "MAX" ou quando encontra-se duas
   células consecutivas sem pontos pretos, o grupo corrente X é finalizado.
   O proximo grupo (X+1) é iniciado quando voltarmos a achar distancias <= a MAX.
   Ps: sempre que a celula tiver mais de um ponto preto, ela jah entra
   no grupo, mesmo se nao tiver a distancia maxima necessaria.   

*/
void Grid::MatchPoints( Table *blackTable, double MAX )
{
     Cell *gridCell;
     int currentGroup=0;
     double distance;
     
     gridCell = firstGridCell;
     while ( gridCell!=0 && gridCell->next!=0 )
     {  
        if ( gridCell->uBlack>=1 || gridCell->next->uBlack>=1 )
        {  distance = 
           Distance(gridCell,gridCell->next, blackTable);
        
           if (distance <= MAX || gridCell->uBlack > 1)
           {  currentGroup++;
              gridCell->group = currentGroup;
              while ( distance<=MAX && gridCell->next!=0 && (gridCell->uBlack>=1 || gridCell->next->uBlack>=1) )
              {
                    gridCell = gridCell->next;
                    gridCell->group = currentGroup;
                    if (gridCell->next==0) break;             
                    distance = Distance(gridCell,gridCell->next, blackTable);
                    if ( distance > MAX && gridCell->next->uBlack > 1 )
                       distance = MAX; /* permite que a proxima celula pertença ao grupo
                                       corrente (mesmo que nao esteja perto o suficiente)
                                       caso ela contenha mais de um ponto preto */
              }
           }
        }
        gridCell = gridCell->next;
     }
     this->numberOfGroups = currentGroup;
     
     if ( numberOfGroups == 0 ){ 
          cout << "\n\nWarning: no groups could be made. "
               <<"Check if the parameters MAX and SUPERMAX are not too small.\n";
     }
     
     CreateMatrix();

}

/* _____________________________________________________________________________
                          FUNCTION Distance()
         Ordem de prioridade para calculos de distancia:
              - Se cada uma das celulas tiver 1 ponto preto, calcula a distancia
                entre as coordenadas dos pontos pretos.
              - Se uBlack != 1 em uma, e uBlack = 1 em outra, calcula a distancia
                respectivamente do centro de uma à coordenada do ponto preto da
                outra.
              - Em ultimo caso, calcula-se a distancia de centro a centro.
         Ps: sempre que a celula tiver mais de um ponto preto, a distancia à ela
         é calculada em relação a seu centro.
         
         A distancia eh calculada como sendo o valor do maior cateto.
*/
double Grid::Distance( Cell *gridCell, Cell *nextGridCell, Table *blackTable )
{
    double distanceOfPoints, X1, X2, Y1, Y2;
    long int *vectorCell, *vectorNextCell;
    
    X1 = gridCell->centerX;
    X2 = nextGridCell->centerX;
    Y1 = gridCell->centerY;
    Y2 = nextGridCell->centerY;
    
    /* distance between the coordinates of the gridCell's blackpoint and the
       nextGridCell's blackpoint */
    if ( gridCell->uBlack == 1 && nextGridCell->uBlack == 1 ){

       vectorCell = blackTable->searchPoints(X1,Y1,gridCell->faceLength,gridCell->uBlack);
       vectorNextCell = blackTable->searchPoints(X2,Y2,nextGridCell->faceLength,nextGridCell->uBlack);
       
       distanceOfPoints = fabs( blackTable->getsXPoint()[vectorCell[0]] - blackTable->getsXPoint()[vectorNextCell[0]] );
       if ( distanceOfPoints < fabs( blackTable->getsYPoint()[vectorCell[0]] - blackTable->getsYPoint()[vectorNextCell[0]] ) )
          distanceOfPoints = fabs( blackTable->getsYPoint()[vectorCell[0]] - blackTable->getsYPoint()[vectorNextCell[0]] );     
    }
    else {
       /* distance between the center of gridCell and the coordinates of nextGridCell's
          blackpoint */
       if ( ( gridCell->uBlack == 0 || gridCell->uBlack > 1 ) && nextGridCell->uBlack == 1 ){
    
          vectorNextCell = blackTable->searchPoints(X2,Y2,nextGridCell->faceLength,nextGridCell->uBlack);       
          distanceOfPoints = fabs( X1 - blackTable->getsXPoint()[vectorNextCell[0]] );
          if ( distanceOfPoints < fabs( Y1 - blackTable->getsYPoint()[vectorNextCell[0]] ) )
             distanceOfPoints = fabs( Y1 - blackTable->getsYPoint()[vectorNextCell[0]] );          
       }

       else { 
          /* distance between the coordinates of gridCell's blackpoint and the center
             of nextGridCell */
          if ( gridCell->uBlack == 1 && ( nextGridCell->uBlack == 0 || nextGridCell->uBlack > 1 ) ){

             vectorCell = blackTable->searchPoints(X1,Y1,gridCell->faceLength,gridCell->uBlack);       
             distanceOfPoints = fabs( blackTable->getsXPoint()[vectorCell[0]] - X2 );
             if ( distanceOfPoints < fabs( blackTable->getsYPoint()[vectorCell[0]] - Y2 ) )
                distanceOfPoints = fabs( blackTable->getsYPoint()[vectorCell[0]] - Y2 );          
          }    
        
          else {
          /* distance between the center of gridCell and the center of nextGridCell */
             distanceOfPoints = fabs(X1 - X2);
             if (distanceOfPoints < fabs(Y1 - Y2) )
                distanceOfPoints = fabs(Y1 - Y2);    
          }
       }
    }
            
    return (distanceOfPoints);
}

/*______________________________________________________________________________
                            FUNCTION CreateMatrix()

     Creates and initialites with 0's the matrix of groups. That's a square matrix,
     and will indicate if a group i makes conection with a group j. 
     A linha e coluna zero da Matrix nao serao usadas.
     
*/
void Grid::CreateMatrix()
{
    int i, j;
    Matrix = new int*[numberOfGroups+1];
    for (i=0; i<=numberOfGroups; i++)
        Matrix[i] = new int[numberOfGroups+1];
    
    for (i=0;i<=numberOfGroups;i++)
         for (j=0;j<=numberOfGroups;j++)
             Matrix[i][j]=0;
}
/*______________________________________________________________________________
                           FUNCTION SimpleDistance
          Returns the distance between the points (X1,Y1) and (X2,Y2).
          The distance is calculated as been the higher cathetus.

*/
double Grid::SimpleDistance( double X1, double Y1, double X2, double Y2 )
{
    double distance;

    distance = fabs(X1 - X2);
    if (distance < fabs(Y1 - Y2) )
         distance = fabs(Y1 - Y2);
    
    return (distance);
}


/*_______________________________________________________________________________
                              FUNTION MatchGroups()
     Union of groups

   Percorre a curva de hilbert procurando por celulas que pertençam a algum grupo!=0. 
   Para cada celula encontrada, verifica-se os seus 4 vizinhos ( direçoes norte, sul,
   leste, oeste).
   Para cada direção:
         Se o vizinho for uma celula do tipo preta, então verifica-se
   se este pertence a um grupo!=0 e se seu grupo eh diferente do grupo da celula analisada.
   Caso essas condicoes sejam satisfeitas, verifica-se a distancia entre os centros
   da celula e de sua vizinha. Por sua vez, se essa distancia for menor ou igual a
   uma distancia maxima estipulada (SUPERMAX), entao chama-se
   a funcao AtualizeMatrix(), que ira indicar na Matrix a existencia de uma ligacao
   entre o grupo da celula e o grupo de sua vizinha. 
         Se o vizinho for uma celula do tipo branca (celula de transicao), então
   chama-se a funcao FindNeighbor() enviando como parametros um apontador para a
   celula e outro para sua vizinha. A funcao ira procurar a proxima celula do tipo
   preta (na mesma direcao que esta sendo avaliada) subsequente à vizinha. Ao
   retornar, se existir essa proxima preta, então o procedimento fica sendo o mesmo
   descrito anteriormente para vizinhos pretos. Se essa proxima preta nao existir,
   significa que chegamos a uma celula que se encontra no limite do grid, ou seja,
   aponta para null.
   
   Observacao: os valores armazenados na Matrix serão sempre maiores ou iguais a zero.
   Se a coneccao entre dois grupos for 0, eh porque esses dois grupos nao se ligam
   diretamente;
   Se for 1, eh porque existe uma ligacao direta entre eles;
   Se for 2, eh porque existem duas ligacoes diretas entre eles;
   e assim por diante.

*/
void Grid::MatchGroups( int N, double SUPERMAX )
{
  double distance;
  Cell *gridCell, *auxiliarCell;
     
  if ( numberOfGroups > 0 ){   
     
     gridCell = firstGridCell;
     while ( gridCell!=0 )
     {
           if ( gridCell->group!=0 )
           {
                if ( gridCell->north!=0 ){
                   //vizinho preto
                   if (gridCell->north->type == 'b'){
                      if (gridCell->north->group!=0 && gridCell->north->group!=gridCell->group)
                      {
                       distance = 
                       SimpleDistance(gridCell->centerX,gridCell->centerY,gridCell->north->centerX,gridCell->north->centerY);
                       if ( distance<=SUPERMAX )
                          AtualizeMatrix(gridCell->group, gridCell->north->group);
                      }
                   }
                   //vizinho branco
                   else{
                      auxiliarCell = gridCell->north;
                      auxiliarCell = FindNeighbor( gridCell, auxiliarCell );                
                      if ( auxiliarCell->type == 'b' )  
                      {   if ( auxiliarCell->group!=0 && auxiliarCell->group!=gridCell->group)
                          {
                            distance = 
                            SimpleDistance(gridCell->centerX,gridCell->centerY,auxiliarCell->centerX,auxiliarCell->centerY);
                            if ( distance<=SUPERMAX )
                              AtualizeMatrix(gridCell->group, auxiliarCell->group);
                          }
                      }
                   }
                }
                
                if ( gridCell->south!=0 ){
                   if (gridCell->south->type == 'b'){
                      if (gridCell->south->group!=0 && gridCell->south->group!=gridCell->group)
                      {
                       distance = 
                       SimpleDistance(gridCell->centerX,gridCell->centerY,gridCell->south->centerX,gridCell->south->centerY);
                       if ( distance<=SUPERMAX )
                          AtualizeMatrix(gridCell->group, gridCell->south->group);
                      }
                   }
                   else{
                      auxiliarCell = gridCell->south;  
                      auxiliarCell = FindNeighbor( gridCell, auxiliarCell );
                      if ( auxiliarCell->type == 'b' ){
                          if ( auxiliarCell->group!=0 && auxiliarCell->group!=gridCell->group)
                          { distance = 
                            SimpleDistance(gridCell->centerX,gridCell->centerY,auxiliarCell->centerX,auxiliarCell->centerY);
                            if ( distance<=SUPERMAX )
                               AtualizeMatrix(gridCell->group, auxiliarCell->group);
                          }
                      }
                   }
                }
                
                if ( gridCell->east!=0 ){
                   if (gridCell->east->type == 'b'){
                      if (gridCell->east->group!=0 && gridCell->east->group!=gridCell->group)
                      { distance = 
                        SimpleDistance(gridCell->centerX,gridCell->centerY,gridCell->east->centerX,gridCell->east->centerY);
                        if ( distance<=SUPERMAX )
                           AtualizeMatrix(gridCell->group, gridCell->east->group);
                      }
                   }
                   else{
                      auxiliarCell = gridCell->east;  
                      auxiliarCell = FindNeighbor( gridCell, auxiliarCell );
                      if ( auxiliarCell->type == 'b' ){
                          if ( auxiliarCell->group!=0 && auxiliarCell->group!=gridCell->group)
                          { distance = 
                            SimpleDistance(gridCell->centerX,gridCell->centerY,auxiliarCell->centerX,auxiliarCell->centerY);
                            if ( distance<=SUPERMAX )
                               AtualizeMatrix(gridCell->group, auxiliarCell->group);
                          }
                      }
                   }
                }
                
                if ( gridCell->west!=0 ){
                   if (gridCell->west->type == 'b'){
                      if (gridCell->west->group!=0 && gridCell->west->group!=gridCell->group)
                      { distance = 
                        SimpleDistance(gridCell->centerX,gridCell->centerY,gridCell->west->centerX,gridCell->west->centerY);
                        if ( distance<=SUPERMAX )
                          AtualizeMatrix(gridCell->group, gridCell->west->group);
                      }
                   }
                   else{
                      auxiliarCell = gridCell->west;  
                      auxiliarCell = FindNeighbor( gridCell, auxiliarCell );
                      if ( auxiliarCell->type == 'b' ){
                          if ( auxiliarCell->group!=0 && auxiliarCell->group!=gridCell->group)
                          { distance = 
                            SimpleDistance(gridCell->centerX,gridCell->centerY,auxiliarCell->centerX,auxiliarCell->centerY);
                            if ( distance<=SUPERMAX )
                               AtualizeMatrix(gridCell->group, auxiliarCell->group);
                          }
                      }
                   }
                }
           }
           gridCell = gridCell->next;          
     }
  }
  
}

/*__________________________________________________________________________________

                         FUNCTION FindNeighbor()
                         
       Recebe como parametros uma celula (gridCell), e sua vizinha auxiliarCell
       (na direcao norte, sul, leste ou oeste) que eh inicialmente uma celula branca.
       A funcao procura pela proxima vizinha preta (na mesma direcao), se houver, e
       a aponta com neighborGridCell.

*/
Cell* Grid::FindNeighbor( Cell *gridCell, Cell *auxiliarCell)
{
    Cell *neighborGridCell;

    neighborGridCell = auxiliarCell;
  	if ( neighborGridCell->level > gridCell->level ){
    	while( neighborGridCell->type == 'w' )
    	{
            neighborGridCell = neighborGridCell->doubleConnector2;

        }
    }
  	else{
        if (neighborGridCell->level == gridCell->level )
        {  while ( ( neighborGridCell->type == 'w' ) && ( neighborGridCell->singleConnector != 0 ) )
           {
    	      neighborGridCell = neighborGridCell->singleConnector;
           }
        }
    }

    return neighborGridCell;
}

/* _____________________________________________________________________________

     Incrementa uma ligacao entre dois grupos na Matrix
*/
void Grid::AtualizeMatrix(int group1, int group2)
{    
     Matrix[group1][group2] = Matrix[group1][group2] + 1;
     Matrix[group2][group1] = Matrix[group2][group1] + 1;
}

/* _____________________________________________________________________________

     Constroi uma segunda Curva (denominada blackCurve) que acompanha a Curva de
     Hilbert, mas que ao invés de ligar centro a centro das celulas consecutivas,
     liga:
          - se houver 1 ponto preto na celula, liga às coordenadas deste.
          - se não houver ponto preto, ou houver mais de um na célula, liga ao
          centro.

*/

void Grid::completeToMakeBlackCurve( Table *blackTable )
{
    Cell *gridCell = firstGridCell;;
    long int *vectorBlack;

    while ( gridCell != 0 )
    {
          if( gridCell->uBlack == 1 ) {
              vectorBlack = blackTable->searchPoints(gridCell->centerX, gridCell->centerY, gridCell->faceLength, gridCell->uBlack);
              /* Insere nas variaveis da celula corrente as informações das coordenadas
                 do ponto preto (se houver 1) */
              gridCell->xBlack = blackTable->getsXPoint()[vectorBlack[0]];
              gridCell->yBlack = blackTable->getsYPoint()[vectorBlack[0]];
          }
          gridCell = gridCell->next;
    }
}

/* _____________________________________________________________________________
   
       Turn back the coordinates (to the original) of all cells' centers of the grid.
       
*/
void Grid::turnBackGridPosition( int i ){

     Cell *gridCell = firstGridCell;
     double x;

     switch (i){
            case (0):
            {
                 break;
            }
            case (1):
            {
                 while ( gridCell != 0 ){ 
                     x = gridCell->centerX;
                     gridCell->centerX = 1 - gridCell->centerY;
                     gridCell->centerY = x;
                     gridCell = gridCell->next;
                 }
                 break;
            }
            case (2):
            {
                 while ( gridCell != 0 ){ 
                       gridCell->centerX = 1 - gridCell->centerX;
                       gridCell->centerY = 1 - gridCell->centerY;
                       gridCell = gridCell->next;
                 }
                 break;
            }
            case (3):
            {
                 while ( gridCell != 0 ){
                       x = gridCell->centerX;
                       gridCell->centerX = gridCell->centerY;
                       gridCell->centerY = 1 - x;                       
                       gridCell = gridCell->next;
                 }
                 break;
            }
            default:
		    {
                break;
		    }     
     }
     
}


/* _____________________________________________________________________________
   
     Imprime os valores da Matrix de ligacoes de grupos em um arquivo texto
*/
void Grid::PrintMatrix(char *fileName)
{    int v;
     int i,j;

     ofstream file( fileName, ios::trunc );     
     if( !file.is_open() ) {
         cerr << "Error on function Grid::printMatrix(): The file " << fileName
              << " could not be opened." << endl;
         return;
     }
          
     file << "Total de grupos: " << numberOfGroups <<endl;
     file << "Grupo1 \t\t Grupo2 \t numero de ligacoes" <<endl;
     
     for (i=1; i<=numberOfGroups; i++){
         for (j=i; j<=numberOfGroups; j++)
         {  if (Matrix[i][j]!=0)
               file << i << "\t\t" << j << "\t\t\t" << Matrix[i][j] <<endl;
         }
     }
     file.close();
}

// imprime em um arquivo-texto informacoes de toda celula do grid que pertença a um grupo!=0
void Grid::PrintCellsSuperGroups(char *fileName)
{
     int v;
     Cell *gridCell;
     
     ofstream file( fileName, ios::trunc );
     if( !file.is_open() ) {
         cerr << "Error on function Grid::PrintCellsSuperGroups(): The file " << fileName 
              << " could not be opened." << endl;
         return;
     }          

     gridCell = firstGridCell;
     while( gridCell!=0 )
     {
         if (gridCell->group!=0)
         { file<<"Cell = "<<gridCell->gridPosition;     
           file<<"  Cell(x,y) = ("<<gridCell->centerX<<", "<<gridCell->centerY<<")";
           file<<"  SuperGroup = "<<gridCell->superGroup;
           file<<"  Group = "<<gridCell->group<<"\n";
         }
         gridCell = gridCell->next;
     }
     
     file.close();
}


// imprime em um arquivo-texto informacoes de todas as celulas do grid
void Grid::printCells(char *fileName, Table *blackTable)
{    int v=0;
     int i, n;
     long int *positionBlack;
     Cell *gridCell;

     ofstream file( fileName, ios::trunc );
     if( !file.is_open() ) {
         cerr << "Error on function Grid::printCells(): The file " << fileName 
              << " could not be opened." << endl;
         return;
     }

     gridCell = firstGridCell;
     while ( gridCell!=0 )
     {   n = gridCell->uBlack;
         file<<"Cell = "<<gridCell->gridPosition;
         file<<"  nBlack = "<<n;   
         file<<"  nWhite = "<<gridCell->uWhite;
         file<<"  Cell(x,y) = ("<<gridCell->centerX<<", "<<gridCell->centerY<<")";
         if ( n == 1 )
            file<<"  blackPoint(x,y) = ("<<gridCell->xBlack<<", "<<gridCell->yBlack<<")";
         
         if ( n > 1 ){
            positionBlack = blackTable->searchPoints(gridCell->centerX, gridCell->centerY, gridCell->faceLength, n);
            for (i=0; i<n; i++){
                file<<"  blackPoint(x,y) = ("<<blackTable->getsXPoint()[positionBlack[i]]
                    <<", "<<blackTable->getsYPoint()[positionBlack[i]]<<")";
            }
         }
         file<<"\nlevel = "<<gridCell->level;
         file<<"  SuperGroup = "<<gridCell->superGroup;
         file<<"  Group = "<<gridCell->group<<"\n\n";
          v+=n;
         gridCell = gridCell->next;
     }
     
     file.close();
}
