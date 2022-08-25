//functions.h
  
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Cell.h"
#include "Grid.h"
#include "stdio.h"

/* _____________________________________________________________________________
								graphics.cpp 
______________________________________________________________________________*/

    void setColor( int );    
    void drawLine( double, double, double, double, double, double );
    void drawString(const char* string, void* fonte, double, double, double);  
 
/* _____________________________________________________________________________
								refinement.cpp 
______________________________________________________________________________*/

    void refineGrid( Grid *, int, double );
	void refineCell( Cell * );
    int computeHilbertShapeNumber( int, int );
	void simplify( Cell * );
	void drawHilbertCurveLocally( Cell *, int, bool );
    
/* _____________________________________________________________________________
								derefinement.cpp 
______________________________________________________________________________*/
	
    void derefineGrid( Grid *, double, bool );
    void derefineBunch( Cell *, bool );
    bool needsDerefinement( Cell *, double );
	
/* _____________________________________________________________________________
								interpolacao.cpp 
______________________________________________________________________________*/
    double calcula_Peclet( Cell *, char );
    int atualiza_Gamma( Grid * );
    int atualiza_Beta( Grid * );
    double interpolaVariavelFace( Cell *, short, char );
    void interpolaTodasInterfaces( Grid *, char, bool );

/* _____________________________________________________________________________
								render.cpp 
______________________________________________________________________________*/
    void desenharMalha( Grid * );
    void desenharDensidade( Grid * );
    void desenharTemperatura( Grid * );
    void desenharVelocidade( Grid *, double );
    void desenharPressao( Grid * );
    void desenharTempo( Grid *, double, double, double );
    void desenharPausa( Grid *, double, double, double );
    void desenharValores( Grid *, int);
    void desenharLegenda( Grid *, double, double, double, int );
    void desenharPlacaPlana( Grid * );

/* _____________________________________________________________________________
								solve.cpp 
______________________________________________________________________________*/    
    void solveSuperSonicSIMPLEC( Grid * );
    int condicaoInicial( Grid * );
    int calculaCoeficientesMomentoX( Grid * );
    int calculaCoeficientesMomentoY( Grid * );
    int calculaVelocidadesNasFaces( Grid * );
    int calculaCoeficientesCorrecaoPressao( Grid * );
    int calculaTermoFonteCorrecaoPressao( Grid * );
    int corrige_Velocidades_Densidades_Pressoes( Grid * );
    int calculaCoeficientesEnergia( Grid * );
    int obtemVariaveisVizinhas( Cell *, short, double *, double *, double *, double * );
    int atualizaParametrosDaIteracaoAnterior( Grid *, char );
    int atualizaParametrosDoTempoAnterior( Grid * );
    int convergenciaPressao( Grid * );
    int convergenciaVelocidades( Grid * );
    int convergenciaTemperatura( Grid * );
    int atualizaOutrosEscalares( Grid * );
    int gradienteBiconjugadoEstabilizado( Grid *, short );
    int gradienteConjugado( Grid *, short );

	void verificaResultado( Grid * );
    void imprimeMatriz( Grid *, short );

/* _____________________________________________________________________________
								fronteira.cpp 
______________________________________________________________________________*/ 
    double condicaoFronteira( Cell *, short, char, bool );

/* _____________________________________________________________________________
								derivadas.cpp 
______________________________________________________________________________*/
    double dudx( Cell * , char );
    double dudy( Cell * , char );
    double dvdx( Cell * , char );
    double dvdy( Cell * , char ); 
    double dTdx( Cell * , char );
    double dTdy( Cell * , char );
    
#endif
