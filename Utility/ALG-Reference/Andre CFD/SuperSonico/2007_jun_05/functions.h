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
    void drawTriangle( double, double, double, double, double, double, double, double, double );
 
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
    void desenharPlacasPlanas( Grid * );
    void desenharResiduo( Grid *, int );

/* _____________________________________________________________________________
								solve.cpp 
______________________________________________________________________________*/    
    void solveSuperSonicSIMPLEC( Grid * );
    int condicaoInicial( Grid * );
    int calculaEntradaSaidaMassa( Grid * );
    int calculaCoeficientesMomentoX( Grid * );
    int calculaTermoFonteMomentoX( Grid * );
    int calculaCoeficientesMomentoY( Grid * );
    int calculaTermoFonteMomentoY( Grid * );
    int calculaVelocidadesNasFaces( Grid * );
    int calculaCoeficientesCorrecaoPressao( Grid *, double, double );
    int calculaTermoFonteCorrecaoPressao( Grid * );
    int corrige_Velocidades_Densidades_Pressoes( Grid *, double, double, double );
    int calculaCoeficientesEnergia( Grid * );
    int atualizaOutrosEscalares( Grid *, double );
    int obtemVariaveisVizinhas( Cell *, short, double *, double *, double *, double * );
    int atualizaParametrosDaIteracaoAnterior( Grid *, char );
    int atualizaParametrosDoTempoAnterior( Grid * );
    int convergenciaPressao( Grid * );
    int convergenciaVelocidades( Grid * );
    int convergenciaTemperatura( Grid * );
    int gradienteBiconjugadoEstabilizado( Grid *, short );
    int gradienteConjugado( Grid *, short );
    void calculaResiduos( Grid *, int );

    void imprimeMatriz( Grid *, short );

/* _____________________________________________________________________________
                                fronteira.cpp 
______________________________________________________________________________*/ 
    double valorCondicaoFronteira( Cell *, short, char );
    void definirCelulasFronteira( Grid * );
    short tipoCondicaoFronteira( Cell *, short );

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
