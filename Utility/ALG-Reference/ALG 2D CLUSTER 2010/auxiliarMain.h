#ifndef AUXILIARMAIN_H
#define AUXILIARMAIN_H
#include "table.h"

/* constante refinementBound = 1 indica que cada célula deve ser refinada até que
contenha no máximo 1 ponto (gridcell->u <= 1) */
#define refinementBound 1
#define refinementLevel1 10 /* Nível máximo de refinamento da célula para critério 1*/
#define refinementLevel2 6 /* Nível máximo de refinamento da célula para critério 2*/

// Number of random iterations
#define ITERACOES 10

// number of iterations to compute the average
#define N_STATISTICS 10

// Constants used to calculate the value of MAX and SUPERMAX
#define RAZAOMaxSuperMax 1
#define incremento  1.4
#define initSM  0.005

void simulation();
void draw_DATA( int color );
void draw_MAX_SUPERMAX();

void readMaxAndSupermax( char *fileName, double &MAX, double &SUPERMAX);
void readNumberOfPointsInFile( char *fileName, int &N, int &NBlack );
void readFile( char *fileName, int Total, Table* bigTable);
void converts( int N,double *xTable,double *yTable,double maxX,double maxY,double minX,double minY);
int* originalBlackPoints( int TOTAL, int NBlack );
int* randomBlackPoints( int *HIGHEST, int TOTAL, bool &END, int i, int NBlack );
void dividePoints(Table *bigTable,Table *blackTable,Table *whiteTable,int *BLACKPOINTS);
void createNewFile( char *fileName );
void writeFile( int i );
float rnd(long idum0);
double fx();
double fy();
bool indexAlreadyExists( int index, int i, int *vector);

//ESTATÍSTICAS
void initMediaStatistics( double mediaMaxBlack[N_STATISTICS], double mediaScan[N_STATISTICS], int invalidos[N_STATISTICS] );
void atualQuantilStatistics( int &quantilMaxBlack, double &quantilScan, double scan[ITERACOES], int nMaxBlackInSG[ITERACOES]);
void atualMediaStatistics( double &mediaMaxBlack, double &mediaScan, double scan[ITERACOES], int nMaxBlackInSG[ITERACOES] );

//ORDENAÇÃO
void swap(double &a, double &b);
int partition(double vec[], int left, int right);
void quickSort(double vec[], int left, int right);

//ARQUIVOS E IMPRESSÕES
void printStatistics( double mediaMaxBlack[N_STATISTICS], double mediaScan[N_STATISTICS], int invalidos[N_STATISTICS] );
void printMaxBlackPoints( int nMaxBlackInSG[ITERACOES], double scan[ITERACOES], double MAX, double SUPERMAX, int k, double intensity, double original, int counter );
void chooseTypeOfShift(int &, int &);
void Print();
void instrucoes();

#endif
