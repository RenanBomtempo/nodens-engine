/*  ========== solve.cpp =========
    Contem funcoes para resolver as equacoes
    de Navier-Stokes
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

                              solveSuperSonicSIMPLEC()

           Resolver por SIMPLEC (acoplamento pressao-velocidade/densidade) 
    o escoamento de Couette.

Criado por Andre STALTZ dia 31/03/07
______________________________________________________________________________*/
void solveSuperSonicSIMPLEC( Grid *malha )
{
    bool convergiu = false;
    int  iteracoesC = 0,
         iteracoesB = 0,
         iteracoesA = 0;
    int bla;

    do // Ciclo A -------------------------------------------------------------
    {
        atualizaParametrosDaIteracaoAnterior( malha, 'A' );

        iteracoesB = 0;
        do // Ciclo B -------------------------------------------------------------
        {
            atualizaParametrosDaIteracaoAnterior( malha, 'B' );

            interpolaTodasInterfaces( malha, 'B', false );
            atualiza_Gamma( malha );
            atualiza_Beta( malha );

            // Calcula entrada e saida de massa para satisfazer conservacao
            //calculaEntradaSaidaMassa( malha );

            // Item 2. (calcular os coeficientes do momentum de u* e v*)
            calculaCoeficientesMomentoX( malha );
            calculaCoeficientesMomentoY( malha );
/*
            calculaTermoFonteMomentoX( malha );
            calculaTermoFonteMomentoY( malha );

            // Item 4. (campo de velocidades u* e v* e' determinado)
            cout << "Resolvendo matriz do momento-x...\t";
            gradienteBiconjugadoEstabilizado( malha, ID_VAR_U );
            cout << "Resolvendo matriz do momento-y...\t";
            gradienteBiconjugadoEstabilizado( malha, ID_VAR_V );

            calculaVelocidadesNasFaces( malha );*/

            iteracoesC = 0;
            do // Ciclo C ---------------------------------------------------------
            {      
                atualizaParametrosDaIteracaoAnterior( malha, 'C' );

                calculaTermoFonteMomentoX( malha );
                calculaTermoFonteMomentoY( malha );

                // Item 3.
                calculaCoeficientesCorrecaoPressao( malha, RELAXAMENTO_U, RELAXAMENTO_V );

                // Item 4. (campo de velocidades u* e v* e' determinado)
                cout << "Resolvendo matriz do momento-x...\t";
                gradienteBiconjugadoEstabilizado( malha, ID_VAR_U );
                cout << "Resolvendo matriz do momento-y...\t";
                gradienteBiconjugadoEstabilizado( malha, ID_VAR_V );

                calculaVelocidadesNasFaces( malha );/**/

                // Item 5. Termo fonte (b^P')
                calculaTermoFonteCorrecaoPressao( malha );

                // Item 6a. Resolver a equacao para P'
                cout << "Resolvendo matriz de pressao'...\t";
                gradienteBiconjugadoEstabilizado( malha, ID_VAR_PLINHA );

                // Item 6b e 6c. Corrigir u*, v*, rho* e P*
                corrige_Velocidades_Densidades_Pressoes( malha, RELAXAMENTO_P, 
                                                         RELAXAMENTO_U, RELAXAMENTO_V );

                // Verifica convergencia
                if( convergenciaPressao( malha ) == SUCESSO )
                    convergiu = true;
                else
                    convergiu = false;

                iteracoesC++;
                cout << "\n";

            // Item 7. Retorna ao item 3 ate' convergir ---------------------------
            } while( !convergiu && iteracoesC < MAXITER_C ); // Ciclo C

            cout << "\nIteracoes do ciclo C: " << iteracoesC << endl;
            //cout << "\n\n#####################################\n\n";
            //scanf("%i",&bla);

            // Verifica convergencia
            if( convergenciaVelocidades( malha ) == SUCESSO )
                convergiu = true;
            else
                convergiu = false;

            iteracoesB++;
        // Item 8. Retorna ao item 2 ate' convergir -------------------------------
        } while( !convergiu && iteracoesB < MAXITER_B ); // Ciclo B
  
        cout << "\nIteracoes do ciclo B: " << iteracoesB << endl;

        // Item 9.
        // Calcular os coeficientes da equacao de energia
        calculaCoeficientesEnergia( malha );

        // Resolve a equacao de energia total
        cout << "Resolvendo matriz da energia total...\t";
        gradienteBiconjugadoEstabilizado( malha, ID_VAR_Et );

        // Campos de T, rho, mi, k sao determinados
        atualizaOutrosEscalares( malha, RELAXAMENTO_T );

        // Verifica convergencia da temperatura
        if( convergenciaTemperatura( malha ) == SUCESSO )
            convergiu = true;
        else
            convergiu = false;

        iteracoesA++;
    // Item 11. Retorna ao item 2 ate' convergir
    } while( !convergiu && iteracoesA < MAXITER_A ); // Ciclo A

    cout << "\nIteracoes do ciclo A: " << iteracoesA << endl;

    // A solucao do problema em t + deltaT foi obtida

    cout << " Tempo atual: " << malha->tempoAtual << " s ################################" << endl;	
}

/*______________________________________________________________________________

                              condicaoInicial()

           Configura as variaveis na condicao inicial, ou seja, em t = 0.
______________________________________________________________________________*/
int condicaoInicial( Grid *malha )
{
    Cell *celula;

    // Ajusta tempo para zero
    malha->tempoAtual = 0.0;

    definirCelulasFronteira( malha );

    if(malha->firstGridCell != NULL)
    {
        // Ajustar todas as celulas com a configuracao inicial
        for(celula = malha->firstGridCell; 
            celula != 0; 
            celula = celula->next)
        {
            if( celula->active )
            {
                // Variaveis de fluido
                celula->pAtual = PRESSAO_LIVRE;
                celula->pAnterior = celula->pAtual;

                celula->uAtual = VEL_LIVRE; 
                celula->uAnterior =  celula->uAtual;

                celula->vAtual = 0.0; 
                celula->vAnterior = celula->vAtual; 

                celula->rhoAtual = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
                celula->rhoAnterior = celula->rhoAtual;

                celula->TAtual = TEMPERATURA_LIVRE;
                celula->TAnterior = celula->TAtual;

                celula->EtAtual = celula->rhoAtual
                            * (
                                R*TEMPERATURA_LIVRE / (GAMMA - 1.0) // e (minusculo!)
                            + (
                                  celula->uAtual * celula->uAtual
                                + celula->vAtual * celula->vAtual
                                ) / 2.0
                            );
                celula->EtAnterior = celula->EtAtual;

                celula->miAtual = MI_ZERO;
                celula->miAnterior = celula->miAtual;

                celula->kAtual = (MI_ZERO * GAMMA * R) / ((GAMMA - 1.0) * PRANDTL);
                celula->kAnterior = celula->kAtual;
            }
        }

        interpolaTodasInterfaces( malha, 'B', true );

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao condicaoInicial():\n";
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao condicaoInicial():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*____________________________________________________________________________________
                        calculaEntradaSaidaMassa()

    Calcula a entrada e a saida de massa totais da malha, que serao usadas no calculo
dos coeficientes do momento-x, com base no valor de rho*u nas fronteira leste e oeste
do dominio.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Henrique dia 11/06/07.
_____________________________________________________________________________________*/
int calculaEntradaSaidaMassa( Grid *malha )
{

    // ###########################################
    // OBS! So' funciona para uma malha uniforme
    // ###########################################'

    Cell *celula;
    int bla;

    malha->entradaMassa = malha->saidaMassa = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Percorre as celulas que sao volumes ficticios da fronteira leste do dominio
        // para calcular a saida total de massa
        for(celula = malha->firstGridCell->south;
            celula->active;
            celula = celula->south)
        {
            if( celula->east->type == 'w' && celula->volumeFicticio )
            {
                malha->saidaMassa += celula->rhoAtual * celula->uAtual;
            }
        }

        // Posiciona o apontador "celula" na primeira celula de volume ficticio da
        // fronteira oeste do dominio
        for(celula = malha->firstGridCell;
            celula->west->type != 'w';
            celula = celula->west);

        // Percorre as celulas que sao volumes ficticios da fronteira oeste do dominio
        // para calcular a entrada total de massa
        for(celula = celula->south; 
            celula->active;
            celula = celula->south)
        {
            if( celula->west->type == 'w' && celula->volumeFicticio )
            {
                malha->entradaMassa += celula->rhoAtual * celula->uAtual;
            }
        }
        cout << "entradaMassa = " << malha->entradaMassa
             << "\t saidaMassa = " << malha->saidaMassa << endl;
            //scanf("%i",&bla);
             
        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaEntradaSaidaMassa():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaEntradaSaidaMassa():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaCoeficientesMomentoX()

    Calcula os coeficientes A_p, A_e, A_w, A_n, A_s para cada celula, 
montando a matriz [A] do sistema associado 'a correcao de momento.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Andre STALTZ e Henrique Fav dia 23/04/07
Atualizado em 29/05/07 por Henrique Fav
______________________________________________________________________________*/
int calculaCoeficientesMomentoX( Grid *malha )
{
    // Obs: Todos os deltaY/deltaX da discretizacao foram substituidos por 1,
    // visto que a malha do ALG e' sempre quadrada

    // Obs: Variaveis nas faces (mi, rho, u) PRECISAM ser os "*" (estimados)

    Cell *celula;
    short tipoFronteira;
int bla;
    // Coeficientes
    double A_P = 0.0,
           A_e = 0.0,
           A_w = 0.0,
           A_n = 0.0,
           A_s = 0.0;
  
    double S_P = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            // Evitar caso especial dos quatro cantos ficticios: nao entram no sistema
            if( celula->active )
            {
                // Obtem o identificador que mostra qual o tratamento para a fronteira
                tipoFronteira = tipoCondicaoFronteira( celula, ID_VAR_U );
    
                // Volume ficticio (fronteira oeste)
                if( celula->tipoFronteira == FRONTEIRA_OESTE && celula->volumeFicticio == true ){

                    // Se no OESTE for entrada de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_ENTRADA_MASSA)
                    {
                        // Entrada de massa, coeficientes importantes
                        A_P = 1.0;
                        A_e = 0.0;

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_w = 0.0;

                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoX = valorCondicaoFronteira( celula, ID_VAR_U, 'w' );
                    }

                    // Se no OESTE for saida de massa
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
                    {
                        // Saida de massa, coeficientes importantes
                        A_P = 1.0;
                        A_e = -1.0 ;// * (malha->entradaMassa / malha->saidaMassa);

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_w = 0.0;

                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoX = 0.0;
                    }

                    // Se no OESTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Coeficientes importantes para essa extrapolacao
                        A_P = 1.0;
                        A_e = -1.0;

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_w = 0.0;

                        // coeficiente B
                        celula->b_MomentoX = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoX:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no oeste!" << endl;
                    }
                }

                // Volume ficticio (fronteira leste)
                else if( celula->tipoFronteira == FRONTEIRA_LESTE && celula->volumeFicticio == true ){

                    // Se no LESTE for entrada de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_ENTRADA_MASSA)
                    {
                        // Entrada de massa, coeficientes importantes
                        A_P = 1.0;
                        A_w = 0.0;

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_e = 0.0;

                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoX = valorCondicaoFronteira( celula, ID_VAR_U, 'e' );
                    }

                    // Se no LESTE for saida de massa
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
                    {
                        // Saida de massa, coeficientes importantes
                        A_P = 1.0;
                        A_w = -1.0 ;// * (malha->entradaMassa / malha->saidaMassa);

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_e = 0.0;

                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoX = 0.0;
                    }

                    // Se no LESTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Coeficientes importantes para essa extrapolacao
                        A_P = 1.0;
                        A_w = -1.0;

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_e = 0.0;

                        // coeficiente B
                        celula->b_MomentoX = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoX:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no leste!" << endl;
                    }
                }

                // Volume ficticio (fronteira norte)
                else if( celula->tipoFronteira == FRONTEIRA_NORTE && celula->volumeFicticio == true ){

                    // Se o NORTE for fronteira impermeavel (u prescrito)
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        A_P = 1.0;
                        A_s = 1.0; 

                        // Outros coeficientes
                        A_e = 0.0; A_w = 0.0; A_n = 0.0;

                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoX = 2.0 * valorCondicaoFronteira( celula, ID_VAR_U, 'n' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoX:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no norte!" << endl;
                    }
                }

                // Volume ficticio (fronteira sul)
                else if( celula->tipoFronteira == FRONTEIRA_SUL && celula->volumeFicticio == true ){

                    // Se o SUL for fronteira impermeavel (u prescrito)
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        A_P = 1.0;
                        A_n = 1.0;
    
                        // Outros coeficientes
                        A_e = 0.0; A_w = 0.0; A_s = 0.0;
    
                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoX = 2.0 * valorCondicaoFronteira( celula, ID_VAR_U, 's' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoX:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no sul!" << endl;
                    }
                }

                // Volumes internos
                else{                    

                    // Calculo dos coeficientes A das interfaces
                    A_e = (0.5 - celula->eastFaceGamma) * (celula->eastFaceRho)
                        * (celula->eastFaceU) * celula->faceLength
                        - (celula->eastFaceMi) * (celula->eastFaceBeta);

                    A_w = -1.0 * (0.5 + celula->westFaceGamma) * (celula->westFaceRho)
                        * (celula->westFaceU) * celula->faceLength
                        - (celula->westFaceMi) * (celula->westFaceBeta);

                    A_n = (0.5 - celula->northFaceGamma) * (celula->northFaceRho) 
                        * (celula->northFaceV) * celula->faceLength
                        - (celula->northFaceMi) * (celula->northFaceBeta);

                    A_s = -1.0 * (0.5 + celula->southFaceGamma) * (celula->southFaceRho)
                        * (celula->southFaceV) * celula->faceLength
                        - (celula->southFaceMi) * (celula->southFaceBeta);

                    S_P = (malha->deltaT) * (-celula->eastFaceMi * celula->eastFaceBeta 
                        -celula->westFaceMi * celula->westFaceBeta) / 3.0;

                    // Calculo do coeficiente A principal
                    A_P = - A_e - A_w - A_n - A_s - S_P 
                        + (celula->rhoAnterior) 
                        * AO_QUADRADO(celula->faceLength) / (malha->deltaT);                    
                }

                // Insere coeficiente principal na lista encadeada
                celula->iniciaElement( A_P, &(celula->firstElementMomentoX) );

                // Insere coeficientes dos vizinhos na mesma lista encadeada
                celula->insereElement( A_e, celula->east, &(celula->firstElementMomentoX) );

                celula->insereElement( A_w, celula->west, &(celula->firstElementMomentoX) );

                celula->insereElement( A_n, celula->north, &(celula->firstElementMomentoX) );

                celula->insereElement( A_s, celula->south, &(celula->firstElementMomentoX) );
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaCoeficientesMomentoX():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaCoeficientesMomentoX():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaTermoFonteMomentoX()

    Calcula o termo fonte do momento-x para cada celula, 
adicionando na matriz [B] do sistema associado ao momento x.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Andre STALTZ e Henrique Fav dia 26/06/07
______________________________________________________________________________*/
int calculaTermoFonteMomentoX( Grid *malha )
{
    Cell *celula;

    // Termo fonte
    double S_c = 0.0,
           S_e = 0.0,
           S_w = 0.0;

    double uVizLeste = 0.0,
           uVizOeste = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            if( !celula->volumeFicticio )
            {
                obtemVariaveisVizinhas( celula, ID_VAR_U, NULL, NULL, &uVizLeste, &uVizOeste );

                // Termo fonte
                S_e = (malha->deltaT) * (celula->eastFaceMi) * (celula->eastFaceBeta) / 3.0;

                S_w = (malha->deltaT) * (celula->westFaceMi) * (celula->westFaceBeta) / 3.0;

                S_c = malha->deltaT * celula->faceLength *
                      ( celula->northFaceMi * dvdx(celula, 'n')
                      - celula->southFaceMi * dvdx(celula, 's') )
                      + malha->deltaT * celula->faceLength * ( 2.0 / 3.0 )
                      * ( -celula->eastFaceMi * dvdy(celula, 'e')
                      + celula->westFaceMi * dvdy(celula, 'w') )
                      - malha->deltaT * celula->faceLength * ( celula->eastFaceP - celula->westFaceP )
                      + S_e * uVizLeste + S_w * uVizOeste;

                // Insere o membro direito da equacao no vetor b da resolucao
                celula->b_MomentoX = S_c + ( celula->rhoAnterior * AO_QUADRADO( celula->faceLength ) 
                                   * celula->uAnterior ) / (malha->deltaT);
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaTermoFonteMomentoX():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaTermoFonteMomentoX():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaCoeficientesMomentoY()

    Calcula os coeficientes A_p^v, A_e^v, A_w^v, A_n^v, A_s^v para cada celula,
montando a matriz [A] do sistema associado 'a correcao de pressao.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Henrique dia 24/04/07
______________________________________________________________________________*/
int calculaCoeficientesMomentoY( Grid *malha )
{
    // Obs: Todos os deltaY/deltaX da discretizacao foram substituidos por 1,
    // visto que a malha do ALG e' sempre quadrada, como na funcao para o momento Y

    // Obs: Aqui tambem, as variaveis nas faces (mi, rho, u) PRECISAM ser os "*" (estimados)

    Cell *celula;
    short tipoFronteira;

    // Coeficientes
    double A_P = 0.0,
           A_e = 0.0,
           A_w = 0.0,
           A_n = 0.0,
           A_s = 0.0;

    double S_P = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            // Evitar caso especial dos quatro cantos ficticios: nao entram no sistema
            if( celula->active )
            {
                // Obtem o identificador que mostra qual o tratamento para a fronteira
                tipoFronteira = tipoCondicaoFronteira( celula, ID_VAR_V );

                // Volume ficticio (fronteira oeste)
                if( celula->tipoFronteira == FRONTEIRA_OESTE && celula->volumeFicticio == true ){

                    // Se no OESTE for entrada de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_ENTRADA_MASSA)
                    {
                        // Entrada de massa, coeficientes importantes
                        A_P = 1.0;
                        A_e = 0.0;
    
                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_w = 0.0;
    
                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoY = valorCondicaoFronteira( celula, ID_VAR_V, 'w' );
                    }

                    // Se o OESTE for saida de massa
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
                    {
                        // Saida de massa, coeficientes importantes
                        A_P = 1.0;
                        A_e = -1.0;
    
                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_w = 0.0;
    
                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoY = 0.0;
                    }

                    // Se no OESTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Coeficientes importantes para essa extrapolacao
                        A_P = 1.0;
                        A_e = -1.0;

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_w = 0.0;

                        // coeficiente B
                        celula->b_MomentoY = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoY:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no oeste!" << endl;
                    }
                }

                // Volume ficticio (fronteira leste)
                else if( celula->tipoFronteira == FRONTEIRA_LESTE && celula->volumeFicticio == true ){

                    // Se no LESTE for saida de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
                    {
                        // Saida de massa, coeficientes importantes
                        A_P = 1.0;
                        A_w = -1.0;
    
                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_e = 0.0;
    
                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoY = 0.0;
                    }

                    // Se no LESTE for entrada de massa
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_ENTRADA_MASSA)
                    {
                        // Entrada de massa, coeficientes importantes
                        A_P = 1.0;
                        A_w = 0.0;
    
                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_e = 0.0;
    
                        // B de acordo com 7.88 do Maliska
                        celula->b_MomentoY = valorCondicaoFronteira( celula, ID_VAR_V, 'e' );
                    }

                    // Se no LESTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Coeficientes importantes para essa extrapolacao
                        A_P = 1.0;
                        A_w = -1.0;

                        // Outros coeficientes
                        A_n = 0.0; A_s = 0.0; A_e = 0.0;

                        // coeficiente B
                        celula->b_MomentoY = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoY:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no leste!" << endl;
                    }
                }

                // Volume ficticio (fronteira norte)
                else if( celula->tipoFronteira == FRONTEIRA_NORTE && celula->volumeFicticio == true ){

                    // Se o NORTE for fronteira impermeavel (v prescrito)
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        A_P = 1.0;
                        A_s = 1.0; 
    
                        // Outros coeficientes
                        A_e = 0.0; A_w = 0.0; A_n = 0.0;
    
                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoY = 2.0 * valorCondicaoFronteira( celula, ID_VAR_V, 'n' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoY:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no norte!" << endl;
                    }
                }

                // Volume ficticio (fronteira sul)
                else if( celula->tipoFronteira == FRONTEIRA_SUL && celula->volumeFicticio == true ){

                    // Se o SUL for fronteira impermeavel (v prescrito)
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        A_P = 1.0;
                        A_n = 1.0;
    
                        // Outros coeficientes
                        A_e = 0.0; A_w = 0.0; A_s = 0.0;
    
                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoY = 2.0 * valorCondicaoFronteira( celula, ID_VAR_V, 's' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesMomentoY:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no sul!" << endl;
                    }
                }

                // Volumes internos
                else{
                    // Calculo dos coeficientes A das interfaces
                    A_e = (0.5 - celula->eastFaceGamma) * (celula->eastFaceRho)
                        * (celula->eastFaceU) * celula->faceLength
                        - (celula->eastFaceMi) * (celula->eastFaceBeta);

                    A_w = -1.0 * (0.5 + celula->westFaceGamma) * (celula->westFaceRho)
                        * (celula->westFaceU) * celula->faceLength
                        - (celula->westFaceMi) * (celula->westFaceBeta);

                    A_n = (0.5 - celula->northFaceGamma) * (celula->northFaceRho)
                        * (celula->northFaceV) * celula->faceLength
                        - (celula->northFaceMi) * (celula->northFaceBeta);

                    A_s = -1.0 * (0.5 + celula->southFaceGamma) * (celula->southFaceRho)
                        * (celula->southFaceV) * celula->faceLength
                        - (celula->southFaceMi) * (celula->southFaceBeta);

                    S_P = (malha->deltaT) * (-celula->northFaceMi*celula->northFaceBeta 
                                        -celula->southFaceMi*celula->southFaceBeta) / 3.0;

                    // Calculo do coeficiente A principal
                    A_P = - A_e - A_w - A_n - A_s - S_P 
                        + (celula->rhoAnterior)
                        * AO_QUADRADO(celula->faceLength) / (malha->deltaT);
                }

                // Insere coeficiente principal na lista encadeada
                celula->iniciaElement( A_P, &(celula->firstElementMomentoY) );

                // Insere coeficientes dos vizinhos na mesma lista encadeada
                celula->insereElement( A_e, celula->east, &(celula->firstElementMomentoY) );

                celula->insereElement( A_w, celula->west, &(celula->firstElementMomentoY) );

                celula->insereElement( A_n, celula->north, &(celula->firstElementMomentoY) );

                celula->insereElement( A_s, celula->south, &(celula->firstElementMomentoY) );
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaCoeficientesMomentoY():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaCoeficientesMomentoY():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaTermoFonteMomentoY()

    Calcula o termo fonte do momento-y para cada celula, 
adicionando na matriz [B] do sistema associado ao momento y.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Andre STALTZ e Henrique Fav dia 26/06/07
______________________________________________________________________________*/
int calculaTermoFonteMomentoY( Grid *malha )
{
    Cell *celula;

    // Termo fonte
    double S_c = 0.0,
           S_P = 0.0,
           S_n = 0.0,
           S_s = 0.0;

    double vVizNorte = 0.0,
           vVizSul = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            if( !celula->volumeFicticio )
            {
                obtemVariaveisVizinhas( celula, ID_VAR_V, &vVizNorte, &vVizSul, NULL, NULL );

                // Termo fonte
                S_n = (malha->deltaT) * (celula->northFaceMi) * (celula->northFaceBeta) / 3.0;

                S_s = (malha->deltaT) * (celula->southFaceMi) * (celula->southFaceBeta) / 3.0;

                S_c = malha->deltaT * celula->faceLength *
                    ( celula->eastFaceMi * dudy(celula, 'e') - celula->westFaceMi * dudy(celula, 'w') ) +
                    malha->deltaT * celula->faceLength * ( 2.0 / 3.0 ) *
                    ( -celula->northFaceMi * dudx(celula, 'n') + celula->southFaceMi * dudx(celula,'s'))
                    - malha->deltaT * celula->faceLength * ( celula->northFaceP - celula->southFaceP )
                    + S_n * vVizNorte + S_s * vVizSul;

                // Insere o membro direito da equacao no vetor b da resolucao
                celula->b_MomentoY = S_c + ( celula->rhoAnterior * AO_QUADRADO( celula->faceLength ) 
                            * celula->vAnterior ) / malha->deltaT;
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaTermoFonteMomentoY():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaTermoFonteMomentoY():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaVelocidadesNasFaces()

    Calcula o valor de u e v em todas as interfaces da malha, atraves da media
ponderada entre os valores de u nos centros das celulas correspondentes. O peso
usado e' o coeficiente A_P da equacao do momento.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Henrique dia 25/04/07
Atualizado pro Andre Staltz dia 29/05/07
______________________________________________________________________________*/
int calculaVelocidadesNasFaces( Grid *malha )
{
    // Celula da malha
    Cell *celula;

    // A_P de uma celula e da sua vizinha
    double A_P_u = 0.0,
           A_P_v = 0.0,
           A_P_viz = 0.0;

    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            if( celula->volumeFicticio == false )
            {
                // Obtem o A_P^u e o A_P^v da celula atual
                if( celula->firstElementMomentoX != NULL
                ||  celula->firstElementMomentoY != NULL ) // Verifica erro
                {
                    A_P_u = celula->firstElementMomentoX->value;
                    A_P_v = celula->firstElementMomentoY->value;
                }
                else
                {
                    IMPRIMIR_ERRO_ELEMENT_NULO();
                    cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
                    cout << " celula: " << celula->gridPosition << endl;
                    return ERRO_ELEMENT_NULO;
                }

                // --- Calcula velocidade v na face norte ---
                // Obtem o A_P_N
                if( celula->north->firstElementMomentoY != NULL )
                    A_P_viz = celula->north->firstElementMomentoY->value;
                else
                {
                    IMPRIMIR_ERRO_ELEMENT_NULO();
                    cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
                    cout << " celula->north: " << celula->north->gridPosition;
                    return ERRO_ELEMENT_NULO;
                }

                // Obtem o I_n
                celula->I_n = A_P_v * celula->vAtual + A_P_viz * celula->north->vAtual;

                // Calcula v_n para essa celula
                celula->northFaceV = celula->I_n / ( A_P_v + A_P_viz );
            
                // --- Calcula velocidade v na face sul ---
                // Obtem o A_P_S
                if( celula->south->firstElementMomentoY != NULL )
                    A_P_viz = celula->south->firstElementMomentoY->value;
                else
                {
                    IMPRIMIR_ERRO_ELEMENT_NULO();
                    cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
                    cout << " celula->south: " << celula->south->gridPosition;
                    return ERRO_ELEMENT_NULO;
                }

                // Obtem o I_s
                celula->I_s = A_P_v * celula->vAtual + A_P_viz * celula->south->vAtual;

                // Calcula v_s para essa celula
                celula->southFaceV = celula->I_s / ( A_P_v + A_P_viz );
            
                // --- Calcula velocidade u na face leste ---
                // Obtem o A_P_E
                if( celula->east->firstElementMomentoX != NULL )
                    A_P_viz = celula->east->firstElementMomentoX->value;
                else
                {
                    IMPRIMIR_ERRO_ELEMENT_NULO();
                    cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
                    return ERRO_ELEMENT_NULO;
                }

                // Obtem o I_e por media ponderada
                celula->I_e = A_P_u * celula->uAtual + A_P_viz * celula->east->uAtual;

                // Calcula u_e para essa celula
                celula->eastFaceU = celula->I_e / ( A_P_u + A_P_viz );

                // --- Calcula velocidade na face norte ---
                // Obtem o A_P_W
                if( celula->west->firstElementMomentoX != NULL )
                    A_P_viz = celula->west->firstElementMomentoX->value;
                else
                {
                    IMPRIMIR_ERRO_ELEMENT_NULO();
                    cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
                    cout << " celula->west: " << celula->west->gridPosition;
                    return ERRO_ELEMENT_NULO;
                }

                // Obtem o I_w
                celula->I_w = A_P_u * celula->uAtual + A_P_viz * celula->west->uAtual;

                // Calcula u_w para essa celula
                celula->westFaceU = celula->I_w / ( A_P_u + A_P_viz );           
            }
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em calculaVelocidadesNasFaces()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaCoeficientesCorrecaoPressao()

    Calcula os coeficientes L_p, L_e, L_w, L_n, L_s para cada celula, 
montando a matriz [A] do sistema associado 'a correcao de pressao.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Andre STALTZ dia 31/03/07
______________________________________________________________________________*/
int calculaCoeficientesCorrecaoPressao( Grid *malha, double relaxamentoU = 1.0, 
                                        double relaxamentoV = 1.0 )
{
    Cell *celula;
    short tipoFronteira;

    // Declaracao dos fatores inclusos nos coeficientes
    double m_P_rho = 0.0,
           m_e_rho = 0.0,
           m_w_rho = 0.0,
           m_n_rho = 0.0,
           m_s_rho = 0.0;

    double m_e_u = 0.0,
           m_w_u = 0.0,
           m_n_v = 0.0,
           m_s_v = 0.0;

    double C_P_rho = 0.0,
           C_E_rho = 0.0,
           C_W_rho = 0.0,
           C_N_rho = 0.0,
           C_S_rho = 0.0;

    double L_P = 0.0,
           L_e = 0.0,
           L_w = 0.0,
           L_n = 0.0,
           L_s = 0.0;

    double rhoVizLeste = 0.0,
           rhoVizOeste = 0.0,
           rhoVizNorte = 0.0,
           rhoVizSul = 0.0;

    double TVizLeste = 0.0,
           TVizOeste = 0.0,
           TVizNorte = 0.0,
           TVizSul = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            // Evitar caso especial dos quatro cantos ficticios: nao entram no sistema
            if( celula->active )
            {
                // Obtem o identificador que mostra qual o tratamento para a fronteira
                tipoFronteira = tipoCondicaoFronteira( celula, ID_VAR_PLINHA );

                // Volume ficticio (fronteira oeste)
                if( celula->tipoFronteira == FRONTEIRA_OESTE && celula->volumeFicticio == true ){

                    // Se o OESTE for entrada de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_ENTRADA_MASSA)
                    {
                        // Entrada de massa, coeficientes importantes
                        L_P = 1.0;
                        L_e = 0.0;
    
                        // Outros coeficientes
                        L_n = 0.0; L_s = 0.0; L_w = 0.0;
                    }

                    // Se no OESTE for prescrita
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Entrada de massa, coeficientes importantes
                        L_P = 1.0;
                        L_e = 1.0;
    
                        // Outros coeficientes
                        L_n = 0.0; L_s = 0.0; L_w = 0.0;

                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoY = 2.0 * valorCondicaoFronteira( celula, ID_VAR_PLINHA, 'w');
                    }

                    // Se no OESTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Coeficientes importantes para a extrapolacao
                        L_P = 1.0;
                        L_e = -1.0;
    
                        // Outros coeficientes
                        L_n = 0.0; L_s = 0.0; L_w = 0.0;
                    }
                }

                // Volume ficticio (fronteira leste)
                else if( celula->tipoFronteira == FRONTEIRA_LESTE && celula->volumeFicticio == true ){

                    // Se o LESTE for saida de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
                    {
                        // Saida de massa, coeficientes importantes
                        L_P = 1.0;
                        L_w = -1.0;
    
                        // Outros coeficientes
                        L_n = 0.0; L_s = 0.0; L_e = 0.0;
                    }
                    // Se o LESTE for prescrito
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Coeficientes importantes para condicao dirichlet
                        L_P = 1.0;
                        L_w = 1.0;
    
                        // Outros coeficientes
                        L_n = 0.0; L_s = 0.0; L_e = 0.0;

                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoY = 2.0 * valorCondicaoFronteira( celula, ID_VAR_PLINHA, 'e' );
                    }
                    // Se o LESTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Coeficientes importantes para a extrapolacao
                        L_P = 1.0;
                        L_w = -1.0;
    
                        // Outros coeficientes
                        L_n = 0.0; L_s = 0.0; L_e = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesCorrecaoPressao:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no leste!" << endl;
                    }
                }

                // Volume ficticio (fronteira norte)
                else if( celula->tipoFronteira == FRONTEIRA_NORTE && celula->volumeFicticio == true ){
                    
                    // Se no NORTE for prescrito
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Coeficientes importantes para condicao dirichlet
                        L_P = 1.0;
                        L_s = 1.0;
    
                        // Outros coeficientes
                        L_w = 0.0; L_n = 0.0; L_e = 0.0;

                        // B de acordo com 7.84 do Maliska
                        celula->b_MomentoY = 2.0 * valorCondicaoFronteira( celula, ID_VAR_PLINHA, 'n');
                    }
                    // Se o NORTE for extrapolado
                    else if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        L_P = 1.0;
                        L_s = -1.0; 
    
                        // Outros coeficientes
                        L_e = 0.0; L_w = 0.0; L_n = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesCorrecaoPressao:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no norte!" << endl;
                    }
                }

                // Volume ficticio (fronteira sul)
                else if( celula->tipoFronteira == FRONTEIRA_SUL && celula->volumeFicticio == true ){

                    // Se o SUL for extrapolado
                    if(tipoFronteira == CONDICAO_FRONTEIRA_EXTRAPOLADA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        L_P = 1.0;
                        L_n = -1.0;
    
                        // Outros coeficientes
                        L_e = 0.0; L_w = 0.0; L_s = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesCorrecaoPressao:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no sul!" << endl;
                    }
                }

                // Volumes internos
                else{

                    obtemVariaveisVizinhas( celula, ID_VAR_RHO, &rhoVizNorte, &rhoVizSul,
                                                            &rhoVizLeste, &rhoVizOeste );
                    obtemVariaveisVizinhas( celula, ID_VAR_T, &TVizNorte, &TVizSul,
                                                            &TVizLeste, &TVizOeste );

                    // Fator m_(P,e,w,n,s)_rho que depende de (u,v)_(e,w,n,s)^* e os gammas ---
                    m_P_rho = AO_QUADRADO( celula->faceLength ) / malha->deltaT
                            + celula->faceLength 
                            *(
                                 (0.5 + celula->eastFaceGamma) * celula->eastFaceU
                                -(0.5 - celula->westFaceGamma) * celula->westFaceU
                                +(0.5 + celula->northFaceGamma) * celula->northFaceV
                                -(0.5 - celula->southFaceGamma) * celula->southFaceV
                             );

                    m_e_rho = celula->faceLength * celula->eastFaceU
                            * (0.5 - celula->eastFaceGamma);

                    m_w_rho = -1.0 * celula->faceLength * celula->westFaceU
                            * (0.5 + celula->westFaceGamma);

                    m_n_rho = celula->faceLength * celula->northFaceV
                            * (0.5 - celula->northFaceGamma);

                    m_s_rho = -1.0 * celula->faceLength * celula->southFaceV
                            * (0.5 + celula->southFaceGamma);

                    // Fator C_(P,E,W,N,S)_rho que depende da temperatura e da equacao de estado 
                    C_P_rho = 1.0 / (R * celula->TAtual);
                    C_E_rho = 1.0 / (R * TVizLeste);
                    C_W_rho = 1.0 / (R * TVizOeste);
                    C_N_rho = 1.0 / (R * TVizNorte);
                    C_S_rho = 1.0 / (R * TVizSul);

                    // Fatores m_(e,w,n,s)^(u,v) que dependem de rho_(E,W,N,S)^* e os gammas
                    m_e_u = celula->faceLength
                            *(
                                (0.5 + celula->eastFaceGamma) * celula->rhoAtual
                                +(0.5 - celula->eastFaceGamma) * rhoVizLeste
                             );

                    m_w_u = -1.0 * celula->faceLength
                            *(
                                (0.5 + celula->westFaceGamma) * rhoVizOeste
                                +(0.5 - celula->westFaceGamma) * celula->rhoAtual
                             );

                    m_n_v = celula->faceLength
                            *(
                                (0.5 + celula->northFaceGamma) * celula->rhoAtual
                                +(0.5 - celula->northFaceGamma) * rhoVizNorte
                             );

                    m_s_v = -1.0 * celula->faceLength
                            *(
                                (0.5 + celula->southFaceGamma) * rhoVizSul
                                +(0.5 - celula->southFaceGamma) * celula->rhoAtual
                             );

                    // Fatores d_(e,w,n,s) que dependem de I_(e,w,n,s), dado pelo parametro

                    // OBS: FOI EVITADA A DIVISAO POR ZERO, MAS ISTO E' QUESTIONAVEL
                    celula->d_e = ( (celula->I_e == 0.0) ? 0.0 
                                        : 
                                    (celula->faceLength / celula->I_e) * relaxamentoU );

                    celula->d_w = ( (celula->I_w == 0.0) ? 0.0 
                                        : 
                                    (celula->faceLength / celula->I_w) * relaxamentoU );

                    celula->d_n = ( (celula->I_n == 0.0) ? 0.0 
                                        : 
                                    (celula->faceLength / celula->I_n) * relaxamentoV );

                    celula->d_s = ( (celula->I_s == 0.0) ? 0.0 
                                        : 
                                    (celula->faceLength / celula->I_s) * relaxamentoV );

                    // Calcula L_P ==================================================
                    L_P =   m_P_rho * C_P_rho
                            + m_e_u * celula->d_e
                            - m_w_u * celula->d_w
                            + m_n_v * celula->d_n
                            - m_s_v * celula->d_s;

                    // Calcula L_(e,w,n,s) =========================================== 
                    L_e = -1.0 * (-1.0 * m_e_rho * C_E_rho + m_e_u * celula->d_e);
                    L_w = -1.0 * (-1.0 * m_w_rho * C_W_rho - m_w_u * celula->d_w);
                    L_n = -1.0 * (-1.0 * m_n_rho * C_N_rho + m_n_v * celula->d_n);
                    L_s = -1.0 * (-1.0 * m_s_rho * C_S_rho - m_s_v * celula->d_s);
                }

                // Zerar (iniciar) o vetor B da equacao AX=B da pressao'
                celula->b_PressaoLinha = 0.0;

                /* Adicionar L_P na matriz de coeficientes de pressao */
                celula->iniciaElement( L_P, &(celula->firstElementPressaoLinha) );

                // Insere coeficientes dos vizinhos na mesma lista encadeada
                celula->insereElement( L_e, celula->east, &(celula->firstElementPressaoLinha) );

                celula->insereElement( L_w, celula->west, &(celula->firstElementPressaoLinha) );

                celula->insereElement( L_n, celula->north, &(celula->firstElementPressaoLinha) );

                celula->insereElement( L_s, celula->south, &(celula->firstElementPressaoLinha) );
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaCoeficientesCorrecaoPressao():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaCoeficientesCorrecaoPressao():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaTermoFonteCorrecaoPressao()

    Calcula o termo fonte b^(P') da equacao de correcao de pressao usando as
velocidades u* e v* que acabaram de ser calculados na etapa anterior. Esse termo
e' somado na matriz [B] do sistema associado a correcao de pressao.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Andre STALTZ dia 31/03/07
______________________________________________________________________________*/
int calculaTermoFonteCorrecaoPressao( Grid *malha )
{
    Cell *celula;

    // Declaracao dos fatores inclusos no termo fonte
    double m_P_rho = 0.0,
           m_e_rho = 0.0,
           m_w_rho = 0.0,
           m_n_rho = 0.0,
           m_s_rho = 0.0;

    double b_P = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            // Volumes internos
            if( celula->volumeFicticio == false ){

                double rhoVizLeste = 0.0, rhoVizOeste = 0.0,
                       rhoVizNorte = 0.0, rhoVizSul = 0.0;

                obtemVariaveisVizinhas( celula, ID_VAR_RHO, &rhoVizNorte, &rhoVizSul,
                                                        &rhoVizLeste, &rhoVizOeste );

                // Fator m_(P,e,w,n,s)_rho que depende de (u,v)_(e,w,n,s)^* e os gammas ---
                m_P_rho = AO_QUADRADO( celula->faceLength ) / malha->deltaT
                        + celula->faceLength
                        *(
                            (0.5 + celula->eastFaceGamma) * celula->eastFaceU
                           -(0.5 - celula->westFaceGamma) * celula->westFaceU
                           +(0.5 + celula->northFaceGamma) * celula->northFaceV
                           -(0.5 - celula->southFaceGamma) * celula->southFaceV
                        );

                m_e_rho = celula->faceLength * celula->eastFaceU
                        * (0.5 - celula->eastFaceGamma);

                m_w_rho = -1.0 * celula->faceLength * celula->westFaceU
                        * (0.5 + celula->westFaceGamma);

                m_n_rho = celula->faceLength * celula->northFaceV
                        * (0.5 - celula->northFaceGamma);

                m_s_rho = -1.0 * celula->faceLength * celula->southFaceV
                        * (0.5 + celula->southFaceGamma);

                b_P = celula->rhoAnterior *
                        ( AO_QUADRADO( celula->faceLength ) / malha->deltaT )
                    - m_P_rho * celula->rhoAtual
                    - m_e_rho * rhoVizLeste
                    - m_w_rho * rhoVizOeste
                    - m_n_rho * rhoVizNorte
                    - m_s_rho * rhoVizSul;

                /* Adicionar b_P na matriz [B_pressao] na linha (celula) */
                celula->b_PressaoLinha += b_P;
            }
            // Nos volumes ficticios
            else
                celula->b_PressaoLinha = 0.0;
        }
        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaTermoFonteCorrecaoPressao():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaTermoFonteCorrecaoPressao():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                corrige_Velocidades_Densidades_Pressoes()

    Corrige os valores das velocidades em todas as interfaces da malha, usando a
correcao de pressao, atraves de media das velocidades anteriores ponderadas
pelos respectivos coeficientes na matriz. Em seguida, atraves de uma media
aritmetica, obtem os novos valores de velocidade nos centros das celulas.
    Corrige tambem os valores das densidades em todas as celulas da malha,
atraves da equacao de estado e, por ultimo, calcula um novo campo de pressoes.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Henrique dia 08/05/07
______________________________________________________________________________*/
int corrige_Velocidades_Densidades_Pressoes( Grid *malha, double relaxamentoP = 1.0,
                                             double relaxamentoU = 1.0, double relaxamentoV = 1.0 )
{
    // Celula da malha
    Cell *celula;
    short tipoFronteira;

    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            // Correcao para os volumes internos
            if( celula->active && celula->volumeFicticio == false )
            {
                // Corrige velocidade u na interface leste da celula atual
                // Se nao esta na fronteira leste
                if( celula->east->type == 'b' && celula->east->active == true)
                    celula->eastFaceU -= celula->d_e  
                                        * ( celula->east->pLinha - celula->pLinha );

                // Corrige velocidade u na interface oeste da celula atual
                // Se nao esta na fronteira oeste
                if( celula->west->type == 'b' && celula->west->active == true)
                    celula->westFaceU -= celula->d_w 
                                        * ( celula->pLinha - celula->west->pLinha );

                // Corrige velocidade v na interface norte da celula atual
                // Se nao esta na fronteira norte
                if( celula->north->type == 'b' && celula->north->active == true)
                    celula->northFaceV -= celula->d_n 
                                         * ( celula->north->pLinha - celula->pLinha );

                // Corrige velocidade v na interface sul da celula atual
                // Se nao esta na fronteira sul
                if( celula->south->type == 'b' && celula->south->active == true)
                    celula->southFaceV -= celula->d_s 
                                         * ( celula->pLinha - celula->south->pLinha );

                // Corrige as velocidades no centro da celula atraves de media das interfaces
                celula->uAtual = ( celula->eastFaceU + celula->westFaceU ) / 2.0;
                celula->uAtual =  relaxamentoU * celula->uAtual
                               + (1.0 - relaxamentoU) * celula->uIterAnterior;

                celula->vAtual = ( celula->northFaceV + celula->southFaceV ) / 2.0;
                celula->vAtual =  relaxamentoV * celula->vAtual
                               + (1.0 - relaxamentoV) * celula->vIterAnterior;

                // Finalmente, corrige a pressao no centro da celula atual
                celula->pAtual += relaxamentoP * celula->pLinha;

                // Corrige a densidade no centro da celula atual
                celula->rhoAtual += (relaxamentoP * celula->pLinha) / ( R * celula->TAtual );
            }
            else if(celula->tipoFronteira == FRONTEIRA_SUL){
                //cout << "Temp do ficticio sul = "<< celula->TAtual << "\n";
                //cout << "Temp no vizinho norte do ficticio sul = "<< celula->north->TAtual << "\n";
                
            }
        }

        // Percorre as celulas que sao volumes ficticios da fronteira leste do dominio
        // para calcular a correcao de variaveis
        for(celula = malha->firstGridCell->south;
            celula->active;
            celula = celula->south)
        {
            // Obtem o identificador que mostra qual o tratamento para a fronteira
            tipoFronteira = tipoCondicaoFronteira( celula, ID_VAR_U );

            // Se no LESTE for saida de massa
            if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
            {   
                // u no leste
                celula->uAtual += (celula->west->uAtual - celula->west->uIterAnterior);

                // v no leste
                celula->vAtual += (celula->west->vAtual - celula->west->vIterAnterior);
            }
            // Imprime erro de programacao
            else
            {
                cout << "Na funcao corrige_Velocidades_Densidades_Pressoes:" << endl
                        << "Tipo de fronteira " << tipoFronteira 
                        << " nao implementado no leste!" << endl;
            }

            celula->uAtual =  relaxamentoU * celula->uAtual
                            + (1.0 - relaxamentoU) * celula->uIterAnterior;

            celula->vAtual =  relaxamentoV * celula->vAtual
                            + (1.0 - relaxamentoV) * celula->vIterAnterior;

            // densidade no leste
            celula->rhoAtual += (relaxamentoP * celula->pLinha) / ( R * celula->TAtual );

            // pressao no leste
            celula->pAtual += relaxamentoP * celula->pLinha;
        }

        // Percorre as celulas que sao volumes ficticios da fronteira sul do dominio
        // para calcular a correcao de variaveis
        for(celula = celula->west;
            celula->active;
            celula = celula->west)
        {
            // Obtem o identificador que mostra qual o tratamento para a fronteira
            tipoFronteira = tipoCondicaoFronteira( celula, ID_VAR_U );

            // Se no SUL for prescrito
            if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
            {   
                // u no sul
                celula->uAtual -= (celula->north->uAtual - celula->north->uIterAnterior);

                // v no sul
                celula->vAtual -= (celula->north->vAtual - celula->north->vIterAnterior);
            }
            // Imprime erro de programacao
            else
            {
                cout << "Na funcao corrige_Velocidades_Densidades_Pressoes:" << endl
                        << "Tipo de fronteira " << tipoFronteira 
                        << " nao implementado no sul!" << endl;
            }

            celula->uAtual =  relaxamentoU * celula->uAtual
                            + (1.0 - relaxamentoU) * celula->uIterAnterior;

            celula->vAtual =  relaxamentoV * celula->vAtual
                            + (1.0 - relaxamentoV) * celula->vIterAnterior;

            // densidade no sul
            celula->rhoAtual += (relaxamentoP * celula->pLinha) / ( R * celula->TAtual );

            // pressao no sul
            celula->pAtual += relaxamentoP * celula->pLinha;
        
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao corrige_Velocidades_Densidades_Pressoes():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao corrige_Velocidades_Densidades_Pressoes():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                   calculaCoeficientesEnergia()

    Calcula os coeficientes F_p, F_e, F_w, F_n, F_s para cada celula da malha
e constroi a matriz da energia. Calcula tambem o termo fonte B da equacao da
energia para cada celula e constroi o vetor b da energia.

    Parametro:
        malha: apontador para a malha em questao;

Criado por Henrique dia 14/05/07.
Atualizado por Henrique dia 12/06/07.
______________________________________________________________________________*/
int calculaCoeficientesEnergia( Grid *malha )
{
    Cell *celula;
    short tipoFronteira;

    // Coeficientes
    double F_P = 0.0,
           F_e = 0.0,
           F_w = 0.0,
           F_n = 0.0,
           F_s = 0.0;
int bla;
    // Tensoes
    double tauxx_e = 0.0,
           tauxx_w = 0.0,
           tauxy_e = 0.0,
           tauxy_w = 0.0,
           tauxy_n = 0.0,
           tauxy_s = 0.0,
           tauyy_n = 0.0,
           tauyy_s = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            // Evitar caso especial dos quatro cantos ficticios: nao entram no sistema
            if( celula->active )
            {
                // Obtem o identificador que mostra qual o tratamento para a fronteira
                tipoFronteira = tipoCondicaoFronteira( celula, ID_VAR_Et );
    
                // Volume ficticio (fronteira oeste)
                if( celula->tipoFronteira == FRONTEIRA_OESTE && celula->volumeFicticio == true )
                {
                    // Se no OESTE for entrada de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_ENTRADA_MASSA)
                    {
                        // Entrada de massa, coeficientes importantes
                        F_P = 1.0;
                        F_e = 0.0;
    
                        // Outros coeficientes
                        F_n = 0.0; F_s = 0.0; F_w = 0.0;
    
                        // B de acordo com 7.88 do Maliska
                        celula->b_Energia = valorCondicaoFronteira( celula, ID_VAR_Et, 'w' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesEnergia:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no oeste!" << endl;
                    }
                }

                // Volume ficticio (fronteira leste)
                else if( celula->tipoFronteira == FRONTEIRA_LESTE && celula->volumeFicticio == true )
                {
                    // Se no LESTE for saida de massa
                    if(tipoFronteira == CONDICAO_FRONTEIRA_SAIDA_MASSA)
                    {
                        // Saida de massa, coeficientes importantes
                        F_P = 1.0;
                        F_w = -1.0;
    
                        // Outros coeficientes
                        F_n = 0.0; F_s = 0.0; F_e = 0.0;
    
                        // B de acordo com 7.88 do Maliska
                        celula->b_Energia = 0.0;
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesEnergia:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no leste!" << endl;
                    }
                }

                // Volume ficticio (fronteira norte)
                else if( celula->tipoFronteira == FRONTEIRA_NORTE && celula->volumeFicticio == true )
                {
                    // Se no NORTE for prescrito
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        F_P = 1.0;
                        F_s = 1.0;
    
                        // Outros coeficientes
                        F_e = 0.0; F_w = 0.0; F_n = 0.0;
    
                        // B de acordo com 7.84 do Maliska
                        celula->b_Energia = 2.0 * valorCondicaoFronteira( celula, ID_VAR_Et, 'n' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesEnergia:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no norte!" << endl;
                    }
                }

                // Volume ficticio (fronteira sul)
                else if( celula->tipoFronteira == FRONTEIRA_SUL && celula->volumeFicticio == true )
                {
                    // Se no SUL for prescrito
                    if(tipoFronteira == CONDICAO_FRONTEIRA_PRESCRITA)
                    {
                        // Fronteira impermeavel, coeficientes importantes
                        F_P = 1.0;
                        F_n = 1.0;
    
                        // Outros coeficientes
                        F_e = 0.0; F_w = 0.0; F_s = 0.0;
    
                        // B de acordo com 7.84 do Maliska
                        celula->b_Energia = 2.0 * valorCondicaoFronteira( celula, ID_VAR_Et, 's' );
                    }
                    // Imprime erro de programacao
                    else
                    {
                        cout << "Na funcao calculaCoeficientesEnergia:" << endl
                             << "Tipo de fronteira " << tipoFronteira 
                             << " nao implementado no sul!" << endl;
                    }
                }

                // Volumes internos
                else
                {
                    // Calculo dos coeficientes da equacao da energia
                    F_P = 1.0 + ( malha->deltaT / malha->sideLength ) * (
                                    celula->eastFaceU * ( 0.5 + celula->eastFaceGamma ) -
                                    celula->westFaceU * ( 0.5 - celula->westFaceGamma ) +
                                    celula->northFaceV * ( 0.5 + celula->northFaceGamma ) -
                                    celula->southFaceV * ( 0.5 - celula->southFaceGamma )
                                );
                    F_e = ( malha->deltaT / malha->sideLength )
                        * celula->eastFaceU * ( 0.5 - celula->eastFaceGamma );
                    F_w = - ( malha->deltaT / malha->sideLength )
                        * celula->westFaceU * ( 0.5 + celula->westFaceGamma );
                    F_n = ( malha->deltaT / malha->sideLength )
                        * celula->northFaceV * ( 0.5 - celula->northFaceGamma );
                    F_s = - ( malha->deltaT / malha->sideLength )
                        * celula->southFaceV * ( 0.5 + celula->southFaceGamma );

                    // Calculo das pressoes nas interfaces da celula
                    celula->northFaceP = interpolaVariavelFace(celula, ID_VAR_P, 'n');
                    celula->southFaceP = interpolaVariavelFace(celula, ID_VAR_P, 's');
                    celula->eastFaceP  = interpolaVariavelFace(celula, ID_VAR_P, 'e');
                    celula->westFaceP  = interpolaVariavelFace(celula, ID_VAR_P, 'w');

                    // Calculo das tensoes nas interfaces
                    tauxx_e = celula->eastFaceMi * (
                                                    (4.0 / 3.0) * dudx(celula, 'e')
                                                    - (2.0 / 3.0) * dvdy(celula, 'e')
                                                );
                    tauxx_w = celula->westFaceMi * (
                                                    (4.0 / 3.0) * dudx(celula, 'w')
                                                    - (2.0 / 3.0) * dvdy(celula, 'w')
                                                );

                    tauxy_e = celula->eastFaceMi * ( dudy(celula, 'e') + dvdx(celula, 'e') );
                    tauxy_w = celula->westFaceMi * ( dudy(celula, 'w') + dvdx(celula, 'w') );
                    tauxy_n = celula->northFaceMi * ( dudy(celula, 'n') + dvdx(celula, 'n') );
                    tauxy_s = celula->southFaceMi * ( dudy(celula, 's') + dvdx(celula, 's') );

                    tauyy_n = celula->northFaceMi * (
                                                    -(2.0 / 3.0) * dudx(celula, 'n')
                                                    +(4.0 / 3.0) * dvdy(celula, 'n')
                                                    );
                    tauyy_s = celula->southFaceMi * (
                                                    -(2.0 / 3.0) * dudx(celula, 's')
                                                    +(4.0 / 3.0) * dvdy(celula, 's')
                                                    );

                    // Calcula o termo fonte da equacao da energia
                    celula->b_Energia = celula->EtAnterior - ( malha->deltaT / malha->sideLength )
                                        * (
                                              celula->eastFaceP * celula->eastFaceU
                                            - celula->eastFaceU * tauxx_e
                                            - celula->eastFaceV * tauxy_e
                                            + (- celula->eastFaceK * dTdx(celula, 'e')) // q_x_e
                                            - celula->westFaceP * celula->westFaceU
                                            + celula->westFaceU * tauxx_w
                                            + celula->westFaceV * tauxy_w
                                            - (- celula->westFaceK * dTdx(celula, 'w')) // q_x_w
                                            + celula->northFaceP * celula->northFaceV
                                            - celula->northFaceV * tauyy_n
                                            - celula->northFaceU * tauxy_n
                                            + (- celula->northFaceK * dTdy(celula, 'n')) // q_y_n
                                            - celula->southFaceP * celula->southFaceV
                                            + celula->southFaceV * tauyy_s
                                            + celula->southFaceU * tauxy_s
                                            - (- celula->southFaceK * dTdy(celula, 's')) // q_y_s
                                        );
                }

                // Insere coeficiente F_P na lista encadeada
                celula->iniciaElement( F_P, &(celula->firstElementEnergia) );

                // Insere coeficientes dos vizinhos na mesma lista encadeada
                celula->insereElement( F_e, celula->east, &(celula->firstElementEnergia) );

                celula->insereElement( F_w, celula->west, &(celula->firstElementEnergia) );

                celula->insereElement( F_n, celula->north, &(celula->firstElementEnergia) );

                celula->insereElement( F_s, celula->south, &(celula->firstElementEnergia) );
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao calculaCoeficientesEnergia():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao calculaCoeficientesEnergia():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  atualizaOutrosEscalares()

   Atualiza os campos de temperatura, densidade, condutividade, viscosidade ao 
final de uma iteracao da equacao da energia.

       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 14/05/07.
Atualizado por Henrique dia 12/06/07.
______________________________________________________________________________*/
int atualizaOutrosEscalares( Grid *malha, double relaxamentoT )
{
    Cell *celula;

    double energiaInterna = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            if( celula->active )
            {
                // calcula energia interna
                energiaInterna = ( celula->EtAtual / celula->rhoAtual ) 
                             - 0.5 * ( AO_QUADRADO(celula->uAtual) + AO_QUADRADO(celula->vAtual) );

                // calcula temperatura
                celula->TAtual = (1.0 - relaxamentoT) * celula->TAtual
                                  + relaxamentoT * energiaInterna / ( R / (GAMMA-1.0) );

                // calcula densidade
                celula->rhoAtual = celula->pAtual / ( R * celula->TAtual );

                // Calcula viscosidade
                celula->miAtual = MI_ZERO
                                * pow( celula->TAtual / TEMPERATURA_LIVRE, 3.0/2.0 )
                                * (
                                    (TEMPERATURA_LIVRE + 110.0)
                                    /(celula->TAtual + 110.0)
                                );

                // Calcula condutividade termica
                celula->kAtual = (celula->miAtual * GAMMA * (R / (GAMMA-1.0)) ) / PRANDTL;
            }
        }

        return SUCESSO;
    }

    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em atualizaOutrosEscalares()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em atualizaOutrosEscalares()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                              obtemVariaveisVizinhas()
                        
    Obtem (por parametro) os valores de uma certa variavel nas celulas vizinhas
a celula atual. Essa funcao assume uma malha UNIFORME, mas considera
tambem condicoes de fronteira. Retorna codigo de erro ou sucesso.

    Parametros:
         celula: a celula centrada em torno dos vizinhos
         id_variavel: um identificador para saber qual variavel esta sendo
                      solicitada;
         varNorte: apontador para a variavel da celula do Norte (vai receber o resultado);
         varSul: apontador para a variavel da celula do Sul (vai receber o resultado);        
         varLeste: apontador para a variavel da celula do Leste (vai receber o resultado);
         varOeste: apontador para a variavel da celula do Oeste (vai receber o resultado);

Criado por Andre STALTZ dia 31/03/07
Atualizado por Henrique Fav dia 24/04/07
______________________________________________________________________________*/                       
int obtemVariaveisVizinhas( Cell *celula,
                            short id_variavel,
                            double *varNorte,
                            double *varSul,
                            double *varLeste,
                            double *varOeste )
{
    // Celulas vizinhas pretas ou de fronteira (supondo uma malha uniforme)
    Cell *vizLeste,
         *vizOeste,
         *vizNorte,
         *vizSul;

    // Verifica erro de celula inexistente
    if( celula == NULL )
    {
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
        return ERRO_CELULA_INEXISTENTE;   
    }

    // Verifica erro de ID inexistente
    if( id_variavel != ID_VAR_RHO
    &&  id_variavel != ID_VAR_U
    &&  id_variavel != ID_VAR_V
    &&  id_variavel != ID_VAR_P
    &&  id_variavel != ID_VAR_T
    &&  id_variavel != ID_VAR_K
    &&  id_variavel != ID_VAR_Et
    &&  id_variavel != ID_VAR_MI )
    {
        IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
        cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
        return ERRO_ID_VAR_INEXISTENTE;
    }

    // Determinar celulas vizinhas pretas ------------------------------
    vizLeste = celula->east;        
    // Buscar no' preto ou no' de fronteira
    while( ( vizLeste->type == 'w' ) &&
           ( vizLeste->singleConnector != 0 )
         )
    {
        vizLeste = vizLeste->singleConnector;
    }

    vizOeste = celula->west;
    // Buscar no' preto ou no' de fronteira
    while( ( vizOeste->type == 'w' ) &&
           ( vizOeste->singleConnector != 0 )
         )
    {
        vizOeste = vizOeste->singleConnector;
    }

    vizNorte = celula->north;
    // Buscar no' preto ou no' de fronteira
    while( ( vizNorte->type == 'w' ) &&
           ( vizNorte->singleConnector != 0 )
         )
    {
        vizNorte = vizNorte->singleConnector;
    }

    vizSul = celula->south;       
    // Buscar no' preto ou no' de fronteira
    while( ( vizSul->type == 'w' ) &&
           ( vizSul->singleConnector != 0 )
         )
    {
        vizSul = vizSul->singleConnector;
    }

    if( varLeste != NULL )
    {
        // Retorna a variavel no vizinho leste
        switch( id_variavel )
        {
            case ID_VAR_RHO:
                *varLeste = vizLeste->rhoAtual;
            break;

            case ID_VAR_U:
                *varLeste = vizLeste->uAtual;
            break;

            case ID_VAR_V:
                *varLeste = vizLeste->vAtual;
            break;

            case ID_VAR_T:
                *varLeste = vizLeste->TAtual;
            break;

            case ID_VAR_P:
                *varLeste = vizLeste->pAtual;
            break;

            default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
            break;
        }
    }

    if( varOeste != NULL )
    {
        // Retorna a variavel no vizinho oeste
        switch( id_variavel )
        {
            case ID_VAR_RHO:
                *varOeste = vizOeste->rhoAtual;
            break;

            case ID_VAR_U:
                *varOeste = vizOeste->uAtual;
            break;

            case ID_VAR_V:
                *varOeste = vizOeste->vAtual;
            break;

            case ID_VAR_T:
                *varOeste = vizOeste->TAtual;
            break;

            case ID_VAR_P:
                *varOeste = vizOeste->pAtual;
            break;

            default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
            break;
        }
    }

    if( varNorte != NULL )
    {
        // Retorna a variavel no vizinho norte
        switch( id_variavel )
        {
            case ID_VAR_RHO:
                *varNorte = vizNorte->rhoAtual;
            break;

            case ID_VAR_U:
                *varNorte = vizNorte->uAtual;
            break;

            case ID_VAR_V:
                *varNorte = vizNorte->vAtual;
            break;

            case ID_VAR_T:
                *varNorte = vizNorte->TAtual;
            break;

            case ID_VAR_P:
                *varNorte = vizNorte->pAtual;
            break;

            default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
            break;
        }
    }

    if( varSul != NULL )
    {
        // Retorna a variavel no vizinho sul
        switch( id_variavel )
        {
            case ID_VAR_RHO:
                *varSul = vizSul->rhoAtual;
            break;

            case ID_VAR_U:
                *varSul = vizSul->uAtual;
            break;

            case ID_VAR_V:
                *varSul = vizSul->vAtual;
            break;

            case ID_VAR_T:
                *varSul = vizSul->TAtual;
            break;

            case ID_VAR_P:
                *varSul = vizSul->pAtual;
            break;

            default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
            break;
        }
    }

    return SUCESSO;
}      

/*______________________________________________________________________________

                  atualizaParametrosDaIteracaoAnterior()

   Atualiza parametros no inicio de cada iteracao
para a verificacao do criterio
   de convergencia ao final da mesma.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 25/04/07.
Atualizado por Henrique dia 12/06/07.
______________________________________________________________________________*/
int atualizaParametrosDaIteracaoAnterior( Grid *malha, char ciclo )
{
    Cell *celula;

    if( ciclo != 'A'
    &&  ciclo != 'B'
    &&  ciclo != 'C' )
    {
        cout << "Erro ciclo inexistente\n";
        return INSUCESSO;
    }

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            if( celula->active )
            {
                // atualiza parametros
                if( ciclo == 'A' )
                {
                    celula->TIterAnterior = celula->TAtual;
                }
                else if( ciclo == 'B' )
                {
                    celula->uIterAnterior = celula->uAtual;
                    celula->vIterAnterior = celula->vAtual;
                }
                else if( ciclo == 'C' )
                {
                    celula->pIterAnterior = celula->pAtual;
                }
            }
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em atualizaParametrosDaIteracaoAnterior()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em atualizaParametrosDaIteracaoAnterior()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  atualizaParametrosDoTempoAnterior()

   Atualiza parametros para prosseguir em um proximo passo de tempo.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Andre STALTZ dia 08/05/07
______________________________________________________________________________*/
int atualizaParametrosDoTempoAnterior( Grid *malha )
{
    Cell *celula;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            if( celula->active )
            {
                // atualiza parametros
                celula->uAnterior = celula->uAtual;
                celula->vAnterior = celula->vAtual;
                celula->pAnterior = celula->pAtual;
                celula->rhoAnterior = celula->rhoAtual;
                celula->EtAnterior = celula->EtAtual;
                celula->TAnterior = celula->TAtual;
            }
        }

        return SUCESSO;
    }
    else
    {
        cout << "Na funcao atualizaParametrosDoTempoAnterior():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao atualizaParametrosDoTempoAnterior():\n";
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  convergenciaPressao()

   Verifica a convergencia das pressoes.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 25/04/07
______________________________________________________________________________*/
int convergenciaPressao( Grid *malha )
{
    double erroPressaoCelula = 0.0,
           erroPressaoGlobal = 0.0;

    Cell *celula;

    // Se a malha possui celulas (foi inicializada)
    if( malha->firstGridCell != NULL )
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            if( celula->volumeFicticio == false && celula->active == true )
            {
                // Calcula erro relativo
                    erroPressaoCelula = fabs( 
                                        (celula->pAtual - celula->pIterAnterior) 
                                            / celula->pIterAnterior );
            
                if ( erroPressaoCelula > erroPressaoGlobal )
                    erroPressaoGlobal = erroPressaoCelula;
            }
        }

        // Verifica convergencia e retorna resultado
        if ( erroPressaoGlobal < COTA_PRESSAO )
            return SUCESSO;
        else{
            cout << "erroPressaoGlobal = " << erroPressaoGlobal << endl;
            return INSUCESSO;
        }
    }
    else
    {
        cout << "Na funcao convergenciaPressao():\n"; 
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao convergenciaPressao():\n"; 
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  convergenciaVelocidades()

   Verifica a convergencia das velovidades.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 25/04/07
______________________________________________________________________________*/
int convergenciaVelocidades( Grid *malha )
{
    double erroUcelula = 0.0,
           erroVcelula = 0.0,
           erroVelocidadesCelula = 0.0,
           erroVelocidadesGlobal = 0.0;

    Cell *celula;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            if( celula->volumeFicticio == false && celula->active == true )
            {
                // calcula erros
                erroUcelula = fabs( (celula->uAtual - celula->uIterAnterior) 
                            / celula->uIterAnterior );
                erroVcelula = fabs(  (celula->vAtual - celula->vIterAnterior) 
                            / celula->vIterAnterior );
            
                // Verificar de qual velocidade o erro e' maior
                if( erroUcelula > erroVcelula )
                {
                    erroVelocidadesCelula = erroUcelula;
                }
                else
                {
                    erroVelocidadesCelula = erroVcelula;
                }
    
                // Encontra erro maior entre as celulas
                if ( erroVelocidadesCelula > erroVelocidadesGlobal )
                {
                    erroVelocidadesGlobal = erroVelocidadesCelula;
                }
            }
        }
 
        // Verifica convergencia e retorna resultado
        if ( erroVelocidadesGlobal < COTA_VELOCIDADES )
            return SUCESSO;
        else
        {
            cout << "erroVelocidadesGlobal = " << erroVelocidadesGlobal << endl;
            return INSUCESSO;
        }
    }
    else
    {
        cout << "Na funcao convergenciaVelocidades():" << endl;
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return ERRO_MALHA_VAZIA;
    }

    cout << "Na funcao convergenciaVelocidades():" << endl;
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  convergenciaTemperatura()

   Verifica a convergencia das temperaturas.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 14/05/07
______________________________________________________________________________*/
int convergenciaTemperatura( Grid *malha )
{
    double erroTemperaturaCelula = 0.0,
           erroTemperaturaGlobal = 0.0;

    Cell *celula;

    // Se a malha possui celulas (foi inicializada)
    if( malha->firstGridCell != NULL )
    {
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            if( celula->volumeFicticio == false && celula->active == true )
            {
                erroTemperaturaCelula = fabs( (celula->TAtual - celula->TIterAnterior)
                                            / celula->TIterAnterior );

                if ( erroTemperaturaCelula > erroTemperaturaGlobal )
                    erroTemperaturaGlobal = erroTemperaturaCelula;
            }
        }

        // Verifica convergencia e retorna resultado
        if ( erroTemperaturaGlobal < COTA_TEMPERATURA )
            return SUCESSO;
        else
        {
            cout << "erroTemperaturaGlobal = " << erroTemperaturaGlobal << endl;
            return INSUCESSO;
        }
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em convergenciaTemperatura()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em convergenciaTemperatura()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                    gradienteBiconjugadoEstabilizado()

    Biconjugate gradient stabilized method for nonsymmetric matrices.
    Notation close to the one used in the original van der Vorst paper.
______________________________________________________________________________*/
int gradienteBiconjugadoEstabilizado( Grid *malha, short id_variavel )
{
    double error = 0.0,      //Euclidean norm of second res�due r after each iteration.
           tolerance = 1e-9, //Error tolerance
           rho = 1.0,
           rhoNew,
           alpha = 1.0,
           beta,
           omega = 1.0,
           r_ScalarAp,
           rScalarR = 0.0,
           sScalarAs,
           AsScalarAs;

    int numberOfIterations = 0;

    Cell *gridCell;
    Element *element;

    // Verificar se o sistema da variavel solicitada existe
    if( id_variavel != ID_VAR_U
    &&  id_variavel != ID_VAR_V
    &&  id_variavel != ID_VAR_PLINHA
    &&  id_variavel != ID_VAR_Et )
    {
        IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
        cout << "Erro acima ocorreu em gradienteBiconjugadoEstabilizado()" << endl;
        return ERRO_ID_VAR_INEXISTENTE;
    }

    //__________________________________________________________________________
    //Computes vector A*x in order to compute initial residue r = b - Ax, 
    //and then rScalarR = r^T * r. Uses Ap to store Ax.
    //Sets second residual vector r_ as r (r_ is an arbitrary vector, not 
    //orthogonal to r).
    gridCell = malha->firstGridCell;
    while( gridCell != NULL )
    {
        gridCell->Ap = 0.0;

        switch( id_variavel ) // Escolher qual o sistema sera' resolvido
        {
            case ID_VAR_U: // Velocidade U (horizontal) ======
                element = gridCell->firstElementMomentoX;
            break;

            case ID_VAR_V: // Velocidade V (vertical) ========
                element = gridCell->firstElementMomentoY;
            break;

            case ID_VAR_PLINHA: // Pressao (correcao) ========================
                element = gridCell->firstElementPressaoLinha;
            break;

            case ID_VAR_Et: // Energia total ========================
                element = gridCell->firstElementEnergia;
            break;
        }

        while( element != NULL )
        {
            switch( id_variavel ) // Escolher qual o sistema sera' resolvido
            {
                case ID_VAR_U: // Velocidade U (horizontal) ======
                    gridCell->Ap += element->value * element->cell->uAtual;
                break;

                case ID_VAR_V: // Velocidade V (vertical) ========
                    gridCell->Ap += element->value * element->cell->vAtual;
                break;

                case ID_VAR_PLINHA: // Pressao ========================
                    gridCell->Ap += element->value * element->cell->pLinha;
                break;

                case ID_VAR_Et: // Energia total ========================
                    gridCell->Ap += element->value * element->cell->EtAtual;
                break;
            }

            element = element->next;
        }

        switch( id_variavel ) // Escolher qual o sistema sera' resolvido
        {
            case ID_VAR_U: // Velocidade U (horizontal) ======
                gridCell->r = gridCell->b_MomentoX - gridCell->Ap;
            break;

            case ID_VAR_V: // Velocidade V (vertical) ========
                gridCell->r = gridCell->b_MomentoY - gridCell->Ap;
            break;

            case ID_VAR_PLINHA: // Pressao ========================
                gridCell->r = gridCell->b_PressaoLinha - gridCell->Ap;
            break;

            case ID_VAR_Et: // Pressao ========================
                gridCell->r = gridCell->b_Energia - gridCell->Ap;
            break;
        }

        gridCell->r_ = gridCell->r; //r_ should never be orthogonal to r.
        rScalarR += gridCell->r * gridCell->r;
        gridCell->Ap = 0.0;//Resets Ap to 0.

        gridCell = gridCell->next;
    }
    error = sqrt(rScalarR);
    //__________________________________________________________________________
    //Biconjugate gradient stabilized iterations.
    while( error > tolerance
        && numberOfIterations < LIMITE_ITERACOES_GRADBICONJUGADO)// && rho > tolerance && omega > tolerance )
    {
        //______________________________________________________________________
        //Computes rhoNew = < r_, r >.
        gridCell = malha->firstGridCell;
        rhoNew = 0.0;
        while( gridCell != 0 )
        {
            rhoNew += gridCell->r_ * gridCell->r;
            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes beta and updates rho.
        beta = (rhoNew/rho)*(alpha/omega);
        rho = rhoNew;
        //______________________________________________________________________
        //Updates p.
        gridCell = malha->firstGridCell; 
        while( gridCell != 0 )
        {
            gridCell->p 
            = gridCell->r + beta*( gridCell->p - omega*gridCell->Ap );

            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes Ap and < r_, Ap >.
        gridCell = malha->firstGridCell;
        r_ScalarAp = 0.0;
        while( gridCell != 0 )
        {
            gridCell->Ap = 0.0;
            switch( id_variavel ) // Escolher qual o sistema sera' resolvido
            {
                case ID_VAR_U: // Velocidade U (horizontal) ======
                    element = gridCell->firstElementMomentoX;
                break;

                case ID_VAR_V: // Velocidade V (vertical) ========
                    element = gridCell->firstElementMomentoY;
                break;

                case ID_VAR_PLINHA: // Pressao ========================
                    element = gridCell->firstElementPressaoLinha;
                break;

                case ID_VAR_Et: // Pressao ========================
                    element = gridCell->firstElementEnergia;
                break;
            }
            while( element != 0 )
            {
                gridCell->Ap += element->value * element->cell->p;
                element = element->next;
            }
            r_ScalarAp += gridCell->r_ * gridCell->Ap;

            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes alpha.
        alpha = rhoNew/r_ScalarAp;
        //______________________________________________________________________
        //Updates second residue s = r - alpha*Ap and sScalarS.
        gridCell = malha->firstGridCell;
        while( gridCell != 0 )
        {
            gridCell->s = gridCell->r - alpha * gridCell->Ap;
            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes As, sScalarAs and AsScalarAs.
        gridCell = malha->firstGridCell;
        sScalarAs = 0.0;
        AsScalarAs = 0.0;
        while( gridCell != 0 )
        {
            gridCell->As = 0.0;
            switch( id_variavel ) // Escolher qual o sistema sera' resolvido
            {
                case ID_VAR_U: // Velocidade U (horizontal) ======
                    element = gridCell->firstElementMomentoX;
                break;

                case ID_VAR_V: // Velocidade V (vertical) ========
                    element = gridCell->firstElementMomentoY;
                break;

                case ID_VAR_PLINHA: // Pressao ========================
                    element = gridCell->firstElementPressaoLinha;
                break;

                case ID_VAR_Et: // Energia ========================
                    element = gridCell->firstElementEnergia;
                break;
            }
            while( element != 0 )
            {
                gridCell->As += element->value * element->cell->s;
                element = element->next;
            }
            sScalarAs += gridCell->s * gridCell->As;
            AsScalarAs += gridCell->As * gridCell->As;

            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes omega.
        omega = sScalarAs/AsScalarAs;
        //______________________________________________________________________
        //Updates solution x = x + alpha*p + omega*s and computes
        //residue r = s - omega*As and <r,r>.
        gridCell = malha->firstGridCell;
        rScalarR = 0.0;
        while( gridCell != 0 )
        {
            switch( id_variavel ) // Escolher qual o sistema sera' resolvido
            {
                case ID_VAR_U: // Velocidade U (horizontal) ======
                    gridCell->uAtual += alpha*gridCell->p + omega*gridCell->s;
                break;

                case ID_VAR_V: // Velocidade V (vertical) ========
                    gridCell->vAtual += alpha*gridCell->p + omega*gridCell->s;
                break;

                case ID_VAR_PLINHA: // Pressao ========================
                    gridCell->pLinha += alpha*gridCell->p + omega*gridCell->s;
                break;

                case ID_VAR_Et: // Energia total ========================
                    gridCell->EtAtual += alpha*gridCell->p + omega*gridCell->s;
                break;
            }

            gridCell->r = gridCell->s - omega * gridCell->As;
            rScalarR += gridCell->r * gridCell->r;

            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Updates error.
        error = sqrt(rScalarR);

        numberOfIterations++;
    }

    cout << "Custo do gradiente biconjugado estabilizado: " << numberOfIterations << " iteracoes.";

    if( numberOfIterations == LIMITE_ITERACOES_GRADBICONJUGADO )
        cout << " (maximo estabelecido)";

    cout << endl;

    return SUCESSO;
}//end gradienteBiconjugadoEstabilizado() function.

/*______________________________________________________________________________

                              gradienteConjugado()
                        
    Resolve o sistema A x = b (de algum sistema dado pelo parametro) por gradiente 
conjugado.

    Parametros:
         malha: ponteiro para o Grid contendo as celulas e as matrizes a serem resolvidas
         id_variavel: um identificador para saber qual variavel sera' resolvida.

Criado por Andre STALTZ dia 08/05/07
______________________________________________________________________________*/
int gradienteConjugado( Grid *malha, short id_variavel ) 
{
    double error = 0.0,	     //Norma do residuo (b - Ax) apos cada iteracao
           pError = 1.0,
           rTr,
           r1Tr1,
           pTAp,
           alpha,
           beta,
           normR = 0.0, 
           normPP1 = 0.0,
           precision = COTA_GRADCONJUGADO; // Precisao minima cujo o erro nao pode ultrapassar
           
    int maxIteracoes = malha->numberOfCells,
        iteracoes = 0;
        
    Cell *gridCell;
    Element *element;

    // Verificar se o sistema da variavel solicitada existe
    if( id_variavel != ID_VAR_U
    &&  id_variavel != ID_VAR_V
    &&  id_variavel != ID_VAR_PLINHA )
    {
        IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
        cout << "Erro acima ocorreu em gradienteConjugado()" << endl;
        return ERRO_ID_VAR_INEXISTENTE;
    }
        
    //__________________________________________________________________________    
    //Computes vector A*x, residue r = b - Ax, scalar rTr = r^T * r and 
    //sets initial search direction p.    
    gridCell = malha->firstGridCell; 
    rTr = 0.0;   
    while( gridCell != 0 )
    {
        if( gridCell->active )
        {
            gridCell->Ax = 0.0;// Para cada celula da malha (correspondendo a uma linha da matriz)
            switch( id_variavel ) // Escolher qual o sistema sera' resolvido
            {
                case ID_VAR_U: // Velocidade U (horizontal) ======
                    element = gridCell->firstElementMomentoX;
                break;
        
                case ID_VAR_V: // Velocidade V (vertical) ========
                    element = gridCell->firstElementMomentoY;
                break;

                case ID_VAR_PLINHA: // Pressao ========================
                    element = gridCell->firstElementPressaoLinha;
                break;
            }
            
            while( element != NULL )
            {
                switch( id_variavel ) // Escolher qual o sistema sera' resolvido
                {
                    case ID_VAR_U: // Velocidade U (horizontal) ======
                        gridCell->Ax += element->value * element->cell->uAtual;
                    break;
        
                    case ID_VAR_V: // Velocidade V (vertical) ========
                        gridCell->Ax += element->value * element->cell->vAtual;
                    break;

                    case ID_VAR_PLINHA: // Pressao ========================
                        gridCell->Ax += element->value * element->cell->pLinha;
                    break;
                }

                element = element->next;           
            }

            switch( id_variavel ) // Escolher qual o sistema sera' resolvido
            {
                case ID_VAR_U: // Velocidade U (horizontal) ======
                    gridCell->r = gridCell->b_MomentoX - gridCell->Ax;
                break;
        
                case ID_VAR_V: // Velocidade V (vertical) ========
                    gridCell->r = gridCell->b_MomentoY - gridCell->Ax;
                break;

                case ID_VAR_PLINHA: // Pressao ========================
                    gridCell->r = gridCell->b_PressaoLinha - gridCell->Ax;
                break;
            }

            rTr += gridCell->r * gridCell->r;
            gridCell->p = gridCell->r;
            if( fabs(gridCell->r) > error )
                error = fabs(gridCell->r);
        }    
        gridCell = gridCell->next;
    }    
    //__________________________________________________________________________
    //Conjugate gradient iterations.
    if( error >= precision )
    {
        while( (pError >= precision)  && 
               (iteracoes < maxIteracoes) )
        {
            pError = 0.0;
            normPP1 = 0.0;
            //__________________________________________________________________
            // Computes Ap and pTAp. Uses Ax to store Ap.
            gridCell = malha->firstGridCell;
            pTAp = 0.0;    
            while( gridCell != NULL )
            {
                if( gridCell->active )
                {
                    gridCell->Ax = 0.0;


                    switch( id_variavel ) // Escolher qual o sistema sera' resolvido
                    {
                        case ID_VAR_U: // Velocidade U (horizontal) ======
                            element = gridCell->firstElementMomentoX;
                        break;
        
                        case ID_VAR_V: // Velocidade V (vertical) ========
                            element = gridCell->firstElementMomentoY;
                        break;

                        case ID_VAR_PLINHA: // Pressao ========================
                            element = gridCell->firstElementPressaoLinha;
                        break;
                    }

                    while( element != NULL )
                    {
                        gridCell->Ax += element->value * element->cell->p;
                        element = element->next;           
                    }
                    pTAp += gridCell->p * gridCell->Ax;                        
                }    
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            // Computes alpha.
            alpha = rTr/pTAp;
            //__________________________________________________________________
            // Computes new value of solution: u = u + alpha*p.
            gridCell = malha->firstGridCell;    
            while( gridCell != NULL )
            {
                if( gridCell->active )                    
                {
                    switch( id_variavel ) // Escolher qual o sistema sera' resolvido
                    {
                        case ID_VAR_U: // Velocidade U (horizontal) ======
                            gridCell->uAtual += alpha * gridCell->p;
                        break;
        
                        case ID_VAR_V: // Velocidade V (vertical) ========
                            gridCell->vAtual += alpha * gridCell->p;
                        break;

                        case ID_VAR_PLINHA: // Pressao ========================
                            gridCell->pLinha += alpha * gridCell->p;
                        break;
                    }
                }
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            //Upgrades residue r1 = r - alpha*Ap and computes r1Tr1 = r1^T * r1. 
            gridCell = malha->firstGridCell;
            r1Tr1 = 0.0;    
            while( gridCell != NULL )
            {
                if( gridCell->active )
                {                
                    gridCell->r -= alpha * gridCell->Ax;
                    r1Tr1 += gridCell->r * gridCell->r;
                }                       
                gridCell = gridCell->next;
            }
            //__________________________________________________________________            
            //Computes beta.
            beta = r1Tr1/rTr;
            //__________________________________________________________________            
            //Computes vector p1 = r1 + beta*p and uses it to upgrade p.
            gridCell = malha->firstGridCell;    
            while( gridCell != NULL )
            {
                if( gridCell->active )
                {                
                    gridCell->p1 = gridCell->r + beta * gridCell->p;   
                    if ( fabs(gridCell->r) > normR )
                         normR = fabs(gridCell->r);
                    if ( fabs(gridCell->p - gridCell->p1) > normPP1 )
                         normPP1 = fabs(gridCell->p - gridCell->p1);
                    gridCell->p = gridCell->p1;
                }                    
                gridCell = gridCell->next;
            }
            
            if( normR < precision )
                pError = 0.0;
            else if( normPP1 > pError )
                pError = normPP1;
            //__________________________________________________________________
            //Upgrades rTr.
            rTr = r1Tr1;

            iteracoes++;
        }  
     
    cout << "Custo do gradiente conjugado: " << 100.0*(   
                                                            ( (double)iteracoes )
                                                          / ( (double)malha->numberOfCells )
                                                      )  
         << " %" << endl;  
    }

    return SUCESSO;
}

/*______________________________________________________________________________

                        calculaResiduos()

    Calcula os residuos das variaveis importantes para desenhar seus graficos.
______________________________________________________________________________*/
void calculaResiduos( Grid *malha, int iteracaoAtual )
{
    Cell *celula; // Celula da malha
    double somaResiduosP = 0.0,
           somaResiduosVel = 0.0,
           somaResiduosT = 0.0;
    int quantCelulas = 0;

    if(malha->firstGridCell != NULL)
    {
        // Soma todos os residuos de todas as celulas ativas
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            if(celula->volumeFicticio == false && celula->active == true){
                somaResiduosP += fabs(celula->pAtual - celula->pAnterior);
                somaResiduosVel += fabs( 
                                        sqrt( AO_QUADRADO(celula->uAtual) +    
                                              AO_QUADRADO(celula->vAtual) 
                                        ) 
                                      - sqrt( AO_QUADRADO(celula->uAnterior) +    
                                              AO_QUADRADO(celula->vAnterior) 
                                        )
                                   );
                somaResiduosT += fabs(celula->TAtual - celula->TAnterior);

                quantCelulas++;
            }
        }

        // Calcula media dos residuos
        malha->residuosP[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS] = 
                        somaResiduosP / ( (double) quantCelulas );

        malha->residuosVel[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS] = 
                        somaResiduosVel / ( (double) quantCelulas );

        malha->residuosT[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS] = 
                        somaResiduosT / ( (double) quantCelulas );

        // Atualiza maior residuo
        if(malha->residuosP[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS] > malha->maiorResiduo){
            malha->maiorResiduo = malha->residuosP[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS];
        }
        if(malha->residuosVel[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS] > malha->maiorResiduo){
            malha->maiorResiduo = malha->residuosVel[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS];
        }
        if(malha->residuosT[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS] > malha->maiorResiduo){
            malha->maiorResiduo = malha->residuosT[iteracaoAtual % MAXITER_GRAFICO_RESIDUOS];
        }
    }
    else
    {
        cout << "Na funcao calculaResiduos():\n";
        IMPRIMIR_ERRO_MALHA_VAZIA();
        return;
    }
}







// Imprime a matriz de alguma variavel
void imprimeMatriz( Grid *malha, short id_variavel )
{
    Cell *celula;

    Element *element;

    // Verifica se a id de variavel solicitada tem matriz correspondente
    if( id_variavel != ID_VAR_U
    &&  id_variavel != ID_VAR_V
    &&  id_variavel != ID_VAR_PLINHA
    &&  id_variavel != ID_VAR_Et )
    {
        IMPRIMIR_ERRO_ID_VARIAVEL_INEXISTENTE();
        cout << "Erro acima ocorreu em imprimeMatriz()" << endl;
    }

    if(malha->firstGridCell != NULL)
    {
        switch( id_variavel ) // Escolher qual o sistema sera' resolvido
        {
            case ID_VAR_U: // Velocidade U (horizontal) ======
                cout << "\n\nImprimindo a matriz da velocidade horizontal (u)\n";
            break;

            case ID_VAR_V: // Velocidade V (vertical) ========
                cout << "\n\nImprimindo a matriz da velocidade vertical (v))\n";
            break;

            case ID_VAR_PLINHA: // Pressao ========================
                cout << "\n\nImprimindo a matriz da pressao\n";
            break;

            case ID_VAR_Et: // Energia total ========================
                cout << "\n\nImprimindo a matriz da energia\n";
            break;
        }

        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            if( celula->active )
            {
                if( celula->active )
                {
                    cout << "\n";

                    switch( id_variavel ) // Escolher qual o sistema sera' resolvido
                    {
                        case ID_VAR_U: // Velocidade U (horizontal) ======
                            element = celula->firstElementMomentoX;
                        break;

                        case ID_VAR_V: // Velocidade V (vertical) ========
                            element = celula->firstElementMomentoY;
                        break;

                        case ID_VAR_PLINHA: // Pressao ========================
                            element = celula->firstElementPressaoLinha;
                        break;

                        case ID_VAR_Et: // Energia total ========================
                            element = celula->firstElementEnergia;
                        break;
                    }

                    for( ; element != NULL; element = element->next )
                    {
                        cout << "(" << celula->gridPosition
                            << ", " << element->column << ")"
                            << "=" << element->value << "   ";
                    }
                }
            }
        }
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em ImprimeMatriz()" << endl;
    }
}
