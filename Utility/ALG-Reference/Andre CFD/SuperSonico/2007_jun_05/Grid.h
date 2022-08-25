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

        // Entrada e saida totais de massa
        double entradaMassa,
               saidaMassa;

        double residuosP[MAXITER_GRAFICO_RESIDUOS];
        double residuosVel[MAXITER_GRAFICO_RESIDUOS];
        double residuosT[MAXITER_GRAFICO_RESIDUOS];
        double maiorResiduo;

};//end class Grid

#endif
