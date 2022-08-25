 /*  ========== derivadas.cpp =========
    Derivadas du/dx, dv/dx, ...
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

                                 dudx()

           Calcula derivada horizontal de u em uma face solicitada.

    Parametros:
        celula: celula principal (central) do calculo.
        face: em qual face a derivada sera' calculada.

Criado por Andre STALTZ dia 25/04/07
______________________________________________________________________________*/
double dudx( Cell *celula, char face )
{
    // Verifica validade do parametro
    if( celula == NULL )
    {
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;
    }
    else 
    {
        Cell *Leste, *Oeste;

        double deltaX = 1.0;
        double du_dx = 0.0;

        Leste = celula->east;
        Oeste = celula->west;

        switch( face )
        {             // ---------------------------------------------------------
            case 'n': // -------------- FACE NORTE -------------------------------         
                             // ----- vizinho LESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Leste->level > celula->level )
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o de cima, entre os do leste
        	       Leste = Leste->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {
    		      Leste = Leste->singleConnector;
   		   }
                           // ----- vizinho OESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Oeste->level > celula->level )
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o de cima, entre os do oeste
        	       Oeste = Oeste->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {
    		      Oeste = Oeste->singleConnector;
   		   }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Leste->type == 'w') || (Leste->active == false) )
                    Leste = celula; // Isto denota que Leste e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Oeste->type == 'w') || (Oeste->active == false) )
                    Oeste = celula; // Isto denota que Oeste e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Leste e Oeste invalidos )
                if( (Leste->type == 'b') && (Leste->active == true) )
                {
                    if( Leste->centerY + Leste->halfFaceLength !=
                        celula->centerY + celula->halfFaceLength )
                        Leste = celula; // Isto denota que Leste e' invalido
                }

                if( (Oeste->type == 'b') && (Oeste->active == true) )
                {
                    if( Oeste->centerY + Oeste->halfFaceLength !=
                        celula->centerY + celula->halfFaceLength )
                        Oeste = celula; // Isto denota que Oeste e' invalido
                }

                // Delta x total entre extremos (east--west ou atual--west ou east--atual)
                deltaX =  
                ( 
                    ( Oeste == celula )? 
                        0 
                        : 
                    (Oeste->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( Leste == celula )? 
                        0 
                        : 
                    (Leste->halfFaceLength + celula->halfFaceLength)
                );

                // --------- CALCULANDO DU_DX --------
                // Caso em que nem East nem West sao invalidos
                if( Leste != celula && Oeste != celula )
                {
                    // Diferencas centradas
                    du_dx = (Leste->northFaceU - Oeste->northFaceU) 
                                / deltaX;
                    // Diferencas pra tras
                    //du_dx = (celula->northFaceU - Oeste->northFaceU) 
                    //                / deltaX;
                }
                // Divisao de casos considerando East ou West invalidos
                if( Leste == celula )
                {
                    du_dx = (celula->northFaceU - Oeste->northFaceU) 
                                / deltaX;
                }
                if( Oeste == celula )
                {
                    du_dx = (Leste->northFaceU - celula->northFaceU) 
                                / deltaX;
                }
            break;

                      // ---------------------------------------------------------
            case 's': // -------------- FACE SUL -------------------------------                         
                        // ----- vizinho LESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Leste->level > celula->level )
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o de baixo, entre os do leste
        	       Leste = Leste->doubleConnector2;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {
    		      Leste = Leste->singleConnector;
   		   }
                           // ----- vizinho OESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Oeste->level > celula->level )
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o de baixo, entre os do oeste
        	       Oeste = Oeste->doubleConnector2;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {
    		      Oeste = Oeste->singleConnector;
   		   }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Leste->type == 'w') || (Leste->active == false) )
                    Leste = celula; // Isto denota que Leste e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Oeste->type == 'w') || (Oeste->active == false) )
                    Oeste = celula; // Isto denota que Oeste e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Leste e Oeste invalidos )
                if( (Leste->type == 'b') && (Leste->active == true) )
                {
                    if( Leste->centerY - Leste->halfFaceLength !=
                        celula->centerY - celula->halfFaceLength )
                        Leste = celula; // Isto denota que Leste e' invalido
                }

                if( (Oeste->type == 'b') && (Oeste->active == true) )
                {
                    if( Oeste->centerY - Oeste->halfFaceLength !=
                        celula->centerY - celula->halfFaceLength )
                        Oeste = celula; // Isto denota que Oeste e' invalido
                }

                // Delta x total entre extremos (east--west ou atual--west ou east--atual)
                deltaX =  
                ( 
                    ( Oeste == celula )? 
                        0 
                        : 
                    (Oeste->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( Leste == celula )? 
                        0 
                        : 
                    (Leste->halfFaceLength + celula->halfFaceLength)
                );

                // --------- CALCULANDO DU_DX --------
                // Caso em que nem East nem West sao invalidos
                if( Leste != celula && Oeste != celula )
                {
                    // Diferencas centradas
                    du_dx = (Leste->southFaceU - Oeste->southFaceU) 
                                / deltaX;
                    // Diferencas pra tras
                    //du_dx = (celula->southFaceU - Oeste->southFaceU) 
                    //                / deltaX;
                }
                // Divisao de casos considerando East ou West invalidos
                if( Leste == celula )
                {
                    du_dx = (celula->southFaceU - Oeste->southFaceU) 
                                / deltaX;
                }
                if( Oeste == celula )
                {
                    du_dx = (Leste->southFaceU - celula->southFaceU) 
                                / deltaX;
                }
            break;

            default: // Outras faces
                IMPRIMIR_ERRO_FACE_INVALIDA_DUDX();
                return 0.0;
            break;
        }

        return du_dx;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao dudx()";
    return 0.0;
}



/*______________________________________________________________________________

                                 dudy()

           Calcula derivada vertical de u em uma face solicitada.

    Parametros:
        celula: celula principal (central) do calculo.
        face: em qual face a derivada sera' calculada.

Criado por Andre STALTZ dia 25/04/07
______________________________________________________________________________*/
double dudy( Cell *celula, char face )
{
    // Verifica validade do parametro
    if( celula == NULL )
    {
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;
    }
    else 
    {
        Cell *Norte, *Sul;

        double deltaY = 1.0;
        double du_dy = 0.0;

        Norte = celula->north;
        Sul = celula->south;

        bool northContribui  = true,
             southContribui  = true;

        switch( face )
        {
                      // ---------------------------------------------------------
            case 'e': // -------------- FACE LESTE -------------------------------         
                     
                        // ----- vizinho NORTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Norte->level > celula->level )
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o da direita, entre os do norte
        	       Norte = Norte->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {
    		      Norte = Norte->singleConnector;
   		   }
                           // ----- vizinho SUL ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Sul->level > celula->level )
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o da direita, entre os do sul
        	       Sul = Sul->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {
    		      Sul = Sul->singleConnector;
   	           }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Norte->type == 'w') || (Norte->active == false) )
                    northContribui = false; // Isto denota que Norte e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Sul->type == 'w') || (Sul->active == false) )
                    southContribui = false; // Isto denota que Sul e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Norte e Sul invalidos )
                if( (Norte->type == 'b') && (Norte->active == true) )
                {
                    if( Norte->centerX + Norte->halfFaceLength !=
                    celula->centerX + celula->halfFaceLength )
                        northContribui = false; // Isto denota que Norte e' invalido
                }

                if( (Sul->type == 'b') && (Sul->active == true) )
                {
                    if( Sul->centerX + Sul->halfFaceLength !=
                        celula->centerX + celula->halfFaceLength )
                        southContribui = false; // Isto denota que Sul e' invalido
                }

                // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
                deltaY =  
                ( 
                    ( !northContribui )? 
                        0 
                        : 
                    (Norte->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( !southContribui )? 
                        0 
                        : 
                    (Sul->halfFaceLength + celula->halfFaceLength)
                );

                // --------- CALCULANDO DU_DY --------
                // Caso em que nem North nem South sao invalidos
                if( northContribui && southContribui )
                {
                    // Diferencas centradas
                    du_dy = (Norte->eastFaceU - Sul->eastFaceU) 
                                / deltaY;
                    // Diferencas pra tras
                    //du_dy = (celula->eastFaceU - Sul->eastFaceU) 
                    //               / deltaY;
                }
                // Divisao de casos considerando North ou South invalidos
                if( !northContribui )
                {
                    du_dy = (celula->eastFaceU - Sul->eastFaceU) 
                                / deltaY;
                }
                if( !southContribui )
                {
                    du_dy = (Norte->eastFaceU - celula->eastFaceU) 
                                / deltaY;
                }
            break;
           
                      // ---------------------------------------------------------
            case 'w': // -------------- FACE OESTE -------------------------------         
                      
                        // ----- vizinho NORTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Norte->level > celula->level )
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o da esquerda, entre os do norte
        	       Norte = Norte->doubleConnector2;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {
    		      Norte = Norte->singleConnector;
   		   }
                           // ----- vizinho SUL ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Sul->level > celula->level )
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o da esquerda, entre os do sul
        	       Sul = Sul->doubleConnector2;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {
    		      Sul = Sul->singleConnector;
   		   }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Norte->type == 'w') || (Norte->active == false) )
                    northContribui = false; // Isto denota que Norte e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Sul->type == 'w') || (Sul->active == false) )
                    southContribui = false; // Isto denota que Sul e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Norte e Sul invalidos )
                if( (Norte->type == 'b') && (Norte->active == true) )
                {
                    if( Norte->centerX - Norte->halfFaceLength !=
                        celula->centerX - celula->halfFaceLength )
                        northContribui = false; // Isto denota que Norte e' invalido
                }

                if( (Sul->type == 'b') && (Sul->active == true) )
                {
                    if( Sul->centerX - Sul->halfFaceLength !=
                        celula->centerX - celula->halfFaceLength )
                        southContribui = false; // Isto denota que Sul e' invalido
                }

                // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
                deltaY =  
                ( 
                    ( !northContribui )? 
                        0 
                        : 
                    (Norte->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( !southContribui )? 
                        0 
                        : 
                    (Sul->halfFaceLength + celula->halfFaceLength)
                );
                 
                // --------- CALCULANDO DU_DY --------
                // Caso em que nem North nem South sao invalidos
                if( northContribui && southContribui )
                {
                    // Diferencas centradas
                    du_dy = (Norte->westFaceU - Sul->westFaceU) 
                                / deltaY;
                    // Diferencas pra tras
                    //du_dy = (celula->westFaceU - Sul->westFaceU) 
                    //                / deltaY;
                }
                // Divisao de casos considerando North ou South invalidos
                if( !northContribui )
                {
                    du_dy = (celula->westFaceU - Sul->westFaceU) 
                                / deltaY;
                }
                if( !southContribui )
                {
                    du_dy = (Norte->westFaceU - celula->westFaceU) 
                                / deltaY;
                }
            break;

            default: // Outras faces
                IMPRIMIR_ERRO_FACE_INVALIDA_DUDY();
                return 0.0;
            break;
        }

        return du_dy;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao dudy()";
    return 0.0;
}


/*______________________________________________________________________________

                                 dvdx()

           Calcula derivada horizontal de v em uma face solicitada.

    Parametros:
        celula: celula principal (central) do calculo.
        face: em qual face a derivada sera' calculada.

Criado por Andre STALTZ dia 24/04/07
______________________________________________________________________________*/
double dvdx( Cell *celula, char face )
{
    // Verifica validade do parametro
    if( celula == NULL )
    {
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;
    }
    else 
    {
        Cell *Leste, *Oeste;

        double deltaX = 1.0;
        double dv_dx = 0.0;

        bool westContribui  = true,
             eastContribui  = true;

        Leste = celula->east;
        Oeste = celula->west;

        switch( face )
        {             // ---------------------------------------------------------
            case 'n': // -------------- FACE NORTE -------------------------------         
                        // ----- vizinho LESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Leste->level > celula->level )
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o de cima, entre os do leste
        	       Leste = Leste->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	            while( ( Leste->type == 'w' ) && 
                           ( Leste->singleConnector != 0 ) 
                    )
    	            {
    		       Leste = Leste->singleConnector;
   	            }
                           // ----- vizinho OESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Oeste->level > celula->level )
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o de cima, entre os do oeste
        	       Oeste = Oeste->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {
    		      Oeste = Oeste->singleConnector;
   	           }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Leste->type == 'w') || (Leste->active == false) )
                    eastContribui = false; // Isto denota que Leste e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Oeste->type == 'w') || (Oeste->active == false) )
                    westContribui = false; // Isto denota que Oeste e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Leste e Oeste invalidos )
                if( (Leste->type == 'b') && (Leste->active == true) )
                {
                    if( Leste->centerY + Leste->halfFaceLength !=
                        celula->centerY + celula->halfFaceLength )
                        eastContribui = false; // Isto denota que Leste e' invalido
                }

                if( (Oeste->type == 'b') && (Oeste->active == true) )
                {
                    if( Oeste->centerY + Oeste->halfFaceLength !=
                        celula->centerY + celula->halfFaceLength )
                        westContribui = false; // Isto denota que Oeste e' invalido
                }

                // Delta x total entre extremos (east--west ou atual--west ou east--atual)
                deltaX =  
                ( 
                    ( !westContribui )? 
                        0 
                        : 
                    (Oeste->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( !eastContribui )? 
                        0 
                        : 
                    (Leste->halfFaceLength + celula->halfFaceLength)
                );

                // --------- CALCULANDO DV_DX --------
                // Caso em que nem East nem West sao invalidos
                if( eastContribui && westContribui )
                {   
                    // Diferencas centradas
                    dv_dx = (Leste->northFaceV - Oeste->northFaceV) 
                                / deltaX;
                    // Diferencas pra tras
                    //dv_dx = (celula->northFaceV - Oeste->northFaceV) 
                    //                / deltaX;
                }
                // Divisao de casos considerando East ou West invalidos
                if( !eastContribui )
                {
                    dv_dx = (celula->northFaceV - Oeste->northFaceV) 
                                / deltaX;
                }
                if( !westContribui )
                {
                    dv_dx = (Leste->northFaceV - celula->northFaceV) 
                                / deltaX;
                }
            break;

            // ---------------------------------------------------------
            case 's': // -------------- FACE SUL -------------------------------         
                        // ----- vizinho LESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if( Leste->level > celula->level )
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o de baixo, entre os do leste
        	       Leste = Leste->doubleConnector2;
    	           } 
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Leste->type == 'w' ) && 
                          ( Leste->singleConnector != 0 ) 
                   )
    	           {
    		      Leste = Leste->singleConnector;
   	           }
                           // ----- vizinho OESTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Oeste->level > celula->level )
    	           while( ( Oeste->type == 'w' ) && 
                          ( Oeste->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o de baixo, entre os do oeste
        	       Oeste = Oeste->doubleConnector2;
    	           }   
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Oeste->type == 'w' ) && 
                        ( Oeste->singleConnector != 0 ) 
                   )
    	           {
    		        Oeste = Oeste->singleConnector;
   	           }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Leste->type == 'w') || (Leste->active == false) )
                    eastContribui = false; // Isto denota que Leste e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Oeste->type == 'w') || (Oeste->active == false) )
                    westContribui = false; // Isto denota que Oeste e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Leste e Oeste invalidos )
                if( (Leste->type == 'b') && (Leste->active == true) )
                {
                    if( Leste->centerY - Leste->halfFaceLength !=
                        celula->centerY - celula->halfFaceLength )
                        eastContribui = false; // Isto denota que Leste e' invalido
                }

                if( (Oeste->type == 'b') && (Oeste->active == true) )
                {
                    if( Oeste->centerY - Oeste->halfFaceLength !=
                        celula->centerY - celula->halfFaceLength )
                        westContribui = false; // Isto denota que Oeste e' invalido
                }

                // Delta x total entre extremos (east--west ou atual--west ou east--atual)
                deltaX =  
                ( 
                    ( !westContribui )? 
                        0 
                        : 
                    (Oeste->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( !eastContribui )? 
                         0 
                        : 
                     (Leste->halfFaceLength + celula->halfFaceLength)
                  );

                // --------- CALCULANDO DV_DX --------
                // Caso em que nem East nem West sao invalidos
                if( eastContribui && westContribui )
                {
                    // Diferencas centradas
                    dv_dx = (Leste->southFaceV - Oeste->southFaceV) 
                                    / deltaX;
                    // Diferencas pra tras
                    //dv_dx = (celula->southFaceV - Oeste->southFaceV) 
                    //                / deltaX;
                }
               // Divisao de casos considerando East ou West invalidos
                if( !eastContribui )
                {
                    dv_dx = (celula->southFaceV - Oeste->southFaceV) 
                                    / deltaX;
                }
                if( !westContribui )
                {
                    dv_dx = (Leste->southFaceV - celula->southFaceV) 
                                    / deltaX;
                }
            break;
    

            default: // Outras faces
                IMPRIMIR_ERRO_FACE_INVALIDA_DVDX();
                return 0.0;
            break;
        }

        return dv_dx;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao dvdx()";
    return 0.0;
}


/*______________________________________________________________________________

                                 dvdy()

           Calcula derivada vertical de v em uma face solicitada.

    Parametros:
        celula: celula principal (central) do calculo.
        face: em qual face a derivada sera' calculada.

Criado por Andre STALTZ dia 24/04/07
______________________________________________________________________________*/
double dvdy( Cell *celula, char face )
{
    // Verifica validade do parametro
    if( celula == NULL )
    {
        IMPRIMIR_ERRO_CELULA_INEXISTENTE();
        return 0.0;
    }
    else 
    {
        Cell *Norte, *Sul;

        double deltaY = 1.0;
        double dv_dy = 0.0;

        Norte = celula->north;
        Sul = celula->south;

        switch( face )
        {            // ---------------------------------------------------------
            case 'e': // -------------- FACE LESTE -------------------------------         
                        // ----- vizinho NORTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Norte->level > celula->level )
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o da direita, entre os do norte
        	       Norte = Norte->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {
    		      Norte = Norte->singleConnector;
   		   }
                           // ----- vizinho SUL ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Sul->level > celula->level )
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {   // DC1 e' o da direita, entre os do sul
        	       Sul = Sul->doubleConnector1;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {
    		      Sul = Sul->singleConnector;
   		   }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Norte->type == 'w') || (Norte->active == false) )
                    Norte = celula; // Isto denota que Norte e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Sul->type == 'w') || (Sul->active == false) )
                    Sul = celula; // Isto denota que Sul e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Norte e Sul invalidos )
                if( (Norte->type == 'b') && (Norte->active == true) )
                {
                    if( Norte->centerX + Norte->halfFaceLength !=
                        celula->centerX + celula->halfFaceLength )
                        Norte = celula; // Isto denota que Norte e' invalido
                }

                if( (Sul->type == 'b') && (Sul->active == true) )
                {
                    if( Sul->centerX + Sul->halfFaceLength !=
                        celula->centerX + celula->halfFaceLength )
                        Sul = celula; // Isto denota que Sul e' invalido
                }

                // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
                deltaY =  
                 ( 
                    ( Norte == celula )? 
                        0 
                        : 
                    (Norte->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( Sul == celula )? 
                        0 
                        : 
                    (Sul->halfFaceLength + celula->halfFaceLength)
                );

                // --------- CALCULANDO DV_DY --------
                // Caso em que nem North nem South sao invalidos
                if( Norte != celula && Sul != celula )
                {
                    // Diferencas centradas
                    dv_dy = (Norte->eastFaceV - Sul->eastFaceV) 
                                / deltaY;
                    // Diferencas pra tras
                    //dv_dy = (celula->eastFaceV - Sul->eastFaceV) 
                    //                / deltaY;
                }
                // Divisao de casos considerando North ou South invalidos
                if( Norte == celula )
                {
                    dv_dy = (celula->eastFaceV - Sul->eastFaceV) 
                                / deltaY;
                }
                if( Sul == celula )
                {
                    dv_dy = (Norte->eastFaceV - celula->eastFaceV) 
                                / deltaY;
                }
            break;

                      // ---------------------------------------------------------
            case 'w': // -------------- FACE OESTE -------------------------------         
                        
                        // ----- vizinho NORTE ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Norte->level > celula->level )
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o da esquerda, entre os do norte
        	       Norte = Norte->doubleConnector2;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Norte->type == 'w' ) && 
                          ( Norte->singleConnector != 0 ) 
                   )
    	           {
    		      Norte = Norte->singleConnector;
   		   }
                           // ----- vizinho SUL ------
                // Buscar vizinho preto quando ele e' menor que a celula atual
                if ( Sul->level > celula->level )
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {   // DC2 e' o da esquerda, entre os do sul
        	       Sul = Sul->doubleConnector2;
    	           }
                // Buscar vizinho preto quando ele e' maior que a celula atual
                else 
    	           while( ( Sul->type == 'w' ) && 
                          ( Sul->singleConnector != 0 ) 
                   )
    	           {
    		      Sul = Sul->singleConnector;
   		   }

                // Verifica se algum desses vizinhos e' fronteira
                if( (Norte->type == 'w') || (Norte->active == false) )
                    Norte = celula; // Isto denota que Norte e' invalido
            
                // Verifica se algum desses vizinhos e' fronteira
                if( (Sul->type == 'w') || (Sul->active == false) )
                    Sul = celula; // Isto denota que Sul e' invalido

                // Verifica se algum desses vizinhos nao compartilha face, isso 
                // ocorre algumas vezes quando o vizinho e' maior que o atual
                // ( pode ser provado que nunca havera' simultaneamente
                //   Norte e Sul invalidos )
                if( (Norte->type == 'b') && (Norte->active == true) )
                {
                    if( Norte->centerX - Norte->halfFaceLength !=
                        celula->centerX - celula->halfFaceLength )
                        Norte = celula; // Isto denota que Norte e' invalido
                }

                if( (Sul->type == 'b') && (Sul->active == true) )
                {
                    if( Sul->centerX - Sul->halfFaceLength !=
                        celula->centerX - celula->halfFaceLength )
                        Sul = celula; // Isto denota que Sul e' invalido
                }

                // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
                deltaY =  
                ( 
                    ( Norte == celula )? 
                        0 
                        : 
                    (Norte->halfFaceLength + celula->halfFaceLength)
                )   
                + 
                ( 
                    ( Sul == celula )? 
                        0 
                        : 
                    (Sul->halfFaceLength + celula->halfFaceLength)
                );

                // --------- CALCULANDO DV_DY --------
                // Caso em que nem North nem South sao invalidos
                if( Norte != celula && Sul != celula )
                {
                    // Diferencas centradas
                    dv_dy = (Norte->westFaceV - Sul->westFaceV) 
                                / deltaY;
                    // Diferencas pra tras
                    //dv_dy = (celula->westFaceV - Sul->westFaceV) 
                    //                / deltaY;
                }
                // Divisao de casos considerando North ou South invalidos
                if( Norte == celula )
                {
                    dv_dy = (celula->westFaceV - Sul->westFaceV) 
                                / deltaY;
                }
                if( Sul == celula )
                {
                    dv_dy = (Norte->westFaceV - celula->westFaceV) 
                                / deltaY;
                }
            break;
    

            default: // Outras faces
                IMPRIMIR_ERRO_FACE_INVALIDA_DVDY();
                return 0.0;
            break;
        }

        return dv_dy;
    }

    IMPRIMIR_ERRO_FLUXO_EXECUCAO();
    cout << "\nFuncao dvdy()";
    return 0.0;
}
