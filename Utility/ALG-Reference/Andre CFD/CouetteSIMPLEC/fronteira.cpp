
/*  ============ fronteira.cpp ===========
    Contem informacoes sobre as condicoes
    de fronteira do problema

Criado por Andre STALTZ dia 31/03/07
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include "constants.h"
#include "erros.h"

/*______________________________________________________________________________
                            
                          condicaoFronteira()
                            
    Retorna o valor de uma certa variavel (indicada pelo parametro) em uma das 
fronteiras do dominio (tambem especificada pelo parametro). A fronteira pode ser
uma face de celula ou uma celula imagina'ria, fora da malha.
    
    Parametros:
         celula: a celula atual que possui fronteira
         id_variavel: um identificador para saber qual variavel esta sendo
                      solicitada;
         direcao: indica qual a fronteira (norte, sul, leste, oeste);
         fronteiraFace: booleano onde verdadeiro indica a fronteira na face
                          da celula e falso indica a fronteira como celula 
                          imagina'ria fora da malha;

Criado por Andre STALTZ dia 31/03/07
Ampliado para u, v e p por Fernando Grossi dia 24/04/07
______________________________________________________________________________*/

double condicaoFronteira( Cell *celula,
                          short id_variavel, 
                          char direcao,
                          bool fronteiraFace )
{   
    // Verificacao de erro de celula inexistente
    if( celula == NULL )
    {
        IMPRIMIR_ERRO_FRONTEIRA_DE_CELULA_INEXISTENTE();
        return 0.0;    
    }

    // Verificacao de erro de direcao inva'lida
    if( direcao != 'n'
    &&  direcao != 's'
    &&  direcao != 'e'
    &&  direcao != 'w' )
    {
        IMPRIMIR_ERRO_FRONTEIRA_DIRECAO_INEXISTENTE();
        return 0.0;
    }

    // Verificar qual variavel foi solicitada na fronteira
    switch( id_variavel )
    {
        case ID_VAR_RHO: // ===================== RHO =========================
            switch( direcao )
            {
                case 'n': // Rho no norte -------------------------------------
                    
                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE); // Rho livre

                break;

                case 's': // Rho no sul ---------------------------------------
                    // Extrapolado com a equacao de estado
                    if( fronteiraFace ) // na face
                    {    
                        return (
                                    2.0*(celula->pAtual) 
                                  - (celula->northFaceP)
                               )
                             / ( R * TEMPERATURA_PAREDE );
                    }
                    else // na celula imagina'ria
                    {
                        return (
                                    3.0*(celula->pAtual) 
                                  - 2.0*(celula->northFaceP)
                               )
                             / ( R * TEMPERATURA_PAREDE );
                    }

                break;
                
                case 'e': // Rho no leste -------------------------------------
                    // Extrapolado com a equacao de estado
                    if( fronteiraFace )
                    {
                        return (
                                 2.0*(celula->pAtual) - (celula->westFaceP)
                               )
                            / (
                                R*(
                                    2.0*(celula->TAtual) - (celula->westFaceT)
                                  )
                              );
                    }
                    else // na celula imagina'ria
                    {
                        return (
                                 3.0*(celula->pAtual) - 2.0*(celula->westFaceP)
                               )
                            / (
                                R*(
                                    3.0*(celula->TAtual) - 2.0*(celula->westFaceT)
                                  )
                              );
                    }

                break; 

                case 'w': // Rho no oeste -------------------------------------

                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE); // Rho livre

                break;
            }
        break;

        case ID_VAR_T: // =================== Temperatura =====================
            switch( direcao )
            {
                case 'n': // Temperatura no norte -----------------------------
                    
                    return TEMPERATURA_LIVRE; // Temperatura livre
                
                break;

                case 's': // Temperatura no sul -------------------------------

                    return TEMPERATURA_PAREDE; // Temperatura da placa plana

                break;

                case 'e': // Temperatura no leste -------------------------------
                    // Extrapolado
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->TAtual) - (celula->westFaceT);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->TAtual) - 2.0*(celula->westFaceT);
                    }

                break;

                case 'w': // Temperatura no leste -------------------------------
                    
                    return TEMPERATURA_LIVRE; // Temperatura livre
                
                break;
            }
        break;

        case ID_VAR_U: // =================== Velocidade U =====================
            switch( direcao )
            {
                case 'n': // Velocidade U no norte -----------------------------
                    
                    return VEL_PAREDE; // Velocidade da placa superior
                
                break;

                case 's': // Velocidade U no sul -------------------------------

                    return 0.0; // Velocidade da placa inferior

                break;

                case 'e': // Velocidade U no leste -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->uAtual) - (celula->westFaceU);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->uAtual) - 2.0*(celula->westFaceU);
                    }

                break;

                case 'w': // Velocidade U no oeste -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->uAtual) - (celula->eastFaceU);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->uAtual) - 2.0*(celula->eastFaceU);
                    }

                break;
            }
        break;

        case ID_VAR_V: // =================== Velocidade V =====================
            switch( direcao )
            {
                case 'n': // Velocidade V no norte -----------------------------
                    
                    return 0.0; // Velocidade nula
                
                break;

                case 's': // Velocidade V no sul -------------------------------

                    return 0.0; // Velocidade nula

                break;

                case 'e': // Velocidade V no leste -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->vAtual) - (celula->westFaceV);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->vAtual) - 2.0*(celula->westFaceV);
                    }

                break;

                case 'w': // Velocidade V no oeste -------------------------------
                    
                    return 0.0; // velocidade nula
                
                break;
            }
        break;

        case ID_VAR_P: // =================== Pressao =====================
            switch( direcao )
            {
                case 'n': // Pressao no norte -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->pAtual) - (celula->southFaceP);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->pAtual) - 2.0*(celula->southFaceP);
                    }

                break;

                case 's': // Pressao no sul -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->pAtual) - (celula->northFaceP);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->pAtual) - 2.0*(celula->northFaceP);
                    }

                break;

                case 'e': // Pressao no leste -------------------------------

                    if( fronteiraFace )
                    {
                        return PRESSAO_LIVRE;
                    }
                    else // na celula imagina'ria
                    {
                        return PRESSAO_LIVRE;
                    }

                break;

                case 'w': // Pressao no oeste -------------------------------

                    if( fronteiraFace )
                    {
                        return PRESSAO_LIVRE;
                    }
                    else // na celula imagina'ria
                    {
                        return PRESSAO_LIVRE;
                    }

                break;
            }
        break;

        default:
            // Outros casos
            IMPRIMIR_ERRO_FRONTEIRA_ID_VARIAVEL_INEXISTENTE();
            cout << "Do erro acima, id_variavel = " << id_variavel << endl;
            return 0.0;
        break;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu na funcao condicaoFronteira()" << endl;
    return 0.0;
}

