
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
        cout << "Na funcao condicaoFronteira():\n";
        IMPRIMIR_ERRO_FRONTEIRA_DE_CELULA_INEXISTENTE();
        return 0.0;    
    }

    // Verificacao de erro de direcao inva'lida
    if( direcao != 'n'
    &&  direcao != 's'
    &&  direcao != 'e'
    &&  direcao != 'w' )
    {
        cout << "Na funcao condicaoFronteira():\n";
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
                    
                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE); // Rho_infinito

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
                        return celula->rhoAtual; // espelhado
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

                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE); // Rho_infinito

                break;
            }
        break;

        case ID_VAR_T: // =================== Temperatura =====================
            switch( direcao )
            {
                case 'n': // Temperatura no norte -----------------------------
                    
                    return TEMPERATURA_LIVRE;
                
                break;

                case 's': // Temperatura no sul -------------------------------

                    if( fronteiraFace )
                    {
                        return TEMPERATURA_PAREDE; // Temperatura da placa plana
                    }
                    else // na celula imagina'ria
                    {
                        return celula->TAtual; // espelhado
                    }
                    
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

                case 'w': // Temperatura no oeste -------------------------------
                    
                    return TEMPERATURA_LIVRE; // Temperatura livre
                
                break;
            }
        break;

        case ID_VAR_U: // =================== Velocidade U =====================
            switch( direcao )
            {
                case 'n': // Velocidade U no norte -----------------------------
                    
                    return VEL_LIVRE; // Velocidade_infinito
                
                break;

                case 's': // Velocidade U no sul -------------------------------                          
                    if( fronteiraFace )
                    {
                        return 0.0; // Velocidade da placa plana
                    }
                    else // na celula imagina'ria
                    {
                        return 0.0; //celula->uAtual; // espelhado
                    }

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

                    return VEL_LIVRE; // Velocidade_infinito

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

                    if( fronteiraFace )
                    {
                        return 0.0; // Velocidade nula
                    }
                    else // na celula imagina'ria
                    {
                        return -1.0*(celula->vAtual); // espelhado
                    }

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

                    return PRESSAO_LIVRE; // p_infinito

                break;

                case 's': // Pressao no sul -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->pAtual) - (celula->northFaceP);
                    }
                    else // na celula imagina'ria
                    {
                        return celula->pAtual; // espelhado
                    }

                break;

                case 'e': // Pressao no leste -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->pAtual) - (celula->westFaceP);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->pAtual) - 2.0*(celula->westFaceP);
                    }

                break;

                case 'w': // Pressao no oeste -------------------------------

                    return PRESSAO_LIVRE;

                break;
            }
        break;

        case ID_VAR_K: // =============== Condutividade termica k ================
            switch( direcao )
            {
                case 'n': // k no norte -----------------------------
                    
                    return (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL); // Livre
                
                break;

                case 's': // k no sul -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->kAtual) - (celula->northFaceK);
                    }
                    else // na celula imagina'ria
                    {
                        return celula->kAtual; // espelhado
                    }

                break;

                case 'e': // k no leste -------------------------------
                    // Extrapolado
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->kAtual) - (celula->westFaceK);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->kAtual) - 2.0*(celula->westFaceK);
                    }
                break;

                case 'w': // k no leste -------------------------------
                    
                    return (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL); // Livre
                
                break;
            }
        break;

        case ID_VAR_MI: // =============== Viscosidade dinamica mi ================
            switch( direcao )
            {
                case 'n': // mi no norte -----------------------------
                    
                    return MI_ZERO; // Livre
                
                break;

                case 's': // mi no sul -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->miAtual) - (celula->northFaceMi);
                    }
                    else // na celula imagina'ria
                    {
                        return celula->miAtual; // espelhado
                    }

                break;

                case 'e': // mi no leste -------------------------------
                    // Extrapolado
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->miAtual) - (celula->westFaceMi);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->miAtual) - 2.0*(celula->westFaceMi);
                    }
                break;

                case 'w': // mi no leste -------------------------------
                    
                    return MI_ZERO; // Livre
                
                break;
            }
        break;

        case ID_VAR_Et: // =============== Energia total Et ================
            switch( direcao )
            {
                case 'n': // Et no norte -----------------------------

                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE) * 
                           (
                               R * TEMPERATURA_LIVRE / (GAMMA-1.0) 
                             + AO_QUADRADO(VEL_LIVRE) / 2.0
                           );
                
                break;

                case 's': // Et no sul -------------------------------
                    // Extrapolado linearmente
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->EtAtual) - (celula->northFaceEt);
                    }
                    else // na celula imagina'ria
                    {
                        return celula->EtAtual;   // espelhado
                    }

                break;

                case 'e': // Et no leste -------------------------------
                    // Extrapolado
                    if( fronteiraFace )
                    {
                        return 2.0*(celula->EtAtual) - (celula->westFaceEt);
                    }
                    else // na celula imagina'ria
                    {
                        return 3.0*(celula->EtAtual) - 2.0*(celula->westFaceEt);
                    }
                break;

                case 'w': // Et no oeste -------------------------------
                    
                    return PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE) * 
                           (
                               R * TEMPERATURA_LIVRE / (GAMMA-1.0) 
                             + AO_QUADRADO(VEL_LIVRE) / 2.0
                           );                
                break;
            }
        break;

        default:
            // Outros casos
            cout << "Na funcao condicaoFronteira():\n";
            IMPRIMIR_ERRO_FRONTEIRA_ID_VARIAVEL_INEXISTENTE();
            cout << "Do erro acima, id_variavel = " << id_variavel << endl;
            return 0.0;
        break;
    }

    cout << "Na funcao condicaoFronteira():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return 0.0;
}

