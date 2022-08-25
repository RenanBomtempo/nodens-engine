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
            if( celula->active )
            {
                // Calculo de gamma segundo o esquema WUDS
                Pe_n = calcula_Peclet( celula, 'n' );
                Pe_s = calcula_Peclet( celula, 's' );
                Pe_e = calcula_Peclet( celula, 'e' );
                Pe_w = calcula_Peclet( celula, 'w' );
    
                celula->northFaceGamma = (Pe_n >= 0) ?
                                        ( AO_QUADRADO(Pe_n) / (10.0 + 2*AO_QUADRADO(Pe_n)) )
                                            :
                                        ( -1.0*AO_QUADRADO(Pe_n) / (10.0 + 2*AO_QUADRADO(Pe_n)) );
    
                celula->southFaceGamma = (Pe_s >= 0) ?
                                        ( AO_QUADRADO(Pe_s) / (10.0 + 2*AO_QUADRADO(Pe_s)) )
                                            :
                                        ( -1.0*AO_QUADRADO(Pe_s) / (10.0 + 2*AO_QUADRADO(Pe_s)) );
    
                celula->eastFaceGamma = (Pe_e >= 0) ?
                                        ( AO_QUADRADO(Pe_e) / (10.0 + 2*AO_QUADRADO(Pe_e)) )
                                            :
                                        ( -1.0*AO_QUADRADO(Pe_e) / (10.0 + 2*AO_QUADRADO(Pe_e)) );
    
                celula->westFaceGamma = (Pe_w >= 0) ?
                                        ( AO_QUADRADO(Pe_w) / (10.0 + 2*AO_QUADRADO(Pe_w)) )
                                            :
                                        ( -1.0*AO_QUADRADO(Pe_w) / (10.0 + 2*AO_QUADRADO(Pe_w)) );
    
    
                // Calculo de gamma segundo o esquema de diferencas centradas
                //celula->northFaceGamma = 0.0;
                //celula->southFaceGamma = 0.0;
                //celula->eastFaceGamma = 0.0;
                //celula->westFaceGamma = 0.0;
            }
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
            if( celula->active )
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
Atualizado por Henrique dia 17/07/2007
______________________________________________________________________________*/
double interpolaVariavelFace( Cell *celula,
                          short id_variavel,
                          char direcao )
{
    double Gamma = 0.0,
           Var1  = 0.0,
           Var2  = 0.0;

    bool extrapolacao = false;

    // Verificacao de erro de celula inexistente
    if( celula == NULL )
    {
        cout << "Na funcao interpolaVariavelFace():\n";
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;
    }

    switch( direcao )
    {
        case 'n': // Norte -------------------------------------
            Gamma = celula->northFaceGamma;

            switch( id_variavel )
            {
                case ID_VAR_RHO: // ===================== RHO =========================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = celula->south->rhoAtual;
                    }
                    else
                    {
                        Var1 = celula->rhoAtual;
                        Var2 = celula->north->rhoAtual;
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = celula->south->uAtual;
                    }
                    else
                    {
                        Var1 = celula->uAtual;
                        Var2 = celula->north->uAtual;
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = celula->south->vAtual;
                    }
                    else
                    {
                        Var1 = celula->vAtual;
                        Var2 = celula->north->vAtual;
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = celula->south->miAtual;
                    }
                    else
                    {
                        Var1 = celula->miAtual;
                        Var2 = celula->north->miAtual;
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = celula->south->kAtual;
                    }
                    else
                    {
                        Var1 = celula->kAtual;
                        Var2 = celula->north->kAtual;
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = celula->south->pAtual;
                    }
                    else
                    {
                        Var1 = celula->pAtual;
                        Var2 = celula->north->pAtual;
                    }

                break;

                default:
                    // Outros casos
                    cout << "Na funcao interpolaVariavelFace(): ID_VARIAVEL = " << id_variavel << "\n";
                    IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
                    return 0.0;
                break;
            }
        break;

        case 's': // Sul -------------------------------------
            Gamma = celula->southFaceGamma;

            switch( id_variavel )
            {
                case ID_VAR_RHO: // ===================== RHO =========================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL)
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = celula->north->rhoAtual;
                    }
                    else
                    {
                        Var1 = celula->south->rhoAtual;
                        Var2 = celula->rhoAtual;
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL)
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = celula->north->uAtual;
                    }
                    else
                    {
                        Var1 = celula->south->uAtual;
                        Var2 = celula->uAtual;
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL)
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = celula->north->vAtual;
                    }
                    else
                    {
                        Var1 = celula->south->vAtual;
                        Var2 = celula->vAtual;
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL)
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = celula->north->miAtual;
                    }
                    else
                    {
                        Var1 = celula->south->miAtual;
                        Var2 = celula->miAtual;
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL)
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = celula->north->kAtual;
                    }
                    else
                    {
                        Var1 = celula->south->kAtual;
                        Var2 = celula->kAtual;
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL)
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = celula->north->pAtual;
                    }
                    else
                    {
                        Var1 = celula->south->pAtual;
                        Var2 = celula->pAtual;
                    }

                break;

                default:
                    // Outros casos
                    cout << "Na funcao interpolaVariavelFace(): ID_VARIAVEL = " << id_variavel << "\n";
                    IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
                    return 0.0;
                break;
            }
        break;

        case 'e': // Leste -------------------------------------
            Gamma = celula->eastFaceGamma;

            switch( id_variavel )
            {
                case ID_VAR_RHO: // ===================== RHO =========================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = celula->west->rhoAtual;
                    }
                    else
                    {
                        Var1 = celula->rhoAtual;
                        Var2 = celula->east->rhoAtual;
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = celula->west->uAtual;
                    }
                    else
                    {
                        Var1 = celula->uAtual;
                        Var2 = celula->east->uAtual;
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = celula->west->vAtual;
                    }
                    else
                    {
                        Var1 = celula->vAtual;
                        Var2 = celula->east->vAtual;
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = celula->west->miAtual;
                    }
                    else
                    {
                        Var1 = celula->miAtual;
                        Var2 = celula->east->miAtual;
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = celula->west->kAtual;
                    }
                    else
                    {
                        Var1 = celula->kAtual;
                        Var2 = celula->east->kAtual;
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = celula->west->pAtual;
                    }
                    else
                    {
                        Var1 = celula->pAtual;
                        Var2 = celula->east->pAtual;
                    }

                break;

                default:
                    // Outros casos
                    cout << "Na funcao interpolaVariavelFace(): ID_VARIAVEL = " << id_variavel << "\n";
                    IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
                    return 0.0;
                break;
            }
        break;

        case 'w': // Oeste -------------------------------------
            Gamma = celula->westFaceGamma;

            switch( id_variavel )
            {
                case ID_VAR_RHO: // ===================== RHO =========================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = celula->east->rhoAtual;
                    }
                    else
                    {
                        Var1 = celula->west->rhoAtual;
                        Var2 = celula->rhoAtual;
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = celula->east->uAtual;
                    }
                    else
                    {
                        Var1 = celula->west->uAtual;
                        Var2 = celula->uAtual;
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = celula->east->vAtual;
                    }
                    else
                    {
                        Var1 = celula->west->vAtual;
                        Var2 = celula->vAtual;
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = celula->east->miAtual;
                    }
                    else
                    {
                        Var1 = celula->west->miAtual;
                        Var2 = celula->miAtual;
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = celula->east->kAtual;
                    }
                    else
                    {
                        Var1 = celula->west->kAtual;
                        Var2 = celula->kAtual;
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    if(celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE)
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = celula->east->pAtual;
                    }
                    else
                    {
                        Var1 = celula->west->pAtual;
                        Var2 = celula->pAtual;
                    }

                break;

                default:
                    // Outros casos
                    cout << "Na funcao interpolaVariavelFace(): ID_VARIAVEL = " << id_variavel << "\n";
                    IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
                    return 0.0;
                break;
            }
        break;

        default:
            cout << "Na funcao interpolaVariavelFace(): Face = " << direcao << "\n";
            IMPRIMIR_ERRO_FACE_INVALIDA();
            return 0.0;
        break;
    }

    // Funcao de interpolacao ############################################################

    // Se a face em que sera' feita a interpolacao nao e' compartilhada com vizinho
    if(celula->volumeFicticio && extrapolacao)
    {
        return (2.0 * Var1) - Var2;  // extrapolacao
    }
    // Se a face e' interna de um ficticio exceto o caso acima
    else if(celula->volumeFicticio && (!extrapolacao) )
    {
        return (Var1 + Var2) / 2.0; // media arit. normal 
    }
    // Se a face em que sera feita a interpolacao e' entre interno e ficticio
    else if( 
             ( (!celula->volumeFicticio) && direcao == 'n' && celula->north->volumeFicticio) ||
             ( (!celula->volumeFicticio) && direcao == 's' && celula->south->volumeFicticio) ||
             ( (!celula->volumeFicticio) && direcao == 'e' && celula->east->volumeFicticio ) ||
             ( (!celula->volumeFicticio) && direcao == 'w' && celula->west->volumeFicticio ) 
           )
    {
        return (Var1 + Var2) / 2.0; // media arit. normal 
    }
    // Se a face e' entre celulas internas
    else
    {
        return (0.5 + Gamma) * Var1 + (0.5 - Gamma) * Var2;  // WUDS
    }
    // ###################################################################################

    cout << "Na funcao interpolaVariavelFace():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return 0.0;
}


/*______________________________________________________________________________

               Funcao interpolaTodasInterfaces()

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
                celula->eastFaceUAnterior = celula->eastFaceU;
                celula->westFaceUAnterior = celula->westFaceU;

                celula->northFaceV = interpolaVariavelFace( celula, ID_VAR_V, 'n' );
                celula->southFaceV = interpolaVariavelFace( celula, ID_VAR_V, 's' );
                celula->northFaceVAnterior = celula->northFaceV;
                celula->southFaceVAnterior = celula->southFaceV;

                // Viscosidade
                celula->northFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 'n' );
                celula->southFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 's' );
                celula->eastFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'e' );
                celula->westFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'w' );
            }

            if( ciclo == 'A' )
            {
                // Viscosidade
                /*celula->northFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 'n' );
                celula->southFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 's' );
                celula->eastFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'e' );
                celula->westFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'w' );*/

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

                // Pressao
                celula->northFaceP = interpolaVariavelFace( celula, ID_VAR_P, 'n' ); 
                celula->southFaceP = interpolaVariavelFace( celula, ID_VAR_P, 's' );
                celula->eastFaceP  = interpolaVariavelFace( celula, ID_VAR_P, 'e' );
                celula->westFaceP  = interpolaVariavelFace( celula, ID_VAR_P, 'w' );
            }

            celula = celula->next;
        }
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
    }
}


