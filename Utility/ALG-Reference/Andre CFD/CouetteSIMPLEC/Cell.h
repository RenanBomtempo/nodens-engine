/* Cell.h
   Definition of class Cell.
*/

#ifndef CELL_H
#define CELL_H

#include "Element.h"

class Element;

class Cell
{
    public:
        
        Cell();			 //constructor
        ~Cell();		 //destructor
//______________________________________________________________________________
//Data structure.

        char type;       /* 'b' = black (cell node)
		                    'w' = white (transition cell node) */
        bool active;     // Indicates if cell is active (true) or inactive (false).
        int level;       // Refinement level (initial level = 0).
        
        //Pointers that only cell nodes use:
        int bunchNumber; /* Bunch identifier (cells derived from same father 
                                              have same bunchNumber).*/        
        Cell *north;     // Points to cell or transition node above this cell.
        Cell *south;     // Points to cell or transition node below this cell.
        Cell *east;      // Points to cell or transition node rightward of this cell.
        Cell *west;      // Points to cell or transition node leftward of this cell.

        Cell *previous;  // Previous cell of the grid in the Hilbert curve ordering.
        Cell *next;      // Next cell of the grid in the Hilbert curve ordering.

        //Pointers that only transition nodes use:
        Cell *singleConnector;
        Cell *doubleConnector1;
        Cell *doubleConnector2;

//______________________________________________________________________________
//Grid ordering through Hilbert curve.

        int gridPosition;   /* Indicates position of cell on grid according to
		                       ordering provided by the modified Hilbert curve. */ 
        int hilbertShapeNumber; /* Variable used to compute the form of the 
		                           Hilbert curve in the bunch created when this  
								   cell is refined. */

//______________________________________________________________________________
//Cell geometry.

        double centerX;
        double centerY;
        double faceLength;
        double halfFaceLength;

//______________________________________________________________________________
//Variables used in solving the Navier-Stokes equations.

    // Variaveis de fluido nesta celula ----------------
    double rhoAtual,
           rhoAnterior,
           rhoIterAnterior;

    double uAtual,
           uAnterior,
           uIterCAnterior,
           uIterBAnterior;

    double vAtual,
           vAnterior,
           vIterCAnterior,
           vIterBAnterior;

    double EtAtual,
           EtAnterior,
           EtIterAnterior;
    
    double TAtual,
           TAnterior,
           TIterAnterior;

    double pAtual,
           pAnterior, //????
           pIterCAnterior,
           pIterBAnterior,
           pLinha;

    double miAtual,
           miAnterior,
           miIterAnterior;

    double kAtual,
           kAnterior, //????
           kIterAnterior; //????

    // Variaveis de fluido nas interfaces ----------------
    // Atualizado em 23/04/07 por Andre Staltz
    double northFaceRho,
           southFaceRho,
           eastFaceRho,
           westFaceRho;

    double eastFaceU,
           westFaceU,
           northFaceU, // usado so' para calcular du/dx
           southFaceU; // usado so' para calcular du/dx

    double northFaceV,
           southFaceV,
           eastFaceV, // usado so' para calcular dv/dy
           westFaceV; // usado so' para calcular dv/dy

    double northFaceEt,
           southFaceEt,
           eastFaceEt,
           westFaceEt;

    double northFaceT,
           southFaceT,
           eastFaceT,
           westFaceT;

    double northFaceP,
           southFaceP,
           eastFaceP,
           westFaceP;

    double northFaceMi,
           southFaceMi,
           eastFaceMi,
           westFaceMi;

    double northFaceK,
           southFaceK,
           eastFaceK,
           westFaceK;

    
    // Variaveis de interpolacao nas interfaces

    double northFaceGamma,
           southFaceGamma,
           eastFaceGamma,
           westFaceGamma;

    double northFaceBeta,
           southFaceBeta,
           eastFaceBeta,
           westFaceBeta;

    // Variaveis usadas no SIMPLEC
    double I_n,
           I_s,
           I_e,
           I_w;

    double d_n,
           d_s,
           d_e,
           d_w;

//______________________________________________________________________________
/* Variaveis de resolucao dos sistemas Ax = b, de pressao', momentos e energia*/

    // Matrizes b
        double b_MomentoX;   /* Corresponde ao elemento dessa celula no b */
        double b_MomentoY;   
        double b_Pressao;   
        double b_Energia;   

        double Ax;  /* Element of vector Ax = b associated to this cell. Also plays the role of Ap.*/
        double r;   /* Element of the vector r = b - Ax associated to this cell. */
        double p;   /* Element of the search direction vector in the conjugate gradient algorithm. */
        double p1;  /* p's upgrade in the conjugate gradient algorithm. */

// Variaveis utilizadas no gradiente biconjugado estabilizado 
        double Ap;
        double r_;
        double As;
        double s;

//______________________________________________________________________________

//Print operators.

		void print();
		void printTransitionNode();
		void printVariable();
		void printBunch();

// Lista encadeada das matrizes de resolucao (para SIMPLEC, dia 23/04/07)

        void iniciaElement( double valor, Element **escolheElementEquacao );
        void insereElement( double valor, Cell *celulaDesteElement, Element **escolheElementEquacao ); 

        Element *firstElementMomentoX,
                *firstElementMomentoY,
                *firstElementPressao,
                *firstElementEnergia;

};//end class Cell

#endif
