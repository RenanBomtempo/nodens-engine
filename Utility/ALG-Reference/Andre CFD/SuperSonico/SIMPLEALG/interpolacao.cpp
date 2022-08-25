/*  ========== interpolacao.cpp =========
    Contem fincaoes para atualizar as variaveis de interpolacao.
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>

#ifndef IOSTREAM__INCLUIDO
#define IOSTREAM__INCLUIDO
#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
    using std::cin;
#endif // IOSTREAM_INCLUIDO

#include "constants.h"
#include "erros.h"

/*______________________________________________________________________________

                              calcula_Peclet()
    Retorna o numero de Peclet em uma dada face norte de uma celula.

Atualizado por Henrique dia 25/04/07
______________________________________________________________________________*/
double calcula_Peclet( Cell *celula, char direcao )
{
    switch( direcao )
    {
        case 'n':
            return ( celula->northFaceRho * celula->northFaceV * celula->faceLength ) / celula->northFaceMi;

        case 's':
            return ( celula->southFaceRho * celula->southFaceV * celula->faceLength ) / celula->southFaceMi;

        case 'e':
            return ( celula->eastFaceRho * celula->eastFaceU * celula->faceLength ) / celula->eastFaceMi;

        case 'w':
            return ( celula->westFaceRho * celula->westFaceU * celula->faceLength ) / celula->westFaceMi;

        default:
            IMPRIMIR_ERRO_DIRECAO_INVALIDA();
            return -1.0;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao calcula_Peclet()";
    return -1.0; 
}

/*______________________________________________________________________________

                              atualiza_Gamma()
    Calcula os novos valores de gamma nas interfaces para cada celula da malha.

Criado por Henrique dia 23/04/07
______________________________________________________________________________*/
int atualiza_Gamma( Grid *malha )
{
    Cell *celula;

    // Numeros de Peclet nas interfaces
    double Pe_n,
           Pe_s,
           Pe_e,
           Pe_w;

    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            
            // Calculo de gamma segundo o esquema WUDS
            Pe_n = calcula_Peclet( celula, 'n' );
            Pe_s = calcula_Peclet( celula, 's' );
            Pe_e = calcula_Peclet( celula, 'e' );
            Pe_w = calcula_Peclet( celula, 'w' );

            celula->northFaceGamma = AO_QUADRADO( Pe_n ) / ( 10.0 + 2*AO_QUADRADO( Pe_n ) );
            celula->southFaceGamma = AO_QUADRADO( Pe_s ) / ( 10.0 + 2*AO_QUADRADO( Pe_s ) );
            celula->eastFaceGamma = AO_QUADRADO( Pe_e ) / ( 10.0 + 2*AO_QUADRADO( Pe_e ) );
            celula->westFaceGamma = AO_QUADRADO( Pe_w ) / ( 10.0 + 2*AO_QUADRADO( Pe_w ) );
            

            // Calculo de gamma segundo o esquema de diferencas centradas
            //celula->northFaceGamma = 0.0;
            //celula->southFaceGamma = 0.0;
            //celula->eastFaceGamma = 0.0;
            //celula->westFaceGamma = 0.0;
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao atauliza_Gamma()";
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                              atualiza_Beta()
    Calcula os novos valores de beta nas interfaces para cada celula da malha.

Criado por Henrique dia 23/04/07
______________________________________________________________________________*/
int atualiza_Beta( Grid *malha )
{
    Cell *celula;

    // Numeros de Peclet nas interfaces
    double Pe_n,
           Pe_s,
           Pe_e,
           Pe_w;

    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            
            // Calculo de  beta segundo o esquema WUDS
            Pe_n = calcula_Peclet( celula, 'n' );
            Pe_s = calcula_Peclet( celula, 's' );
            Pe_e = calcula_Peclet( celula, 'e' );
            Pe_w = calcula_Peclet( celula, 'w' );

            celula->northFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_n ) ) / ( 1.0 + 0.05*AO_QUADRADO( Pe_n ) );
            celula->southFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_s ) ) / ( 1.0 + 0.05*AO_QUADRADO( Pe_s ) );
            celula->eastFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_e ) ) / ( 1.0 + 0.05*AO_QUADRADO( Pe_e ) );
            celula->westFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_w ) ) / ( 1.0 + 0.05*AO_QUADRADO( Pe_w ) );
            

            // Calculo de beta segundo o esquema de diferencas centradas
            //celula->northFaceBeta = 1.0;
            //celula->southFaceBeta = 1.0;
            //celula->eastFaceBeta = 1.0;
            //celula->westFaceBeta = 1.0;
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao atauliza_Beta()";
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________
                            
                          interpolaVariavelFace()
                            
    Retorna o valor de uma variavel escalar de acordo com a funcao de 
interpolacao expressa em gamma (das faces).
    
    Parametros:
         celula: a celula atual
         id_variavel: um identificador para saber qual variavel esta sendo
                      solicitada;
         direcao: indica qual a face solicitada (norte, sul, leste, oeste);

Criado por Andre e Fernando dia 24/04/07
______________________________________________________________________________*/

double interpolaVariavelFace( Cell *celula,
                          short id_variavel, 
                          char direcao )
{
    // Verificacao de erro de celula inexistente
    if( celula == NULL )
    {
        cout << "Em interpolaVariavelFace():\n";
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;    
    }

    // Verificacao de erro de direcao inva'lida
    if( direcao != 'n'
    &&  direcao != 's'
    &&  direcao != 'e'
    &&  direcao != 'w' )
    {
        IMPRIMIR_ERRO_FACE_INVALIDA();
        return 0.0;
    }

    // Verificar qual variavel foi solicitada na fronteira
    switch( id_variavel )
    {
        case ID_VAR_RHO: // ===================== RHO =========================
            switch( direcao )
            {
                case 'n': // Rho no norte -------------------------------------
                    if( celula->north->type == 'b' )
                        return (
                                  (0.5 + celula->northFaceGamma) * (celula->rhoAtual)
                                + (0.5 - celula->northFaceGamma) * (celula->north->rhoAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 's': // Rho no sul ---------------------------------------
                    if( celula->south->type == 'b' )
                        return (
                                  (0.5 + celula->southFaceGamma) * (celula->south->rhoAtual)
                                + (0.5 - celula->southFaceGamma) * (celula->rhoAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;
                
                case 'e': // Rho no leste -------------------------------------
                    if( celula->east->type == 'b' )
                        return (
                                  (0.5 + celula->eastFaceGamma) * (celula->rhoAtual)
                                + (0.5 - celula->eastFaceGamma) * (celula->east->rhoAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break; 

                case 'w': // Rho no oeste -------------------------------------
                    if( celula->west->type == 'b' )
                        return (
                                  (0.5 + celula->westFaceGamma) * (celula->west->rhoAtual)
                                + (0.5 - celula->westFaceGamma) * (celula->rhoAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;
            }
        break;

        case ID_VAR_U: // =================== Velocidade U =====================
            switch( direcao )
            {
                case 'n': // Velocidade U no norte -----------------------------
                    if( celula->north->type == 'b' )
                        return (
                                  (0.5 + celula->northFaceGamma) * (celula->uAtual)
                                + (0.5 - celula->northFaceGamma) * (celula->north->uAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 's': // Velocidade U no sul -------------------------------
                    if( celula->south->type == 'b' )
                        return (
                                  (0.5 + celula->southFaceGamma) * (celula->south->uAtual)
                                + (0.5 - celula->southFaceGamma) * (celula->uAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'e': // Velocidade no leste -------------------------------
                    if( celula->east->type == 'b' )
                        return (
                                  (0.5 + celula->eastFaceGamma) * (celula->uAtual)
                                + (0.5 - celula->eastFaceGamma) * (celula->east->uAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true ); 
                break;

                case 'w': // Velocidade no oeste -------------------------------
                    if( celula->west->type == 'b' )
                        return (
                                  (0.5 + celula->westFaceGamma) * (celula->west->uAtual)
                                + (0.5 - celula->westFaceGamma) * (celula->uAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );  
                break;
            }
        break;

        case ID_VAR_V: // =================== Velocidade V =====================
            switch( direcao )
            {
                case 'n': // Velocidade V no norte -----------------------------
                    if( celula->north->type == 'b' )
                        return (
                                  (0.5 + celula->northFaceGamma) * (celula->vAtual)
                                + (0.5 - celula->northFaceGamma) * (celula->north->vAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );  
                break;

                case 's': // Velocidade V no sul -------------------------------
                    if( celula->south->type == 'b' )
                        return (
                                  (0.5 + celula->southFaceGamma) * (celula->south->vAtual)
                                + (0.5 - celula->southFaceGamma) * (celula->vAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'e': // Velocidade V no leste -------------------------------
                    if( celula->east->type == 'b' )
                        return (
                                  (0.5 + celula->eastFaceGamma) * (celula->vAtual)
                                + (0.5 - celula->eastFaceGamma) * (celula->east->vAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'w': // Velocidade V no oeste -------------------------------
                    if( celula->west->type == 'b' )
                        return (
                                  (0.5 + celula->westFaceGamma) * (celula->west->vAtual)
                                + (0.5 - celula->westFaceGamma) * (celula->vAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );   
                break;
            }
        break;

        case ID_VAR_MI: // =================== Viscosidade =====================
            switch( direcao )
            {
                case 'n': // Viscosidade no norte -------------------------------
                     if( celula->north->type == 'b' )
                        return (
                                  (0.5 + celula->northFaceGamma) * (celula->miAtual)
                                + (0.5 - celula->northFaceGamma) * (celula->north->miAtual)
                               );
                     else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 's': // Viscosidade no sul -------------------------------
                    if( celula->south->type == 'b' )
                        return (
                                  (0.5 + celula->southFaceGamma) * (celula->south->miAtual)
                                + (0.5 - celula->southFaceGamma) * (celula->miAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'e': // Viscosidade no leste -------------------------------
                    if( celula->east->type == 'b' )
                        return (
                                  (0.5 + celula->eastFaceGamma) * (celula->miAtual)
                                + (0.5 - celula->eastFaceGamma) * (celula->east->miAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'w': // Viscosidade no oeste -------------------------------
                    if( celula->west->type == 'b' )
                        return (
                                  (0.5 + celula->westFaceGamma) * (celula->west->miAtual)
                                + (0.5 - celula->westFaceGamma) * (celula->miAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;
            }
        break;

        case ID_VAR_K: // =============== Condutividade termica k ==================
            switch( direcao )
            {
                case 'n': // k no norte -------------------------------
                     if( celula->north->type == 'b' )
                        return (
                                  (0.5 + celula->northFaceGamma) * (celula->kAtual)
                                + (0.5 - celula->northFaceGamma) * (celula->north->kAtual)
                               );
                     else
                        return condicaoFronteira( celula, id_variavel, direcao, true );   
                break;

                case 's': // k no sul -------------------------------
                    if( celula->south->type == 'b' )
                        return (
                                  (0.5 + celula->southFaceGamma) * (celula->south->kAtual)
                                + (0.5 - celula->southFaceGamma) * (celula->kAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );  
                break;

                case 'e': // k no leste -------------------------------
                    if( celula->east->type == 'b' )
                        return (
                                  (0.5 + celula->eastFaceGamma) * (celula->kAtual)
                                + (0.5 - celula->eastFaceGamma) * (celula->east->kAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'w': // k no oeste -------------------------------
                    if( celula->west->type == 'b' )
                        return (
                                  (0.5 + celula->westFaceGamma) * (celula->west->kAtual)
                                + (0.5 - celula->westFaceGamma) * (celula->kAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;
            }
        break;

        case ID_VAR_P: // =============== Pressao p ==================
            switch( direcao )
            {
                case 'n': // p no norte -------------------------------
                     if( celula->north->type == 'b' )
                        return (
                                  (0.5 + celula->northFaceGamma) * (celula->pAtual)
                                + (0.5 - celula->northFaceGamma) * (celula->north->pAtual)
                               );
                     else
                        return condicaoFronteira( celula, id_variavel, direcao, true );   
                break;

                case 's': // p no sul -------------------------------
                    if( celula->south->type == 'b' )
                        return (
                                  (0.5 + celula->southFaceGamma) * (celula->south->pAtual)
                                + (0.5 - celula->southFaceGamma) * (celula->pAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );  
                break;

                case 'e': // p no leste -------------------------------
                    if( celula->east->type == 'b' )
                        return (
                                  (0.5 + celula->eastFaceGamma) * (celula->pAtual)
                                + (0.5 - celula->eastFaceGamma) * (celula->east->pAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;

                case 'w': // p no oeste -------------------------------
                    if( celula->west->type == 'b' )
                        return (
                                  (0.5 + celula->westFaceGamma) * (celula->west->pAtual)
                                + (0.5 - celula->westFaceGamma) * (celula->pAtual)
                               );
                    else
                        return condicaoFronteira( celula, id_variavel, direcao, true );
                break;
            }
        break;

        default:
            // Outros casos
            cout << "Em interpolaVariavelFace(): ID_VARIAVEL = " << id_variavel << "\n";
            IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
            return 0.0;
        break;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao interpolaVariavelFace()";
    return 0.0;
}

/*______________________________________________________________________________

			FUNCTION interpolaTodasInterfaces()
								
    Interpola todas as variaveis escalares das faces de acordo com variaveis 
nos centros das celulas da malha 'grid'.

    Parametros:
        grid: apontador para a malha.

Criado em 24/04/07 por Andre e Fernando	
	
______________________________________________________________________________*/
void interpolaTodasInterfaces( Grid *malha, char ciclo, bool primeiraIteracao )
{
    Cell *celula;  
    
    if(malha->firstGridCell != NULL)
    {
        // Percorrer todas as celulas e definir suas faces com valores nulos
        celula = malha->firstGridCell;
        while ( celula != 0 )
        {         
            // usado so' para calcular du/dx
            celula->northFaceU = interpolaVariavelFace( celula, ID_VAR_U, 'n' );
            celula->southFaceU = interpolaVariavelFace( celula, ID_VAR_U, 's' );

            // usado so' para calcular dv/dy
            celula->eastFaceV = interpolaVariavelFace( celula, ID_VAR_V, 'e' );
            celula->westFaceV = interpolaVariavelFace( celula, ID_VAR_V, 'w' );

            if( primeiraIteracao )
            {
                // Velocidade
                celula->eastFaceU  = interpolaVariavelFace( celula, ID_VAR_U, 'e' );
                celula->westFaceU  = interpolaVariavelFace( celula, ID_VAR_U, 'w' );
    
                celula->northFaceV = interpolaVariavelFace( celula, ID_VAR_V, 'n' );
                celula->southFaceV = interpolaVariavelFace( celula, ID_VAR_V, 's' );
            }

            if( ciclo == 'A' )
            {
                // Viscosidade
                celula->northFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 'n' );
                celula->southFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 's' );
                celula->eastFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'e' );
                celula->westFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'w' );

                /*celula->northFaceEt = interpolaVariavelFace( celula, ID_VAR_Et, 'n' );
                celula->southFaceEt = interpolaVariavelFace( celula, ID_VAR_Et, 's' );
                celula->eastFaceEt  = interpolaVariavelFace( celula, ID_VAR_Et, 'e' );
                celula->westFaceEt  = interpolaVariavelFace( celula, ID_VAR_Et, 'w' );*/

                celula->northFaceK = interpolaVariavelFace( celula, ID_VAR_K, 'n' );
                celula->southFaceK = interpolaVariavelFace( celula, ID_VAR_K, 's' );
                celula->eastFaceK  = interpolaVariavelFace( celula, ID_VAR_K, 'e' );
                celula->westFaceK  = interpolaVariavelFace( celula, ID_VAR_K, 'w' );
            }

            if( ciclo == 'A' || ciclo == 'B' )
            {
                // Densidade
                celula->northFaceRho = interpolaVariavelFace( celula, ID_VAR_RHO, 'n' ); 
                celula->southFaceRho = interpolaVariavelFace( celula, ID_VAR_RHO, 's' );
                celula->eastFaceRho  = interpolaVariavelFace( celula, ID_VAR_RHO, 'e' );
                celula->westFaceRho  = interpolaVariavelFace( celula, ID_VAR_RHO, 'w' );
            }

            celula = celula->next;
        }        
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
    }
}


