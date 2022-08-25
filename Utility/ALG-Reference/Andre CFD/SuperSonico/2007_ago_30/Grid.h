/* Grid.h
Definition of class Grid.
An object Grid represents a square grid. This class is a singleton.
*/

#ifndef GRID_H
#define GRID_H

#include "Cell.h"
#include "constants.h"

class Grid {
    public:

        Grid();	    // constructor
        ~Grid();    // destructor

        Cell *firstGridCell;
        double sideLength;	    // Length of square grid. Default = 1.0.
        int numberOfCells;      // Number of cells of grid.

        //Variaveis para a solucao do escoamento supersonico
        double deltaT,          // time increment
               tempoAtual;

        // Variaveis de configuracao
        int refinamentoMaximo,
            refinamentoMinimo;

        void drawSurfaceGrid();
        void initializeGrid( bool );
        void refineGridInitially( int );
        void drawHilbertCurve( int );
        void orderGridCells();
        void drawPeanoCurve( int );

        //Print functions.
        int print();

        //Flag for helping debug logical errors.
        bool globalFlag;

        // Fluxos totais de massa
        double M_norte, M_sul, M_leste, M_oeste;

        double residuosP[MAXITER_GRAFICO_RESIDUOS];
        double residuosU[MAXITER_GRAFICO_RESIDUOS];
        double residuosV[MAXITER_GRAFICO_RESIDUOS];
        double residuosE[MAXITER_GRAFICO_RESIDUOS];
        double residuosM[MAXITER_GRAFICO_RESIDUOS];
        double maiorResiduo;
        double menorResiduo;

};//end class Grid

#endif
