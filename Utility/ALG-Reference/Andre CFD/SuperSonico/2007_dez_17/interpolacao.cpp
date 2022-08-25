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

                celula->northFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_n ) ) 
                                      / ( 1.0 + 0.05*AO_QUADRADO( Pe_n ) );
                celula->southFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_s ) ) 
                                      / ( 1.0 + 0.05*AO_QUADRADO( Pe_s ) );
                celula->eastFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_e ) ) 
                                     / ( 1.0 + 0.05*AO_QUADRADO( Pe_e ) );
                celula->westFaceBeta = ( 1.0 + 0.005*AO_QUADRADO( Pe_w ) ) 
                                     / ( 1.0 + 0.05*AO_QUADRADO( Pe_w ) );

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
         malha: apontador para a malha
         celula: a celula atual
         id_variavel: um identificador para saber qual variavel esta sendo
                      solicitada;
         direcao: indica qual a face solicitada (norte, sul, leste, oeste);

Criado por Andre e Fernando dia 24/04/07
Atualizado por Henrique dia 17/07/2007
______________________________________________________________________________*/
double interpolaVariavelFace( Cell *celula, short id_variavel, char direcao )
{
    double Gamma = 0.0,
           Var1  = 0.0,
           Var2  = 0.0;

    int levelCelula;

    bool extrapolacao = false;

    // Verificacao de erro de celula inexistente
    if( celula == NULL )
    {
        cout << "Na funcao interpolaVariavelFace():\n";
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;
    }

    // Se a face em que sera feita a interpolacao e' entre interno e ficticio
    if(
        ( (!celula->volumeFicticio) && direcao == 'n' && celula->north->volumeFicticio) ||
        ( (!celula->volumeFicticio) && direcao == 's' && celula->south->volumeFicticio) ||
        ( (!celula->volumeFicticio) && direcao == 'e' && celula->east->volumeFicticio ) ||
        ( (!celula->volumeFicticio) && direcao == 'w' && celula->west->volumeFicticio ) ||
        ( celula->volumeFicticio && direcao == 'n' && celula->direcaoFronteira == FRONTEIRA_SUL ) ||
        ( celula->volumeFicticio && direcao == 's' && celula->direcaoFronteira == FRONTEIRA_NORTE ) ||
        ( celula->volumeFicticio && direcao == 'e' && celula->direcaoFronteira == FRONTEIRA_OESTE ) ||
        ( celula->volumeFicticio && direcao == 'w' && celula->direcaoFronteira == FRONTEIRA_LESTE )
      )
    {
        char direcaoFronteira;
        Cell *celulaInterna;

        // Se a celula e' volume ficticio, seta a direcao da fronteira para a qual sera chamada
        // valorCondicaoFronteira()
        if( celula->volumeFicticio )
            switch( direcao )
            {
                case 'n':
                    celulaInterna = celula->north;
                    direcaoFronteira = 's';
                    break;
                case 's':
                    celulaInterna = celula->south;
                    direcaoFronteira = 'n';
                    break;
                case 'e':
                    celulaInterna = celula->east;
                    direcaoFronteira = 'w';
                    break;
                case 'w':
                    celulaInterna = celula->west;
                    direcaoFronteira = 'e';
                    break;
            }
        // Senao, a direcao e celula e' a mesma pedida
        else
        {
            direcaoFronteira = direcao;
            celulaInterna = celula;
        }

        return valorCondicaoFronteira( celulaInterna, id_variavel, direcaoFronteira);

    }

    levelCelula = celula->level;

    switch( direcao )
    {
        case 'n': // Norte -------------------------------------
            Gamma = celula->northFaceGamma;

            switch( id_variavel )
            {
                case ID_VAR_RHO: // ===================== RHO =========================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->rhoAtual;
                        // Var2 e' a velocidade u no norte da celula
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->uAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->vAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->miAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->kAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->pAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_T: // =================== Temperatura T =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_NORTE) ||
                        !celula->north->active)
                    {
                        extrapolacao = true;
                        Var1 = celula->TAtual;
                        Var2 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->TAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
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

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->rhoAtual;
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->uAtual;
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->vAtual;
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->miAtual;
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->kAtual;
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->pAtual;
                    }

                break;

                    case ID_VAR_T: // =================== Temperatura T =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_SUL) ||
                        !celula->south->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->TAtual;
                        Var2 = calculaVarVizinho( celula->north, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->south, id_variavel, celula->level );
                        Var2 = celula->TAtual;
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

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->rhoAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->uAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->vAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->miAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->kAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->pAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }

                break;

                case ID_VAR_T: // =================== Temperatura T =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_LESTE) ||
                        !celula->east->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->TAtual;
                        Var2 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = celula->TAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
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

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->rhoAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->rhoAtual;
                    }

                break;

                case ID_VAR_U: // =================== Velocidade U =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->uAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->uAtual;
                    }

                break;

                case ID_VAR_V: // =================== Velocidade V =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->vAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->vAtual;
                    }

                break;

                case ID_VAR_MI: // =================== Viscosidade Mi =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->miAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->miAtual;
                    }

                break;

                case ID_VAR_K: // ============== Condutividade termica K ==================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->kAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->kAtual;
                    }

                break;

                case ID_VAR_P: // =================== Pressao P =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->pAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->pAtual;
                    }

                break;

                case ID_VAR_T: // =================== Temperatura T =====================

                    // Se nao tem celulas ativas na direcao pedida, extrapola
                    if( (celula->volumeFicticio && celula->direcaoFronteira == FRONTEIRA_OESTE) ||
                        !celula->west->active )
                    {
                        extrapolacao = true;
                        Var1 = celula->TAtual;
                        Var2 = calculaVarVizinho( celula->east, id_variavel, celula->level );
                    }
                    else
                    {
                        Var1 = calculaVarVizinho( celula->west, id_variavel, celula->level );
                        Var2 = celula->TAtual;
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
        return (2.0 * Var1) - Var2;  // extrapolacao

    // Se a face e' entre celulas internas ou entre dois volumes ficticios
    else
        return (0.5 + Gamma) * Var1 + (0.5 - Gamma) * Var2;  // WUDS
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
        while ( celula != NULL )
        {
            if( celula->active )
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
                    celula->northFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 'n' );
                    celula->southFaceMi = interpolaVariavelFace( celula, ID_VAR_MI, 's' );
                    celula->eastFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'e' );
                    celula->westFaceMi  = interpolaVariavelFace( celula, ID_VAR_MI, 'w' );

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

                    // Temperatura
                    celula->northFaceT = interpolaVariavelFace( celula, ID_VAR_T, 'n' );
                    celula->southFaceT = interpolaVariavelFace( celula, ID_VAR_T, 's' );
                    celula->eastFaceT  = interpolaVariavelFace( celula, ID_VAR_T, 'e' );
                    celula->westFaceT  = interpolaVariavelFace( celula, ID_VAR_T, 'w' );
                }
            }
            celula = celula->next;
        }
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
    }
}

/*______________________________________________________________________________

               Funcao interpolaUsandoPlano()

    Retorna znovo, como resultado da interpolacao de (xnovo, ynovo) no plano
determinado pelos pontos (x0,y0,z0), (x1,y1,z1), (x2,y2,z2).

    Parametros:
        x0, y0, z0, x1, y1, z1, x2, y2, z2: coordenadas dos pontos base.
        xnovo, ynovo: coordenadas do ponto a ser interpolado para obter znovo.

______________________________________________________________________________*/
double interpolaUsandoPlano(double x0, double y0, double z0,
                            double x1, double y1, double z1,
                            double x2, double y2, double z2,
                            double xnovo, double ynovo)
{
    double b = x1*z0 - x2*z0 - x0*z1 + x2*z1 + x0*z2 - x1*z2;
    double c = -x1*y0 + x2*y0 + x0*y1 - x2*y1 - x0*y2 + x1*y2;
    double d = x2*y1*z0 - x1*y2*z0 - x2*y0*z1 + x0*y2*z1 + x1*y0*z2 - x0*y1*z2;
    double a = -y1*z0 + y2*z0 + y0*z1 - y2*z1 - y0*z2 + y1*z2;

    return (-(d/c) -((a*xnovo)/c) - ((b*ynovo)/c) );
}
