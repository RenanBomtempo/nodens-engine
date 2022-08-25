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
         iteracoesB = 0;
    int bla;
   
    //do // Ciclo A -------------------------------------------------------------
    //{ Nao usa para Couette
        do // Ciclo B -------------------------------------------------------------
        {
            atualizaParametrosDaIteracaoAnterior( malha, 'B' );

            interpolaTodasInterfaces( malha, true );
            atualiza_Gamma( malha );
            atualiza_Beta( malha );

            // Item 2. (calcular os coeficientes do momentum de u* e v*)
            calculaCoeficientesMomentoX( malha );
            calculaCoeficientesMomentoY( malha );

            // Item 4. (campo de velocidades u* e v* e' determinado)
            cout << "Resolvendo matriz do momento-x...\t";
            gradienteBiconjugadoEstabilizado( malha, ID_VAR_U );
            cout << "Resolvendo matriz do momento-y...\t";
            gradienteBiconjugadoEstabilizado( malha, ID_VAR_V );

            calculaVelocidadesNasFaces( malha );

            iteracoesC = 0;
            do // Ciclo C ---------------------------------------------------------
            {
                atualizaParametrosDaIteracaoAnterior( malha, 'C' );

                // Item 3.
                calculaCoeficientesCorrecaoPressao( malha );

                // Item 5. Termo fonte (b^P')
                calculaTermoFonteCorrecaoPressao( malha );

                // Item 6a. Resolver a equacao para P'
                cout << "Resolvendo matriz de pressao'...\t";
                gradienteBiconjugadoEstabilizado( malha, ID_VAR_P );

                // Item 6b e 6c. Corrigir u*, v*, rho* e P*
                corrige_Velocidades_Densidades_Pressoes( malha );

                // Verifica convergencia
                if( convergenciaPressao( malha, 'C' ) == SUCESSO )
                    convergiu = true;
                else
                    convergiu = false;

                iteracoesC++;
                cout << "\n";

            // Item 7. Retorna ao item 3 ate' convergir ---------------------------
            } while( !convergiu ); // Ciclo C

            cout << "\nIteracoes do ciclo C: " << iteracoesC << endl;
            //cout << "\n\n#####################################\n\n";
            //scanf("%i",&bla);


            // Verifica convergencia
            if( convergenciaVelocidades( malha, 'B' ) == SUCESSO )
                convergiu = true;
            else
                convergiu = false;

            iteracoesB++;
        // Item 8. Retorna ao item 2 ate' convergir -------------------------------
        } while( !convergiu ); // Ciclo B

        cout << "\nIteracoes do ciclo B: " << iteracoesB << endl;

        // Item 9.  Nao usa para Couette
        // Calcular os coeficientes da equacao de energia e campo de T e' determinado

        // Item 10.  Nao usa para Couette
        // Por equacao de estado, um novo campo de densidade e' calculado
        // HENRIQUE

    // Item 11. Retorna ao item 2 ate' convergir (Nao usa para Couette)
    //} while( false /* AQUI ENTRA O CRITERIO DE CONVERGENCIA */ ); // Ciclo A

    // A solucao do problema em t + deltaT foi obtida

    // O tempo e' incrementado e tudo se repete
    malha->tempoAtual += malha->deltaT;
    atualizaParametrosDoTempoAnterior( malha );

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
    
    if(malha->firstGridCell != NULL)
    {
        // Ajustar todas as celulas com a configuracao inicial
        for(celula = malha->firstGridCell; 
            celula != 0; 
            celula = celula->next)
		{
		// Variaveis de fluido
			celula->pAtual = PRESSAO_LIVRE;
			celula->pAnterior = celula->pAtual;

			celula->uAtual = 0.0001*VEL_PAREDE; // repouso
			celula->uAnterior =  celula->uAtual; // repouso

			celula->vAtual = 0.0; 
			celula->vAnterior = celula->vAtual; 

			celula->rhoAtual = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
			celula->rhoAnterior = celula->rhoAtual;

			celula->TAtual = TEMPERATURA_LIVRE;  // nao entra no Couette, mas e' necessario para condicao 
			celula->TAnterior = celula->TAtual; // de fronteira da densidade (extrapolacao equacao estado)

			/*celula->EtAtual = celula->rhoAtual
                           * (
                                R*TEMPERATURA_LIVRE/(GAMMA-1.0) // e (minusculo!)
                                + (
                                      celula->uAtual * celula->uAtual
                                    + celula->vAtual * celula->vAtual
                                  )/2.0
                             ); 
			celula->EtAnterior = celula->EtAtual; */  // nao entra no Couette
                          
			celula->miAtual = MI_ZERO;
			celula->miAnterior = celula->miAtual;

			// celula->kAtual = (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL); // nao entra no couette
			// celula->kAnterior = celula->kAtual; // nao entra no couette
        }

        return SUCESSO;
    }
    else
    {
        return ERRO_MALHA_VAZIA;
        cout << "Erro acima ocorreu em condicaoInicial()" << endl;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em condicaoInicial()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________
                            
                   calculaCoeficientesMomentoX()
                            
    Calcula os coeficientes A_p, A_e, A_w, A_n, A_s para cada celula, 
montando a matriz [A] do sistema associado 'a correcao de momento.

    Parametros:
        malha: apontador para a malha em questao;

Criado por Andre STALTZ e Henrique Fav dia 23/04/07
Atualizado em 24/04/07 por Andre STALTZ
______________________________________________________________________________*/
int calculaCoeficientesMomentoX( Grid *malha )
{
    // Obs: Todos os deltaY/deltaX da discretizacao foram substituidos por 1,
    // visto que a malha do ALG e' sempre quadrada

    // Obs: Variaveis nas faces (mi, rho, u) PRECISAM ser os "*" (estimados)

    Cell *celula;

    // Coeficientes
    double A_P = 0.0,
           A_e = 0.0,
           A_w = 0.0,
           A_n = 0.0,
           A_s = 0.0;

    // Termo fonte
    double S_c = 0.0,
           S_P = 0.0,
           S_e = 0.0,
           S_w = 0.0;

    // Pressoes nas celulas vizinhas
    double P_E = 0.0, 
           P_W = 0.0;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            // Obtem os valores de pressao nas celulas vizinhas
            obtemVariaveisVizinhas( celula, ID_VAR_P, NULL, NULL, &P_E, &P_W );

            // Termo fonte
            S_e = (malha->deltaT) * (celula->eastFaceMi) / 3.0;

            S_w = (malha->deltaT) * (celula->westFaceMi) / 3.0;

            S_P = (malha->deltaT) * (-celula->eastFaceMi -celula->westFaceMi) / 3.0;

            S_c = malha->deltaT * celula->faceLength *
                  ( celula->northFaceMi * dvdx(celula, 'n') - celula->southFaceMi * dvdx(celula, 's') ) +
                  malha->deltaT * celula->faceLength * ( 2.0 / 3.0 ) *
                  ( -celula->eastFaceMi * dvdy(celula, 'e') + celula->westFaceMi * dvdy(celula, 'w') ) -
                  malha->deltaT * celula->faceLength * ( P_E - P_W );

            // Calculo dos coeficientes A das interfaces
            A_e = (0.5 - celula->eastFaceGamma) * (celula->eastFaceRho) 
                * (celula->eastFaceU) * celula->faceLength
                - (celula->eastFaceMi) * (celula->eastFaceBeta) - S_e;

            A_w = -1.0 * (0.5 + celula->westFaceGamma) * (celula->westFaceRho) 
                * (celula->westFaceU) * celula->faceLength
                - (celula->westFaceMi) * (celula->westFaceBeta) - S_w;

            A_n = (0.5 - celula->northFaceGamma) * (celula->northFaceRho) 
                * (celula->northFaceV) * celula->faceLength
                - (celula->northFaceMi) * (celula->northFaceBeta);

            A_s = -1.0 * (0.5 + celula->southFaceGamma) * (celula->southFaceRho) 
                * (celula->southFaceV) * celula->faceLength
                - (celula->southFaceMi) * (celula->southFaceBeta);

            // Calculo do coeficiente A principal
            A_P = - A_e - A_w - A_n - A_s - S_P 
                + (celula->rhoAnterior) 
                  * AO_QUADRADO(celula->faceLength) / (malha->deltaT);

            // Insere coeficiente principal na lista encadeada
            celula->iniciaElement( A_P, &(celula->firstElementMomentoX) );

            // Zerar (iniciar) o vetor B da equacao AX=B do MomentoX
            celula->b_MomentoX = 0.0;

            // Insere coeficientes dos vizinhos na mesma lista encadeada
            if( celula->east->type == 'b' && celula->east->active == true )
                celula->insereElement( A_e, celula->east, &(celula->firstElementMomentoX) );
            else // Lado direito da equacao AX=B pois leste e' fronteira
                celula->b_MomentoX -= ( A_e * condicaoFronteira( celula, ID_VAR_U, 'e', false ) );

            if( celula->west->type == 'b' )
                celula->insereElement( A_w, celula->west, &(celula->firstElementMomentoX) );
            else // Lado direito da equacao AX=B pois oeste e' fronteira
                celula->b_MomentoX -= ( A_w * condicaoFronteira( celula, ID_VAR_U, 'w', false ) );

            if( celula->north->type == 'b' )
                celula->insereElement( A_n, celula->north, &(celula->firstElementMomentoX) );
            else // Lado direito da equacao AX=B pois norte e' fronteira
                celula->b_MomentoX -= ( A_n * condicaoFronteira( celula, ID_VAR_U, 'n', false ) );

            if( celula->south->type == 'b' )
                celula->insereElement( A_s, celula->south, &(celula->firstElementMomentoX) );
            else // Lado direito da equacao AX=B pois sul e' fronteira
                celula->b_MomentoX -= ( A_s * condicaoFronteira( celula, ID_VAR_U, 's', false ) );

            // Insere o membro direito da equacao no vetor b da resolucao
            celula->b_MomentoX += S_c + ( celula->rhoAnterior * AO_QUADRADO( celula->faceLength ) 
                            * celula->uAnterior ) / (malha->deltaT);
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em calculaCoeficientesMomentoX()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em calculaCoeficientesMomentoX()" << endl;
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

    // Coeficientes
    double A_P = 0.0,
           A_e = 0.0,
           A_w = 0.0,
           A_n = 0.0,
           A_s = 0.0;

    // Termo fonte
    double S_c = 0.0,
           S_P = 0.0,
           S_n = 0.0,
           S_s = 0.0;

    // Pressoes nas celulas vizinhas
    double P_N, P_S;

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            // Obtem os valores de pressao nas celulas vizinhas
            obtemVariaveisVizinhas( celula, ID_VAR_P, &P_N, &P_S, NULL, NULL );

            // Termo fonte
            S_n = (malha->deltaT) * (celula->northFaceMi) / 3.0;

            S_s = (malha->deltaT) * (celula->southFaceMi) / 3.0;

            S_P = (malha->deltaT) * (-celula->northFaceMi -celula->southFaceMi) / 3.0;

            S_c = malha->deltaT * celula->faceLength *
                  ( celula->eastFaceMi * dudy(celula, 'e') - celula->westFaceMi * dudy(celula, 'w') ) +
                  malha->deltaT * celula->faceLength * ( 2.0 / 3.0 ) *
                  ( -celula->northFaceMi * dudx(celula, 'n') + celula->southFaceMi * dudx(celula, 's')) -
                  malha->deltaT * celula->faceLength * ( P_N - P_S );

            // Calculo dos coeficientes A das interfaces
            A_e = (0.5 - celula->eastFaceGamma) * (celula->eastFaceRho)
                * (celula->eastFaceU) * celula->faceLength
                - (celula->eastFaceMi) * (celula->eastFaceBeta);

            A_w = -1.0 * (0.5 + celula->westFaceGamma) * (celula->westFaceRho)
                * (celula->westFaceU) * celula->faceLength
                - (celula->westFaceMi) * (celula->westFaceBeta);

            A_n = (0.5 - celula->northFaceGamma) * (celula->northFaceRho)
                * (celula->northFaceV) * celula->faceLength
                - (celula->northFaceMi) * (celula->northFaceBeta) - S_n;

            A_s = -1.0 * (0.5 + celula->southFaceGamma) * (celula->southFaceRho)
                * (celula->southFaceV) * celula->faceLength
                - (celula->southFaceMi) * (celula->southFaceBeta) - S_s;

            // Calculo do coeficiente A principal
            A_P = - A_e - A_w - A_n - A_s - S_P
                + (celula->rhoAnterior)
                  * AO_QUADRADO(celula->faceLength) / (malha->deltaT);

            // Insere coeficiente principal na lista encadeada
            celula->iniciaElement( A_P, &(celula->firstElementMomentoY) );

            // Zerar (iniciar) o vetor B da equacao AX=B do MomentoY
            celula->b_MomentoY = 0.0;

            // Insere coeficientes dos vizinhos na mesma lista encadeada
            if( celula->east->type == 'b' )
                celula->insereElement( A_e, celula->east, &(celula->firstElementMomentoY) );
            else // Lado direito da equacao AX=B pois leste e' fronteira
                celula->b_MomentoY -= ( A_e * condicaoFronteira( celula, ID_VAR_V, 'e', false ) );
            
            if( celula->west->type == 'b' )
                celula->insereElement( A_w, celula->west, &(celula->firstElementMomentoY) );
            else // Lado direito da equacao AX=B pois oeste e' fronteira
                celula->b_MomentoY -= ( A_w * condicaoFronteira( celula, ID_VAR_V, 'w', false ) );

            if( celula->north->type == 'b' )
                celula->insereElement( A_n, celula->north, &(celula->firstElementMomentoY) );
            else // Lado direito da equacao AX=B pois norte e' fronteira
                celula->b_MomentoY -= ( A_n * condicaoFronteira( celula, ID_VAR_V, 'n', false ) );

            if( celula->south->type == 'b' )
                celula->insereElement( A_s, celula->south, &(celula->firstElementMomentoY) );
            else // Lado direito da equacao AX=B pois sul e' fronteira
                celula->b_MomentoY -= ( A_s * condicaoFronteira( celula, ID_VAR_V, 's', false ) );

            // Insere o membro direito da equacao no vetor b da resolucao
            celula->b_MomentoY += S_c + ( celula->rhoAnterior * AO_QUADRADO( celula->faceLength ) 
                        * celula->vAnterior ) / malha->deltaT;
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em calculaCoeficientesMomentoY()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em calculaCoeficientesMomentoY()" << endl;
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
            if( celula->active )
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
                    cout << " celula: " << celula->gridPosition;
                    return ERRO_ELEMENT_NULO;
                }

                // --- Calcula velocidade v na face norte ---
                // Se nao esta na fronteira
                if( celula->north->type == 'b' )
                {
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
                }

                // Senao, a velocidade na face norte vai ser dada pela condicao de fronteira
                else
                    celula->northFaceV = condicaoFronteira( celula, ID_VAR_V, 'n', true );

                // --- Calcula velocidade v na face sul ---
                // Se nao esta na fronteira
                if( celula->south->type == 'b' )
                {
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
                }

                // Senao, a velocidade na face sul vai ser dada pela condicao de fronteira
                else
                    celula->southFaceV = condicaoFronteira( celula, ID_VAR_V, 's', true );

                // --- Calcula velocidade na face leste ---
                // Se nao esta na fronteira
                if( celula->east->type == 'b' )
                {
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
                }

                // Senao, a velocidade na face leste vai ser dada pela condicao de fronteira
                else
                    celula->eastFaceU = condicaoFronteira( celula, ID_VAR_U, 'e', true );

                // --- Calcula velocidade na face norte ---
                // Se nao esta na fronteira
                if( celula->west->type == 'b' )
                {
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

                // Senao, a velocidade na face oeste vai ser dada pela condicao de fronteira
                else
                    celula->westFaceU = condicaoFronteira( celula, ID_VAR_U, 'w', true );
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
int calculaCoeficientesCorrecaoPressao( Grid *malha )
{
    Cell *celula;

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

    // Se a malha possui celulas (foi inicializada)
    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell; 
            celula != NULL; 
            celula = celula->next)
        {
            double rhoVizLeste = 0.0, rhoVizOeste = 0.0, 
                   rhoVizNorte = 0.0, rhoVizSul = 0.0;
            double TVizLeste = 0.0, TVizOeste = 0.0,
                   TVizNorte = 0.0, TVizSul = 0.0;

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

            // OBS: FOI EVITADO A DIVISAO POR ZERO, MAS ISTO E' QUESTIONAVEL
            celula->d_e = ( (celula->I_e == 0.0) ? 0.0 : (celula->faceLength / celula->I_e) );

            celula->d_w = ( (celula->I_w == 0.0) ? 0.0 : (celula->faceLength / celula->I_w) );

            celula->d_n = ( (celula->I_n == 0.0) ? 0.0 : (celula->faceLength / celula->I_n) );

            celula->d_s = ( (celula->I_s == 0.0) ? 0.0 : (celula->faceLength / celula->I_s) );

            // Calcula L_P ==================================================
            L_P =   m_P_rho * C_P_rho
                    + m_e_u * celula->d_e
                    - m_w_u * celula->d_w
                    + m_n_v * celula->d_n
                    - m_s_v * celula->d_s;

            /* Adicionar L_P na matriz de coeficientes de pressao */
            celula->iniciaElement( L_P, &(celula->firstElementPressao) );

            // Zerar (iniciar) o vetor B da equacao AX=B da pressao'
            celula->b_Pressao = 0.0;

            // Calcula L_(e,w,n,s) =========================================== 
            L_e = -1.0 * (-1.0 * m_e_rho * C_E_rho + m_e_u * celula->d_e);
            L_w = -1.0 * (-1.0 * m_w_rho * C_W_rho - m_w_u * celula->d_w);
            L_n = -1.0 * (-1.0 * m_n_rho * C_N_rho + m_n_v * celula->d_n);
            L_s = -1.0 * (-1.0 * m_s_rho * C_S_rho - m_s_v * celula->d_s);

            // Insere coeficientes dos vizinhos na mesma lista encadeada
            if( celula->east->type == 'b' && celula->east->active == true )
                celula->insereElement( L_e, celula->east, &(celula->firstElementPressao) );
            else // Lado direito da equacao AX=B pois leste e' fronteira
                celula->b_Pressao -= ( L_e * celula->pLinha );//condicaoFronteira( celula, ID_VAR_P, 'e', false ) );

            if( celula->west->type == 'b' )
                celula->insereElement( L_w, celula->west, &(celula->firstElementPressao) );
            else // Lado direito da equacao AX=B pois oeste e' fronteira
                celula->b_Pressao -= ( L_w * celula->pLinha );//condicaoFronteira( celula, ID_VAR_P, 'w', false ) );

            if( celula->north->type == 'b' )
                celula->insereElement( L_n, celula->north, &(celula->firstElementPressao) );
            else // Lado direito da equacao AX=B pois norte e' fronteira
                celula->b_Pressao -= ( L_n * celula->pLinha );//condicaoFronteira( celula, ID_VAR_P, 'n', false ) );

            if( celula->south->type == 'b' )
                celula->insereElement( L_s, celula->south, &(celula->firstElementPressao) );
            else // Lado direito da equacao AX=B pois sul e' fronteira
                celula->b_Pressao -= ( L_s * celula->pLinha );//condicaoFronteira( celula, ID_VAR_P, 's', false ) );
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em calculaCoeficientesCorrecaoPressao()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em calculaCoeficientesCorrecaoPressao()" << endl;
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
            double rhoVizLeste = 0.0, rhoVizOeste = 0.0,
                   rhoVizNorte = 0.0, rhoVizSul = 0.0;
            double TVizLeste = 0.0, TVizOeste = 0.0,
                   TVizNorte = 0.0, TVizSul = 0.0;

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
                       +( 0.5 + celula->northFaceGamma) * celula->northFaceV
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
            celula->b_Pressao += b_P;
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em calculaTermoFonteCorrecaoPressao()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em calculaTermoFonteCorrecaoPressao()" << endl;
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
int corrige_Velocidades_Densidades_Pressoes( Grid *malha )
{
    // Celula da malha
    Cell *celula;

    if(malha->firstGridCell != NULL)
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
        {
            if( celula->active )
            {
                // Corrige velocidade u na interface leste da celula atual
                // Se nao esta na fronteira leste
                if( celula->east->type == 'b' )
                    celula->eastFaceU -= celula->d_e * ( celula->east->pLinha - celula->pLinha );

                // Corrige velocidade u na interface oeste da celula atual
                // Se nao esta na fronteira oeste
                if( celula->west->type == 'b' )
                    celula->westFaceU -= celula->d_w * ( celula->pLinha - celula->west->pLinha );

                // Corrige velocidade v na interface norte da celula atual
                // Se nao esta na fronteira norte
                if( celula->north->type == 'b' )
                    celula->northFaceV -= celula->d_n * ( celula->north->pLinha - celula->pLinha );

                // Corrige velocidade v na interface sul da celula atual
                // Se nao esta na fronteira sul
                if( celula->south->type == 'b' )
                    celula->southFaceV -= celula->d_s * ( celula->pLinha - celula->south->pLinha );

                // Corrige as velocidades no centro da celula atraves de media das interfaces
                celula->uAtual = ( celula->eastFaceU + celula->westFaceU ) / 2.0;
                celula->vAtual = ( celula->northFaceV + celula->southFaceV ) / 2.0;

                // Corrige a densidade no centro da celula atual
                celula->rhoAtual += celula->pLinha / ( R * celula->TAtual );

                // Finalmente, corrige a pressao no centro da celula atual
                celula->pAtual = celula->pAtual + celula->pLinha;
            }
        }

        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em corrige_Velocidades_Densidades_Pressoes()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em corrige_Velocidades_Densidades_Pressoes()" << endl;
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
        // Verifica se os vizinhos sao de fronteira ou internos 'a malha
        if( vizLeste->type == 'b' )     // Celula interna 'a malha
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varLeste = vizLeste->rhoAtual;
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
        else    // Celula imagina'ria "fronteira" (condicao de fronteira)
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varLeste = condicaoFronteira( celula, ID_VAR_RHO, 'e', false );
                break;

                case ID_VAR_T:
                    *varLeste = condicaoFronteira( celula, ID_VAR_T, 'e', false );
                break;

                case ID_VAR_P:
                    *varLeste = condicaoFronteira( celula, ID_VAR_P, 'e', false );
                break;

                default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                    IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                    cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
                break;
            }
        }
    }

    if( varOeste != NULL )
    {
        if( vizOeste->type == 'b' )     // Celula interna 'a malha
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varOeste = vizOeste->rhoAtual;
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
        else // Celula imagina'ria "fronteira" (condicao de fronteira)
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varOeste = condicaoFronteira( celula, ID_VAR_RHO, 'w', false );
                break;

                case ID_VAR_T:
                    *varOeste = condicaoFronteira( celula, ID_VAR_T, 'w', false );
                break;

                case ID_VAR_P:
                    *varOeste = condicaoFronteira( celula, ID_VAR_P, 'w', false );
                break;

                default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                    IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                    cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
                break;
            }
        }
    }

    if( varNorte != NULL )
    {
        if( vizNorte->type == 'b' )     // Celula interna 'a malha
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varNorte = vizNorte->rhoAtual;
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
        else        // Celula imagina'ria "fronteira" (condicao de fronteira)
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varNorte = condicaoFronteira( celula, ID_VAR_RHO, 'n', false );
                break;

                case ID_VAR_T:
                    *varNorte = condicaoFronteira( celula, ID_VAR_T, 'n', false );
                break;

                case ID_VAR_P:
                    *varNorte = condicaoFronteira( celula, ID_VAR_P, 'n', false );
                break;

                default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                    IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                    cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
                break;
            }
        }
    }

    if( varSul != NULL )
    {
        if( vizSul->type == 'b') // Celula interna 'a malha
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varSul = vizSul->rhoAtual;
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
        else // Celula imagina'ria "fronteira" (condicao de fronteira)
        {
            switch( id_variavel )
            {
                case ID_VAR_RHO:
                    *varSul = condicaoFronteira( celula, ID_VAR_RHO, 's', false );
                break;

                case ID_VAR_T:
                    *varSul = condicaoFronteira( celula, ID_VAR_T, 's', false );
                break;

                case ID_VAR_P:
                    *varSul = condicaoFronteira( celula, ID_VAR_P, 's', false );
                break;

                default:        // se nao e' nenhuma dessas variaveis, imprime um erro
                    IMPRIMIR_ERRO_VAR_NAO_IMPLEMENTADA();
                    cout << "Erro acima ocorreu em obtemVariaveisVizinhas()" << endl;
                break;
            }
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

Criado por Fernando Grossi dia 25/04/07
______________________________________________________________________________*/
int atualizaParametrosDaIteracaoAnterior( Grid *malha, char ciclo )
{
    Cell *celula;

    if( ciclo != 'B' 
    &&  ciclo != 'C' )
    {
        cout << "erro ciclo inexistente\n";
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
            // atualiza parametros
            if( ciclo == 'B' )
            {
                celula->uIterBAnterior = celula->uAtual;
                celula->vIterBAnterior = celula->vAtual;
                celula->pIterBAnterior = celula->pAtual;
                //celula->rhoIterBAnterior = celula->rhoAtual;
            }
            else if( ciclo == 'C' )
            {
                celula->uIterCAnterior = celula->uAtual;
                celula->vIterCAnterior = celula->vAtual;
                celula->pIterCAnterior = celula->pAtual;
                //celula->rhoIterCAnterior = celula->rhoAtual;
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
            // atualiza parametros
            celula->uAnterior = celula->uAtual;
            celula->vAnterior = celula->vAtual;
            celula->pAnterior = celula->pAtual;
            celula->rhoAnterior = celula->rhoAtual;
        }
 
        return SUCESSO;
    }
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em atualizaParametrosDoTempoAnterior()" << endl;
        return ERRO_MALHA_VAZIA;
    }
 
    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em atualizaParametrosDoTempoAnterior()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  convergenciaPressao()

   Verifica a convergencia das pressoes.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 25/04/07
______________________________________________________________________________*/
int convergenciaPressao( Grid *malha, char ciclo )
{
    double erroPressaoCelula = 0.0,
           erroPressaoGlobal = 0.0;

    Cell *celula;

    if( ciclo != 'B' 
    &&  ciclo != 'C' )
    {
        cout << "erro ciclo inexistente\n";
        return INSUCESSO;
    }

    // Se a malha possui celulas (foi inicializada)
    if( malha->firstGridCell != NULL )
    {
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for( celula = malha->firstGridCell;
             celula != NULL;
             celula = celula->next)
        {
            // calcula erros
            if( ciclo == 'B' )
            {
                erroPressaoCelula = fabs( 
                                     (celula->pAtual - celula->pIterBAnterior) 
                                        / celula->pIterBAnterior
                                    );
            }
            else if( ciclo == 'C' )
            {
                erroPressaoCelula = fabs( 
                                     (celula->pAtual - celula->pIterCAnterior) 
                                        / celula->pIterCAnterior
                                    );
            }  

            if ( erroPressaoCelula > erroPressaoGlobal )
                   erroPressaoGlobal = erroPressaoCelula;
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
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em convergenciaPressao()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em convergenciaPressao()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                  convergenciaVelocidades()

   Verifica a convergencia das velovidades.

   Parametros:
       malha: apontador para a malha em questao;

Criado por Fernando Grossi dia 25/04/07
______________________________________________________________________________*/
int convergenciaVelocidades( Grid *malha, char ciclo )
{
    double erroUcelula = 0.0,
           erroVcelula = 0.0,
           erroVelocidadesCelula = 0.0,
           erroVelocidadesGlobal = 0.0;

    Cell *celula;

    if( ciclo != 'B' 
    &&  ciclo != 'C' )
    {
        cout << "erro ciclo inexistente\n";
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
            // calcula erros
            if( ciclo == 'B' )
            {
                erroUcelula = fabs(
                               (celula->uAtual - celula->uIterBAnterior) 
                                 / celula->uIterBAnterior
                              );
                erroVcelula = fabs(
                               (celula->vAtual - celula->vIterBAnterior) 
                                 / celula->vIterBAnterior
                              );
            }
            else if( ciclo == 'C' )
            {
                erroUcelula = fabs(
                               (celula->uAtual - celula->uIterCAnterior) 
                                 / celula->uIterCAnterior
                              );
                erroVcelula = fabs(
                               (celula->vAtual - celula->vIterCAnterior) 
                                 / celula->vIterCAnterior
                              );
            }
 
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
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em convergenciaVelocidades()" << endl;
        return ERRO_MALHA_VAZIA;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "Erro acima ocorreu em convergenciaVelocidades()" << endl;
    return ERRO_FLUXO_EXECUCAO;
}

/*______________________________________________________________________________

                        FUNCTION biConjugateGradientStabilized()
                        
    Biconjugate gradient stabilized method for nonsymmetric matrices.
    Notation close to the one used in the original van der Vorst paper.
    
______________________________________________________________________________*/
int gradienteBiconjugadoEstabilizado( Grid *malha, short id_variavel )
{
    double error = 0.0,	     //Euclidean norm of second resídue r after each iteration.
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
    &&  id_variavel != ID_VAR_P )
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

            case ID_VAR_P: // Pressao ========================
                element = gridCell->firstElementPressao;
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

                case ID_VAR_P: // Pressao ========================
                    gridCell->Ap += element->value * element->cell->pLinha;
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

            case ID_VAR_P: // Pressao ========================
                gridCell->r = gridCell->b_Pressao - gridCell->Ap;
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
    while( error > tolerance )// && rho > tolerance && omega > tolerance )
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

                case ID_VAR_P: // Pressao ========================
                    element = gridCell->firstElementPressao;
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

                case ID_VAR_P: // Pressao ========================
                    element = gridCell->firstElementPressao;
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

                case ID_VAR_P: // Pressao ========================
                    gridCell->pLinha += alpha*gridCell->p + omega*gridCell->s;
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

    cout << "Custo do gradiente biconjugado estabilizado: " << numberOfIterations << " iteracoes."  
         << endl; 

    return SUCESSO;     
}//end biConjugateGradientStabilized() function.


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
    &&  id_variavel != ID_VAR_P )
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

                case ID_VAR_P: // Pressao ========================
                    element = gridCell->firstElementPressao;
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

                    case ID_VAR_P: // Pressao ========================
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

                case ID_VAR_P: // Pressao ========================
                    gridCell->r = gridCell->b_Pressao - gridCell->Ax;
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

                        case ID_VAR_P: // Pressao ========================
                            element = gridCell->firstElementPressao;
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

                        case ID_VAR_P: // Pressao ========================
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
     
    cout << "Custo do gradiente conjugado: " << 100.0*(   ( (double)iteracoes )
                                                                   / ( (double)malha->numberOfCells )
                                                )  
         << " %" << endl;  
    }

    return SUCESSO;
}




/*______________________________________________________________________________

                              verificaResultado()
______________________________________________________________________________*/
void verificaResultado( Grid *malha )
{
	Cell *celula;
	double deltaUnaVertical = 0.0;
	double somaErros = 0.0;

	celula = malha->firstGridCell;
	deltaUnaVertical = fabs(celula->uAtual - celula->southFaceU);
	celula = celula->next;

	// Para cada celula da malha (correspondendo a uma linha da matriz)
    for( ;
         celula != NULL;
         celula = celula->next)
    {
		// Obtem diferenca do u_s com u_P
		somaErros += fabs( deltaUnaVertical - fabs(celula->uAtual - celula->southFaceU) );
		deltaUnaVertical = fabs(celula->uAtual - celula->southFaceU);
	}

	somaErros = somaErros / malha->numberOfCells;

	cout << "\nMedia dos erros = " << somaErros << endl;

}








// Imprime a matriz de alguma variavel
void imprimeMatriz( Grid *malha, short id_variavel )
{
    Cell *celula;

    Element *element;

    // Verifica se a id de variavel solicitada tem matriz correspondente
    if( id_variavel != ID_VAR_U
    &&  id_variavel != ID_VAR_V
    &&  id_variavel != ID_VAR_P )
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

            case ID_VAR_P: // Pressao ========================
                cout << "\n\nImprimindo a matriz da pressao\n";
            break;
        }



     
        // Para cada celula da malha (correspondendo a uma linha da matriz)
        for(celula = malha->firstGridCell;
            celula != NULL;
            celula = celula->next)
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

                    case ID_VAR_P: // Pressao ========================
                        element = celula->firstElementPressao;
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
    else
    {
        IMPRIMIR_ERRO_MALHA_VAZIA();
        cout << "Erro acima ocorreu em ImprimeMatriz()" << endl;
    }
}
