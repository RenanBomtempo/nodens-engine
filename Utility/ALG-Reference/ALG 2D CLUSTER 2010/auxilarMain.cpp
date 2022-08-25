#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;
#include <fstream>
    using std::ofstream;
#include <list>
#include <math.h>
#include "auxiliarMain.h"
#include "Grid.h"
#include "Graph.h"
#include "functions.h"

using namespace std;

extern Grid *grid;
extern Table *blackTable;
extern Table *whiteTable;
extern Graph *graph;
extern double MAX, SUPERMAX;
    
void simulation()
{
    Table *bigTable;
    int i, TOTAL, numberOfTheBest, at, invalidos[N_STATISTICS];
    int NWhite, NBlack, *BLACKPOINTS, *HIGHEST, counter, nMaxBlackInSG[ITERACOES];
    double auxIntensity, intensity, original;
    double scan[ITERACOES], mediaScan[N_STATISTICS], mediaMaxBlack[N_STATISTICS];
    bool END;
         
    //_________________________________________________________________________
    // Application.

    /* Does the same rotine for black points and later for the white points:
       - Read the total number N of black/white points from a file;
       - Get the coordinates of all black/white points from a file and fill the
       vectors xTable and yTable with them;
       - Creates an auxiliar Table, with size N and fills that with the
       coordinates' vectors;
       - Creates the main Table, with size N.
    */

    cout << "\n\nProcessando...  ";
    
    char fileNamePoints[50] = "arquivos/INPUT/Dengue.txt";
    char fileNameMAX_SUPERMAX[50] = "arquivos/INPUT/MAX_SUPERMAX.txt";
    
    readNumberOfPointsInFile( fileNamePoints, TOTAL, NBlack);

    bigTable = new Table('.', 'w');
    bigTable->creates(TOTAL);
    NWhite = TOTAL - NBlack;
    
    blackTable = new Table('x', 'b');
    whiteTable = new Table('.', 'w');
    whiteTable->creates( NWhite );
    blackTable->creates( NBlack );
        
    readFile(fileNamePoints,TOTAL, bigTable);
    createNewFile("arquivos/INPUT/superGrupos.txt");
    
    // Lê MAX e SUPERMAX
    readMaxAndSupermax( fileNameMAX_SUPERMAX, MAX, SUPERMAX);

    // BEGIN SIMULATION

            BLACKPOINTS = originalBlackPoints( TOTAL, NBlack );

            /* Divide os pontos pretos e brancos entre as tabelas de pontos pretos e brancos */
            dividePoints(bigTable, blackTable, whiteTable, BLACKPOINTS);
            
            /* Creates an empty structure of class Grid */
            grid = new Grid;
            
			/* Grid Initialization: creates and organizes the first 4 grid's cells. */
			grid->initializeGrid( );
			
			/* Sets for the first time the number of black points (cell.uBlack) in
			each of the 4 first cells. */
			initialCondition( grid, blackTable );
			
			/* Refines the grid */
			refineGrid( grid, blackTable, refinementLevel1, refinementLevel2, refinementBound );
            
		    /* Sets the number of white points (cell.uWhite) in each cell
			after the refinement of the grid */
			grid->getWhitePoints(whiteTable);
	
			/* Matches the points (just those that are consecutive by the Hilbert Curve)
			and creates an empty Matrix of conections */
			grid->MatchPoints( blackTable, MAX );
            
            if ( grid->numberOfGroups > 0 ){
			      /* Does the union of the groups by filling the Matrix */
                  grid->MatchGroups( blackTable->getNumberOfPoints(), SUPERMAX );            
			     
                  /* Creates an empty structure of class Graph, where will be store the
			      supergroups */
                  graph = new Graph(grid->numberOfGroups);

                  /* Creates the supergroups */        
			      graph->mainSuperGroups(grid->numberOfGroups, grid->Matrix);
            
			      /* Informs to each cell its supergroup's number */
			      graph->numberCellSuperGroup(grid);

			      /* Completes the graph with points' informations of each cell */
			      graph->completeGraph(grid);

			      /* Sets the number of all points (black and white) in the graph, for each supergroup.*/
			      graph->getNumberOfAllPoints();

                  /* Sets the intensity of each supergroup of the graph */
                  graph->setIntensity(TOTAL, NBlack);

                  /* Sets the cohesion of this partition of the grid */
			      graph->getCohesion(NWhite, NBlack);

			      /* Completes the information of black points of each cell to later
                  make the black curve */	
                  grid->completeToMakeBlackCurve(blackTable);
			
			      /* Calculates the highest intensity of the supergroups in the graph */  
	              auxIntensity = graph->highestIntensity( numberOfTheBest );

            }
            else {
                 cout<<"\nIteracao invalida.";
            }

    // END SIMULATION
                
        Print(); /* Print the files of the last instance */
    
    cout << "\nFinish!" << endl;

    instrucoes();

    delete bigTable;

}

void draw_MAX_SUPERMAX(){

    char max[5];
    char super_max[5];
    
    char str1[30];
    char str2[30];
    strcpy(str1, "MAX = ");
    strcpy(str2, "SUPERMAX = ");
        
    sprintf( max, "%f", MAX );
    strcat(str1, max);
    drawString(  str1, GLUT_BITMAP_HELVETICA_12, 0.0, 1.07); 
         
    sprintf( super_max, "%f", SUPERMAX );
    strcat(str2, super_max);
    drawString( str2, GLUT_BITMAP_HELVETICA_12, 0.0, 1.04);
}

void draw_DATA( int color){
   setColor( color );
   draw_MAX_SUPERMAX();
   blackTable->drawNPoints(0.5, 1.07);
   whiteTable->drawNPoints(0.5, 1.04);
        
}

void readMaxAndSupermax( char *fileName, double &MAX, double &SUPERMAX){
     char string[15];
     FILE *Arq;

     Arq = fopen(fileName, "r" );

     //lê o MAX no arquivo
     fscanf(Arq, "%s", string);
     //converte o string lido no real SUPERMAX
     MAX = atof(string);
     
     //lê o SUPERMAX no arquivo
     fscanf(Arq, "%s", string);
     //converte o string lido no real SUPERMAX
     SUPERMAX = atof(string);
     
     fclose(Arq);  

}

/*______________________________________________________________________________

                            FUNCTION readNumberOfPointsInFile()
          Open the file and reads the first string, which is the number of points
          in the file, and the second string, that is the number of black points.
*/
void readNumberOfPointsInFile( char *fileName, int &N, int &NBlack ){
     char string[15];
     FILE *Arq;

     Arq = fopen(fileName, "r" );

     //lê o número total de pontos no arquivo
     fscanf(Arq, "%s", string);
     //converte o string lido no inteiro N
     N = atoi(string);

     //lê o número de pontos pretos no arquivo
     fscanf(Arq, "%s", string);
     //converte o string lido no inteiro NBlack
     NBlack = atoi(string);
     
     fclose(Arq);
}

/*______________________________________________________________________________

                            FUNCTION readFile()
          Opens the file, jumps the first string (number of points
          in the file) and the second string (number of black points
          in the file), then starts to get the coordinates of the points in the file.
          The file must have the following format:
              n
              nBlack
              1 x1 y1
              2 x2 y2
              ...
              n xn yn
          "n" is the number of points in the file
          "nBlack" is the number of black points in the file
          x1 and y1 are the coordinates of the first point 
          x2 and y2 are the coordinates of the second point
          ...
          xn and yn are the coordinates of the last point
          At the beginnig of each line it is the number of the point, which is also
          jumped during the reading.
          
          The first (n-nBlack) points MUST be the white points, and the following
          (n-Black+1) until (n) MUST be the black points.
          
          At the end, if there was any value out of the bounds (0 and 1),
          calls the function Converts with the values of the higher and the
          smaller x and y, which will do the conversion of the coordinates
          x and y, so they can be between 0 and 1. 
*/
void readFile( char *fileName, int Total, Table* bigTable){

     char string[25];
     FILE *Arq;
     int iBlack, iWhite, index, i;
     double maxX=0, maxY=0, minX=1000000, minY=1000000;
     double x, y;
     bool needsConversion=false;

     Arq = fopen(fileName, "r" );

     //pula o primeiro número do arquivo, que será o número total de pontos
     fscanf(Arq, "%s", string);
     //pula o segundo número do arquivo, que será o número total de pontos pretos
     fscanf(Arq, "%s", string);
     
     i=0;
     //percorre o arquivo para preencher as tabelas com os valores das coordenadas dos pontos
     while (!feof(Arq)){
           fscanf(Arq, "%s ", string);//pula o número na frente do ponto
           fscanf(Arq, "%s ", string);
           x = atof(string);
           fscanf(Arq, "%s ", string);
           y = atof(string);
           if (i >= Total){
                 cerr<<"ERROR on readFile(): vector of points is already full";
                 system("Pause");
           }           
           bigTable->getsXPoint()[i]=x;
           bigTable->getsYPoint()[i]=y;
           i++;  
              if (x > maxX) maxX = x;
              if (y > maxY) maxY = y;
              if (x < minX) minX = x;
              if (y < minY) minY = y;
              if (x>1 || x<0 || y>1 || y<0) needsConversion = true;           
     }
     fclose(Arq);
     if (needsConversion)
        converts(Total, bigTable->getsXPoint(), bigTable->getsYPoint(), maxX, maxY, minX, minY);
}

/*______________________________________________________________________________

                            FUNCTION converts()
          Converts the values of the coordinates x and y of each point, so that
          they can be between 0 and 1.
*/
void converts( int N, double *xTable, double *yTable, double maxX, double maxY,
               double minX, double minY){
     double delta;
     int i;
              
     delta = maxX - minX;
     if (delta < maxY - minY)
        delta = maxY - minY;
     
     for (i=0;i<N;i++){
         xTable[i] = (xTable[i] - minX)/delta;
         yTable[i] = (yTable[i] - minY)/delta;
     }
}
/*______________________________________________________________________________

                            FUNCTION createNewFile()
    Open the file "fileName" in mode "write", so we make sure that the file will
    be empty (because later we will need to write in that file, but using another
    mode to open the file).                              
*/
void createNewFile( char *fileName ){
     FILE *Arq = fopen(fileName, "w");
     fclose(Arq);
}

/*______________________________________________________________________________

                            FUNCTION writeFile()
     Writes in a file informations of all supergroups founded in iteration i.
     Each line of the file will have the info of one super-group, that is:
        iteration i | number of the supergroup | number of black points | ->
       ->  number of white points | area of the supergroup | number of cells |
       ->  intensity of the supergroup
*/
void writeFile( int i ){
   double area, auxArea, intense;
   int BP, WP, nCells, auxNCell, nGroups, j, it, k;
   int N = graph->getNumberOfSuperGroups();
   int *order;
   list<Group>::iterator group;
   list<Data>::iterator cell;
   FILE *Arq;
   
   Arq = fopen("arquivos/superGrupos.txt", "a");
   if (Arq == 0) cout<<"ERROR: file superGrupos.txt can not be open.";
   
   order = graph->sortInsertionIntensity();
   
   for ( it = N-1; it >= 0; it-- ){
      j = order[it];
      BP = graph->getSuperGroupAt(j)->getNumberOfBlackPoints();
      WP = graph->getSuperGroupAt(j)->getNumberOfWhitePoints();
      intense = graph->getSuperGroupAt(j)->getIntensity();
      nCells = 0;
      area = 0.0;      
      group = graph->getSuperGroupAt(j)->getGroupList()->begin();
      nGroups = graph->getSuperGroupAt(j)->getGroupList()->size();
      for ( k = 0; k < nGroups; k++ ){
          auxNCell = group->getCellList()->size();
          nCells = nCells + auxNCell;
          for ( cell=group->getCellList()->begin(); cell!=group->getCellList()->end(); ++cell ){
              auxArea = pow( cell->faceLength, 2 );
              area = area + auxArea;
          }
          group++;
      }
      fprintf(Arq, "%d, SG %d, BP %d, WP %d, Area %f, ", i, j, BP, WP, area);
      fprintf(Arq, "nCells %d, Scan Statistics %f\n", nCells, intense);
   }
   fprintf(Arq, "\n");
   
   fclose(Arq);
}

/*______________________________________________________________________________

                            FUNCTION originalBlackPoints()

      Returns a vector of integers (BLACKPOINTS) with the positions of the points
      that will be blackpoints. These positions are the "NBlack" last positions of
      the bigTable (in main.cpp), and are the original positions of the (real)
      blackpoints.
*/
int* originalBlackPoints( int TOTAL, int NBlack ){
     int *BLACKPOINTS, i, index;

     BLACKPOINTS = new int[NBlack]; 
     i = 0;    
     for ( index=(TOTAL-NBlack); index<TOTAL; index++ ){
         BLACKPOINTS[i] = index;
         i++;
     }
     return BLACKPOINTS;
}


/*______________________________________________________________________________

                            FUNCTION randomBlackPoints()

      Returns a vector of integers (BLACKPOINTS) with the positions of the points
      that will be blackpoints. These positions refer to bigTable (in main.cpp)
      and they are get randomically.
*/
int* randomBlackPoints( int *HIGHEST, int TOTAL, bool &END, int i, int NBlack ){
     int *BLACKPOINTS;
     int index, j;
     
     if (i==ITERACOES){
        END=true;
        BLACKPOINTS = HIGHEST;
     }
     else {            
         BLACKPOINTS = new int[NBlack];
         for ( j=0; j<NBlack; j++ ){
             index = rand()%TOTAL;
             while ( indexAlreadyExists(index, j, BLACKPOINTS) )
                  index = rand()%TOTAL;
             BLACKPOINTS[j] = index;
         }
     }
     return BLACKPOINTS;
}
            
/*______________________________________________________________________________

                            FUNCTION dividePoints()
                            
     Divide the points x and y of a bigTable between two tables, whiteTable and
     blackTable, in some way that in the end will exist 2 types of points, the blacks
     and the whites.
     The vector BLACKPOINTS has the information that decides if a point of bigTable
     is black or white.
*/
void dividePoints(Table *bigTable,Table *blackTable,Table *whiteTable, int *BLACKPOINTS){
     int nblack, nwhite, total;
     int i=0, iBlack=0, iWhite=0;

     nblack = blackTable->getNumberOfPoints();
     nwhite = whiteTable->getNumberOfPoints();
     total = bigTable->getNumberOfPoints();
     while ( i < total ){
           if ( indexAlreadyExists( i, nblack, BLACKPOINTS) ){
              if (iBlack >= nblack){
                 cerr<<"ERROR on readFile(): vector with blackpoints is already full";
                 exit(1);
              }
              blackTable->getsXPoint()[iBlack] = bigTable->getsXPoint()[i];
              blackTable->getsYPoint()[iBlack] = bigTable->getsYPoint()[i];
              iBlack++;
           }
           else {
              if (iWhite >= nwhite){
                 cerr<<"ERROR on readFile(): vector with whitepoints is already full";
                 exit(1);
              }
              whiteTable->getsXPoint()[iWhite] = bigTable->getsXPoint()[i];
              whiteTable->getsYPoint()[iWhite] = bigTable->getsYPoint()[i];
              iWhite++;
           }     
           i++;
     }
     if (iBlack != nblack){
        cerr<<"ERROR on readFile(): vector with blackpoints is not full";
        system("Pause");
     }
     if (iWhite != nwhite){
        cerr<<"ERROR on readFile(): vector with whitepoints is not full";
        system("Pause");
     }
}


/* 
   Retorna true se alguma posição de vector (mas no máximo a i-ésima posição)
   armazena o valor "index".
*/
bool indexAlreadyExists( int index, int i, int *vector){
     if (i==0)return false;
     for (int j=0; j<i; j++){
         if (vector[j] == index ) return true;
     }
     return false;
}

/* 
     Initializes the vectors mediaMaxBlack and mediaScan with zero.
*/
void initMediaStatistics( double mediaMaxBlack[N_STATISTICS], double mediaScan[N_STATISTICS], int invalidos[N_STATISTICS] ){
     for ( int i=0; i<N_STATISTICS; i++ ){
         mediaMaxBlack[i] = 0.0;
         mediaScan[i] = 0.0;
         invalidos[i] = 0;
     }
}

void atualQuantilStatistics( int &quantilMaxBlack, double &quantilScan, double scan[ITERACOES], int nMaxBlackInSG[ITERACOES]){
     double qMax[ITERACOES];
     double qScan[ITERACOES];
     
     for (int i=0; i<ITERACOES; i++){
         qMax[i] =  (double)nMaxBlackInSG[i];
         qScan[i] = (double)scan[i];
     }

     quickSort(qScan, 0, ITERACOES-1);
     quickSort(qMax, 0, ITERACOES-1);

     int at = (int) (0.95*ITERACOES);
     quantilMaxBlack = (int) qMax[at];
     quantilScan = qScan[at];
     
}


void atualMediaStatistics( double &mediaMaxBlack, double &mediaScan, double scan[ITERACOES], int nMaxBlackInSG[ITERACOES] ){
     for ( int i=0; i<ITERACOES; i++ ){
         if ( scan[i] != -1 && nMaxBlackInSG[i] != -1 ){
            mediaMaxBlack = mediaMaxBlack + nMaxBlackInSG[i];
            mediaScan = mediaScan + scan[i];
         }
     }
     mediaMaxBlack /= ITERACOES;
     mediaScan /= ITERACOES;     
}

void printStatistics( double mediaMaxBlack[N_STATISTICS], double mediaScan[N_STATISTICS], int invalidos[N_STATISTICS] ){
     double MAX, SUPERMAX;
     int i;
     FILE *Arq;

     Arq = fopen("arquivos/statistics.txt","w");     
     fprintf(Arq, "\n(MAX,SUPERMAX)  \tMedia do maximo de pontos pretos por SG\t\tMedia da estatistica scan\t Numero de iteracoes invalidas");

     SUPERMAX = initSM;
     MAX = RAZAOMaxSuperMax * SUPERMAX;
     i = 0;
     for ( int k=1; k<=N_STATISTICS; k++ ){
         fprintf(Arq, "\n(%f,%f): \t\t%f \t\t\t\t\t%f \t\t\t%d", MAX, SUPERMAX, mediaMaxBlack[i], mediaScan[i], invalidos[i]);
         SUPERMAX *= incremento;
         MAX = RAZAOMaxSuperMax * SUPERMAX;
         i++;
     }
     fclose(Arq);
}

/* _____________________________________________________________________________
                             
                               ORDENAÇÃO
   ___________________________________________________________________________*/

void swap(double &a, double &b) {
  double tmp;
  tmp = a;
  a = b;
  b = tmp;
}
 
int partition(double vec[], int left, int right) {
  int i, j;
 
  i = left;
  for (j = left + 1; j <= right; ++j) {
    if (vec[j] < vec[left]) {
      ++i;
      swap(vec[i], vec[j]);
    }
  }
  swap(vec[left], vec[i]);
 
  return i;
}
 
void quickSort(double vec[], int left, int right) {
  int r;
 
  if (right > left) {
    r = partition(vec, left, right);
    quickSort(vec, left, r - 1);
    quickSort(vec, r + 1, right);
  }
}

/* _____________________________________________________________________________
                             
                               ARQUIVOS E IMPRESSÕES
   ___________________________________________________________________________*/

/*
     Imprime um arquivo-texto com o formato:
             Para cada iteração i do algoritmo, imprime o numero maximo de pontos
             pretos pertencentes a algum super-grupo, e a estatistica scan do
             respectivo super-grupo.
             O arquivo resultante com uma linha para cada iteração, resultando
             em 'ITERACOES' linhas.
*/
void printMaxBlackPoints( int nMaxBlackInSG[ITERACOES], double scan[ITERACOES], double MAX, double SUPERMAX,
                          int k, double intensity, double original, int counter){
     FILE *Arq;
     
     switch (k){
            case(1):{
                Arq = fopen("arquivos/maxBlackPoints01.txt","w");
                break;
            }
            case(2):{
                Arq = fopen("arquivos/maxBlackPoints02.txt","w");
                break;
            }
            case(3):{
                Arq = fopen("arquivos/maxBlackPoints03.txt","w");
                break;
            }
            case(4):{
                Arq = fopen("arquivos/maxBlackPoints04.txt","w");
                break;
            }
            case(5):{
                Arq = fopen("arquivos/maxBlackPoints05.txt","w");
                break;
            }
            case(6):{
                Arq = fopen("arquivos/maxBlackPoints06.txt","w");
                break;
            }
            case(7):{
                Arq = fopen("arquivos/maxBlackPoints07.txt","w");
                break;
            }
            case(8):{
                Arq = fopen("arquivos/maxBlackPoints08.txt","w");
                break;
            }
            case(9):{
                Arq = fopen("arquivos/maxBlackPoints09.txt","w");
                break;
            }
            case(10):{
                Arq = fopen("arquivos/maxBlackPoints10.txt","w");
                break;
            }
     }
     
     int maxBlack=0, n, size;
     int *histogrBlack, *histogrScan;
     double maxScan=0;

     fprintf(Arq, "MAX = %f, SUPERMAX = %f", MAX, SUPERMAX);
     fprintf(Arq, "\n\nMaior intensidade encontrada dentre todas as %d iteracoes = %f", ITERACOES, intensity);
     if ( original != -1 ){
        fprintf(Arq, "\nMaior intensidade encontrada nos dados originais = %f",original);
        fprintf(Arq, "\nNumero de vezes que a maior intensidade de uma iteracao foi maior que a original: %d", counter );
     }
     else
         fprintf(Arq, "\nNao houve formacao de grupos nos dados originais.");

     fprintf(Arq, "\n\nIteracao i   Numero maximo de pontos\t  Estatistica scan\n\t\tpretos dentro de SG");
     for ( int i=0; i<ITERACOES; i++ ){
         
         if ( scan[i] != -1 && nMaxBlackInSG[i] != -1 ){
            fprintf(Arq, "\n%d: \t\t\t%d, \t\t\t%f", i, nMaxBlackInSG[i], scan[i]);
            if ( nMaxBlackInSG[i] > maxBlack )
               maxBlack = nMaxBlackInSG[i];
            if ( scan[i] > maxScan )
               maxScan = scan[i];
         }
         else
             fprintf(Arq, "\n%d:    Nao houve formacao de grupos.", i);

     }
     size = int(maxScan/0.05);
     histogrBlack = new int[maxBlack+1];
     histogrScan = new int[size];
     for ( int i=1; i<=maxBlack; i++ ){
         n=0;
         for ( int j=0; j<ITERACOES; j++ ){
             if ( nMaxBlackInSG[j] == i )
                n++;
         }
         histogrBlack[i] = n;
     }
     for ( int i=0; i < size; i++ ){
         n=0;
         for ( int j=0; j<ITERACOES; j++ ){
             if ( i*0.05 <= scan[j] && scan[j] < (i+1)*0.05 )
                n++;
             if ( i == size - 1 ){
                if ( scan[j] >= (i+1)*0.05 )
                   n++;
             }
         }
         histogrScan[i] = n;
     }
     
     fprintf(Arq, "\n\n\nDados para histograma de maximo de pontos pretos:");
     fprintf(Arq, "\nNumero maximo de pontos pretos\t\tFrequencia de ocorrencia");
     for ( int i=1; i<=maxBlack; i++ ){
         fprintf(Arq, "\n\t%d  \t\t\t\t\t%d", i, histogrBlack[i]);
     }

     fprintf(Arq, "\n\n\nDados para histograma de estatistica scan:");
     fprintf(Arq, "\nEstatistica Scan dos SGs com máximo de pontos pretos\t\tFrequencia de ocorrencia");
     for ( int i=0; i<=size-2; i++ ){
         fprintf(Arq, "\n\tEntre %f e %f \t\t\t\t\t%d", i*0.05, (i+1)*0.05, histogrScan[i]);
     }
     fprintf(Arq, "\n\tEntre %f e %f \t\t\t\t\t%d", (size-1)*0.05, maxScan, histogrScan[size-1]);
     
     delete histogrScan;
     delete histogrBlack;
          
     fclose(Arq);
}

/* ___________________________________________________________________________________

                        FUNCTION chooseTypeOfShift()

         Choose the type of shift that will be made.
   ___________________________________________________________________________________*/
void chooseTypeOfShift(int &shiftType, int &rotateType){
    
    cout<<"Existem 36 instancias diferentes para visualizacao, as quais dependem do "
          "deslocamento de pontos retilíneo e rotacional escolhido.";
    cout<<"\n\nEscolha primeiro o tipo de deslocamento retilineo da instancia a ser visualizada:";
    cout<<"\n0, para usar as coordenadas sem deslocamentos";
    cout<<"\n1, para usar as coordenadas deslocadas a norte";
    cout<<"\n2, para usar as coordenadas deslocadas a noroeste";    
    cout<<"\n3, para usar as coordenadas deslocadas a oeste";
    cout<<"\n4, para usar as coordenadas deslocadas a sudoeste";
    cout<<"\n5, para usar as coordenadas deslocadas a sul";
    cout<<"\n6, para usar as coordenadas deslocadas a sudeste";                     
    cout<<"\n7, para usar as coordenadas deslocadas a leste";         
    cout<<"\n8, para usar as coordenadas deslocadas a nordeste";    
    cout<<"\n\n(Para opcoes invalidas sera adotado o caso padrao, 0)";
    cout<<"\nOpcao: ";
    cin>>shiftType;
    if ( shiftType!=0 && shiftType!=1 && shiftType!=2 && shiftType!=3 && shiftType!=4
         && shiftType!=5 && shiftType!=6 && shiftType!=7 && shiftType!=8 )
         shiftType=0;
    
    cout<<"\n\nEscolha agora o tipo de deslocamento rotacional (sentido anti-horario)"
          " da instancia a ser visualizada:";
    cout<<"\n0, para usar as coordenadas dos pontos sem rotacao ( 0 graus )";
    cout<<"\n1, para usar as coordenadas rotacionadas 90 graus ";
    cout<<"\n2, para usar as coordenadas rotacionadas 180 graus ";    
    cout<<"\n3, para usar as coordenadas rotacionadas 270 graus ";   
    cout<<"\n\n(Para opcoes invalidas sera adotado o caso padrao, 0)";
    cout<<"\nOpcao: ";    
    cin>>rotateType;
    if ( rotateType!=0 && rotateType!=1 && rotateType!=2 && rotateType!=3 )
         rotateType = 0;

    cout<<"\n\nAs opcoes de desenho escolhidas foram "<<shiftType<<" para deslocamento"
          " retilineo e "<<rotateType<<" para rotacao.";
}



/* _____________________________________________________________________________
                         FUNCTION Print()
                         
      Cria os arquivos-texto com as informações da Matrix, das celulas, do grafo
      de supergrupos e das tabelas.
   _____________________________________________________________________________ */
   
void Print(){
     
     grid->PrintMatrix("arquivos/OUTPUT/Matrix.txt");
     grid->PrintCellsSuperGroups("arquivos/OUTPUT/CellSuperGroups.txt");
     graph->print("arquivos/OUTPUT/SuperGroups.txt", blackTable, whiteTable);
     blackTable->createFile("arquivos/OUTPUT/BlackTable.txt");
     whiteTable->createFile("arquivos/OUTPUT/WhiteTable.txt");
     grid->printCells("arquivos/OUTPUT/Cells.txt", blackTable);
     graph->printSummarySuperGroups("arquivos/OUTPUT/Summary_SuperGroups.txt");
}

void instrucoes()
{
    cout <<"\n\nPara dar mais zoom:   = ou + "
         <<"\nPara dar menos zoom:  _ ou - "
         <<"\nPara colocar ou retirar o desenho da Curva de Hilbert: h ou H "
         <<"\nPara colocar ou retirar o desenho da BlackCurve: 2 "
         <<"\nPara colocar ou retirar o desenho do Grid: g ou G "
         <<"\nPara colocar ou retirar o desenho dos pontos pretos: b ou B "
         <<"\nPara colocar ou retirar o desenho dos pontos brancos: w ou W "
         <<"\nPara colocar ou retirar o desenho das linhas que formam grupos: l ou L "
         <<"\nPara colocar ou retirar o preenchimento do fundo das celulas pertencentes a grupos: c ou C "
         <<"\nPara colocar ou retirar o numero dos supergrupos de cada célula: n ou N "
         <<"\nPara sair do programa: ESC.";
}

