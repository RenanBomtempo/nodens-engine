 /*  ========== finitedifference.cpp =========
    Contem funcoes que resolvem as tensoes XX, XY, YY, e fluxo de calor QX, QY
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>
#include <iostream>

#include "constants.h"
#include "erros.h"

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
    return 0.0;
}


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
    return 0.0;
}










/*______________________________________________________________________________
                            
                        FUNCTIONs Stress XX, XY, YY

      Calculated by finite difference. The parameter 'face' indicates on
which face of a cell the tension is calculated.
______________________________________________________________________________*/

// ----------------------------- TAU_XX -------------------------------------
double faceTauXX( Cell *gridCell, char face ) 
{
    Cell *cellEast, *cellWest,
         *cellNorth, *cellSouth;

    double deltaX = 1.0;
    double deltaY = 1.0;

    double du_dx, dv_dy;
    double lambdaFace;
    double miFace;

    cellEast = gridCell->east;
    cellWest = gridCell->west;
    cellNorth = gridCell->north;
    cellSouth = gridCell->south;
    
    switch( face )
    {             // ---------------------------------------------------------
        case 'n': // -------------- FACE NORTE -------------------------------         
                        
                        // ----- vizinho LESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellEast->level > gridCell->level )
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o de cima, entre os do leste
        	       cellEast = cellEast->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {
    		      cellEast = cellEast->singleConnector;
   		       }
                           // ----- vizinho OESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellWest->level > gridCell->level )
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o de cima, entre os do oeste
        	       cellWest = cellWest->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {
    		      cellWest = cellWest->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellEast->type == 'w') || (cellEast->active == false) )
                cellEast = gridCell; // Isto denota que cellEast e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellWest->type == 'w') || (cellWest->active == false) )
                cellWest = gridCell; // Isto denota que cellWest e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellEast e cellWest invalidos )
            if( (cellEast->type == 'b') && (cellEast->active == true) )
            {
                if( cellEast->centerY + cellEast->halfFaceLength !=
                    gridCell->centerY + gridCell->halfFaceLength )
                    cellEast = gridCell; // Isto denota que cellEast e' invalido
            }

            if( (cellWest->type == 'b') && (cellWest->active == true) )
            {
                if( cellWest->centerY + cellWest->halfFaceLength !=
                    gridCell->centerY + gridCell->halfFaceLength )
                    cellWest = gridCell; // Isto denota que cellWest e' invalido
            }

          // Delta x total entre extremos (east--west ou atual--west ou east--atual)
            deltaX =  
              ( 
                ( cellWest == gridCell )? 
                    0 
                    : 
                  (cellWest->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellEast == gridCell )? 
                    0 
                    : 
                  (cellEast->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DU_DX --------
            // Caso em que nem East nem West sao invalidos
            if( cellEast != gridCell && cellWest != gridCell )
            {
                // Diferencas centradas
                du_dx = (cellEast->northFaceU - cellWest->northFaceU) 
                                / deltaX;
                // Diferencas pra tras
                //du_dx = (gridCell->northFaceU - cellWest->northFaceU) 
                //                / deltaX;
            }
            // Divisao de casos considerando East ou West invalidos
            if( cellEast == gridCell )
            {
                du_dx = (gridCell->northFaceU - cellWest->northFaceU) 
                                / deltaX;
            }
            if( cellWest == gridCell )
            {
                du_dx = (cellEast->northFaceU - gridCell->northFaceU) 
                                / deltaX;
            }
            
            // --------- CALCULANDO DV_DY --------
            dv_dy = (gridCell->northFaceV - gridCell->vAtual) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->northFaceMi);
            miFace = (gridCell->northFaceMi);

        break;

                  // ---------------------------------------------------------
        case 's': // -------------- FACE SUL -------------------------------         
                        
                        // ----- vizinho LESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellEast->level > gridCell->level )
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o de baixo, entre os do leste
        	       cellEast = cellEast->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {
    		      cellEast = cellEast->singleConnector;
   		       }
                           // ----- vizinho OESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellWest->level > gridCell->level )
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o de baixo, entre os do oeste
        	       cellWest = cellWest->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {
    		      cellWest = cellWest->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellEast->type == 'w') || (cellEast->active == false) )
                cellEast = gridCell; // Isto denota que cellEast e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellWest->type == 'w') || (cellWest->active == false) )
                cellWest = gridCell; // Isto denota que cellWest e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellEast e cellWest invalidos )
            if( (cellEast->type == 'b') && (cellEast->active == true) )
            {
                if( cellEast->centerY - cellEast->halfFaceLength !=
                    gridCell->centerY - gridCell->halfFaceLength )
                    cellEast = gridCell; // Isto denota que cellEast e' invalido
            }

            if( (cellWest->type == 'b') && (cellWest->active == true) )
            {
                if( cellWest->centerY - cellWest->halfFaceLength !=
                    gridCell->centerY - gridCell->halfFaceLength )
                    cellWest = gridCell; // Isto denota que cellWest e' invalido
            }

          // Delta x total entre extremos (east--west ou atual--west ou east--atual)
            deltaX =  
              ( 
                ( cellWest == gridCell )? 
                    0 
                    : 
                  (cellWest->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellEast == gridCell )? 
                    0 
                    : 
                  (cellEast->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DU_DX --------
            // Caso em que nem East nem West sao invalidos
            if( cellEast != gridCell && cellWest != gridCell )
            {
                // Diferencas centradas
                du_dx = (cellEast->southFaceU - cellWest->southFaceU) 
                                / deltaX;
                // Diferencas pra tras
                //du_dx = (gridCell->southFaceU - cellWest->southFaceU) 
                //                / deltaX;
            }
            // Divisao de casos considerando East ou West invalidos
            if( cellEast == gridCell )
            {
                du_dx = (gridCell->southFaceU - cellWest->southFaceU) 
                                / deltaX;
            }
            if( cellWest == gridCell )
            {
                du_dx = (cellEast->southFaceU - gridCell->southFaceU) 
                                / deltaX;
            }
            
            // --------- CALCULANDO DV_DY --------
            dv_dy = (gridCell->vAtual - gridCell->southFaceV) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->southFaceMi);
            miFace = (gridCell->southFaceMi);

        break;

                  // ---------------------------------------------------------
        case 'e': // -------------- FACE LESTE -------------------------------         
                        
                        // ----- vizinho NORTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellNorth->level > gridCell->level )
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o da direita, entre os do norte
        	       cellNorth = cellNorth->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {
    		      cellNorth = cellNorth->singleConnector;
   		       }
                           // ----- vizinho SUL ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellSouth->level > gridCell->level )
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o da direita, entre os do sul
        	       cellSouth = cellSouth->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {
    		      cellSouth = cellSouth->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
                cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
                cellSouth = gridCell; // Isto denota que cellSouth e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellNorth e cellSouth invalidos )
            if( (cellNorth->type == 'b') && (cellNorth->active == true) )
            {
                if( cellNorth->centerX + cellNorth->halfFaceLength !=
                    gridCell->centerX + gridCell->halfFaceLength )
                    cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            }

            if( (cellSouth->type == 'b') && (cellSouth->active == true) )
            {
                if( cellSouth->centerX + cellSouth->halfFaceLength !=
                    gridCell->centerX + gridCell->halfFaceLength )
                    cellSouth = gridCell; // Isto denota que cellSouth e' invalido
            }

          // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
            deltaY =  
              ( 
                ( cellNorth == gridCell )? 
                    0 
                    : 
                  (cellNorth->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellSouth == gridCell )? 
                    0 
                    : 
                  (cellSouth->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DV_DY --------
            // Caso em que nem North nem South sao invalidos
            if( cellNorth != gridCell && cellSouth != gridCell )
            {
                // Diferencas centradas
                dv_dy = (cellNorth->eastFaceV - cellSouth->eastFaceV) 
                                / deltaY;
                // Diferencas pra tras
                //dv_dy = (gridCell->eastFaceV - cellSouth->eastFaceV) 
                //                / deltaY;
            }
            // Divisao de casos considerando North ou South invalidos
            if( cellNorth == gridCell )
            {
                dv_dy = (gridCell->eastFaceV - cellSouth->eastFaceV) 
                                / deltaY;
            }
            if( cellSouth == gridCell )
            {
                dv_dy = (cellNorth->eastFaceV - gridCell->eastFaceV) 
                                / deltaY;
            }
            
            // --------- CALCULANDO DU_DX --------
            du_dx = (gridCell->eastFaceU - gridCell->uAtual) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->eastFaceMi);
            miFace = (gridCell->eastFaceMi);

        break;

                  // ---------------------------------------------------------
        case 'w': // -------------- FACE OESTE -------------------------------         
                        
                        // ----- vizinho NORTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellNorth->level > gridCell->level )
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o da esquerda, entre os do norte
        	       cellNorth = cellNorth->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {
    		      cellNorth = cellNorth->singleConnector;
   		       }
                           // ----- vizinho SUL ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellSouth->level > gridCell->level )
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o da esquerda, entre os do sul
        	       cellSouth = cellSouth->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {
    		      cellSouth = cellSouth->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
                cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
                cellSouth = gridCell; // Isto denota que cellSouth e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellNorth e cellSouth invalidos )
            if( (cellNorth->type == 'b') && (cellNorth->active == true) )
            {
                if( cellNorth->centerX - cellNorth->halfFaceLength !=
                    gridCell->centerX - gridCell->halfFaceLength )
                    cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            }

            if( (cellSouth->type == 'b') && (cellSouth->active == true) )
            {
                if( cellSouth->centerX - cellSouth->halfFaceLength !=
                    gridCell->centerX - gridCell->halfFaceLength )
                    cellSouth = gridCell; // Isto denota que cellSouth e' invalido
            }

          // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
            deltaY =  
              ( 
                ( cellNorth == gridCell )? 
                    0 
                    : 
                  (cellNorth->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellSouth == gridCell )? 
                    0 
                    : 
                  (cellSouth->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DV_DY --------
            // Caso em que nem North nem South sao invalidos
            if( cellNorth != gridCell && cellSouth != gridCell )
            {
                // Diferencas centradas
                dv_dy = (cellNorth->westFaceV - cellSouth->westFaceV) 
                                / deltaY;
                // Diferencas pra tras
                //dv_dy = (gridCell->westFaceV - cellSouth->westFaceV) 
                //                / deltaY;
            }
            // Divisao de casos considerando North ou South invalidos
            if( cellNorth == gridCell )
            {
                dv_dy = (gridCell->westFaceV - cellSouth->westFaceV) 
                                / deltaY;
            }
            if( cellSouth == gridCell )
            {
                dv_dy = (cellNorth->westFaceV - gridCell->westFaceV) 
                                / deltaY;
            }
            
            // --------- CALCULANDO DU_DX --------
            du_dx = (gridCell->uAtual - gridCell->westFaceU) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->westFaceMi);
            miFace = (gridCell->westFaceMi);

        break;

        default: break;
    }
    
    // Retorna TauXX na face solicitada                                 
    return lambdaFace*(du_dx + dv_dy) + 2.0*miFace*du_dx;  
}

// ----------------------------- TAU_XY -------------------------------------
double faceTauXY( Cell *gridCell, char face ) 
{
    Cell *cellEast, *cellWest,
         *cellNorth, *cellSouth;

    double deltaX = 1.0;
    double deltaY = 1.0;
    double du_dy = 0.0, dv_dx = 0.0;
    double miFace = 0.0;

    bool westContribui  = true,
         eastContribui  = true,
         northContribui = true,
         southContribui = true;

    cellEast = gridCell->east;
    cellWest = gridCell->west;
    cellNorth = gridCell->north;
    cellSouth = gridCell->south;

    switch( face )
    {             // ---------------------------------------------------------
        case 'n': // -------------- FACE NORTE -------------------------------         
           
                        // ----- vizinho LESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellEast->level > gridCell->level )
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o de cima, entre os do leste
        	       cellEast = cellEast->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {
    		      cellEast = cellEast->singleConnector;
   		       }
                           // ----- vizinho OESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellWest->level > gridCell->level )
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o de cima, entre os do oeste
        	       cellWest = cellWest->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {
    		      cellWest = cellWest->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellEast->type == 'w') || (cellEast->active == false) )
                eastContribui = false; // Isto denota que cellEast e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellWest->type == 'w') || (cellWest->active == false) )
                westContribui = false; // Isto denota que cellWest e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellEast e cellWest invalidos )
            if( (cellEast->type == 'b') && (cellEast->active == true) )
            {
                if( cellEast->centerY + cellEast->halfFaceLength !=
                    gridCell->centerY + gridCell->halfFaceLength )
                    eastContribui = false; // Isto denota que cellEast e' invalido
            }

            if( (cellWest->type == 'b') && (cellWest->active == true) )
            {
                if( cellWest->centerY + cellWest->halfFaceLength !=
                    gridCell->centerY + gridCell->halfFaceLength )
                    westContribui = false; // Isto denota que cellWest e' invalido
            }

          // Delta x total entre extremos (east--west ou atual--west ou east--atual)
            deltaX =  
              ( 
                ( !westContribui )? 
                    0 
                    : 
                  (cellWest->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( !eastContribui )? 
                    0 
                    : 
                  (cellEast->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DV_DX --------
            // Caso em que nem East nem West sao invalidos
            if( eastContribui && westContribui )
            {   
                // Diferencas centradas
                dv_dx = (cellEast->northFaceV - cellWest->northFaceV) 
                                / deltaX;
                // Diferencas pra tras
                //dv_dx = (gridCell->northFaceV - cellWest->northFaceV) 
                //                / deltaX;
            }
            // Divisao de casos considerando East ou West invalidos
            if( !eastContribui )
            {
                dv_dx = (gridCell->northFaceV - cellWest->northFaceV) 
                                / deltaX;
            }
            if( !westContribui )
            {
                dv_dx = (cellEast->northFaceV - gridCell->northFaceV) 
                                / deltaX;
            }
            
            // --------- CALCULANDO DU_DY --------
            du_dy = (gridCell->northFaceU - gridCell->uAtual) 
                                / (gridCell->halfFaceLength);

            // Mi
            miFace = (gridCell->northFaceMi);
              
        break;

                  // ---------------------------------------------------------
        case 's': // -------------- FACE SUL -------------------------------         
             
                        // ----- vizinho LESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellEast->level > gridCell->level )
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o de baixo, entre os do leste
        	       cellEast = cellEast->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {
    		      cellEast = cellEast->singleConnector;
   		       }
                           // ----- vizinho OESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellWest->level > gridCell->level )
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o de baixo, entre os do oeste
        	       cellWest = cellWest->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {
    		      cellWest = cellWest->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellEast->type == 'w') || (cellEast->active == false) )
                eastContribui = false; // Isto denota que cellEast e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellWest->type == 'w') || (cellWest->active == false) )
                westContribui = false; // Isto denota que cellWest e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellEast e cellWest invalidos )
            if( (cellEast->type == 'b') && (cellEast->active == true) )
            {
                if( cellEast->centerY - cellEast->halfFaceLength !=
                    gridCell->centerY - gridCell->halfFaceLength )
                    eastContribui = false; // Isto denota que cellEast e' invalido
            }

            if( (cellWest->type == 'b') && (cellWest->active == true) )
            {
                if( cellWest->centerY - cellWest->halfFaceLength !=
                    gridCell->centerY - gridCell->halfFaceLength )
                    westContribui = false; // Isto denota que cellWest e' invalido
            }

          // Delta x total entre extremos (east--west ou atual--west ou east--atual)
            deltaX =  
              ( 
                ( !westContribui )? 
                    0 
                    : 
                  (cellWest->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( !eastContribui )? 
                    0 
                    : 
                  (cellEast->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DV_DX --------
            // Caso em que nem East nem West sao invalidos
            if( eastContribui && westContribui )
            {
                // Diferencas centradas
                dv_dx = (cellEast->southFaceV - cellWest->southFaceV) 
                                / deltaX;
                // Diferencas pra tras
                //dv_dx = (gridCell->southFaceV - cellWest->southFaceV) 
                //                / deltaX;
            }
            // Divisao de casos considerando East ou West invalidos
            if( !eastContribui )
            {
                dv_dx = (gridCell->southFaceV - cellWest->southFaceV) 
                                / deltaX;
            }
            if( !westContribui )
            {
                dv_dx = (cellEast->southFaceV - gridCell->southFaceV) 
                                / deltaX;
            }
            
            // --------- CALCULANDO DU_DY --------
            du_dy = (gridCell->uAtual - gridCell->southFaceU) 
                                / (gridCell->halfFaceLength);

            // Mi
            miFace = (gridCell->southFaceMi);
                        
        break;

                  // ---------------------------------------------------------
        case 'e': // -------------- FACE LESTE -------------------------------         
                     
                        // ----- vizinho NORTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellNorth->level > gridCell->level )
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o da direita, entre os do norte
        	       cellNorth = cellNorth->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {
    		      cellNorth = cellNorth->singleConnector;
   		       }
                           // ----- vizinho SUL ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellSouth->level > gridCell->level )
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o da direita, entre os do sul
        	       cellSouth = cellSouth->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {
    		      cellSouth = cellSouth->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
                northContribui = false; // Isto denota que cellNorth e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
                southContribui = false; // Isto denota que cellSouth e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellNorth e cellSouth invalidos )
            if( (cellNorth->type == 'b') && (cellNorth->active == true) )
            {
                if( cellNorth->centerX + cellNorth->halfFaceLength !=
                    gridCell->centerX + gridCell->halfFaceLength )
                    northContribui = false; // Isto denota que cellNorth e' invalido
            }

            if( (cellSouth->type == 'b') && (cellSouth->active == true) )
            {
                if( cellSouth->centerX + cellSouth->halfFaceLength !=
                    gridCell->centerX + gridCell->halfFaceLength )
                    southContribui = false; // Isto denota que cellSouth e' invalido
            }

          // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
            deltaY =  
              ( 
                ( !northContribui )? 
                    0 
                    : 
                  (cellNorth->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( !southContribui )? 
                    0 
                    : 
                  (cellSouth->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DU_DY --------
            // Caso em que nem North nem South sao invalidos
            if( northContribui && southContribui )
            {
                // Diferencas centradas
                du_dy = (cellNorth->eastFaceU - cellSouth->eastFaceU) 
                                / deltaY;
                // Diferencas pra tras
                //du_dy = (gridCell->eastFaceU - cellSouth->eastFaceU) 
                //               / deltaY;
            }
            // Divisao de casos considerando North ou South invalidos
            if( !northContribui )
            {
                du_dy = (gridCell->eastFaceU - cellSouth->eastFaceU) 
                                / deltaY;
            }
            if( !southContribui )
            {
                du_dy = (cellNorth->eastFaceU - gridCell->eastFaceU) 
                                / deltaY;
            }
            
            // --------- CALCULANDO DV_DX --------
            dv_dx = (gridCell->eastFaceV - gridCell->vAtual) 
                                / (gridCell->halfFaceLength);

            // Mi
            miFace = (gridCell->eastFaceMi);
              
        break;
           
                  // ---------------------------------------------------------
        case 'w': // -------------- FACE OESTE -------------------------------         
                      
                        // ----- vizinho NORTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellNorth->level > gridCell->level )
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o da esquerda, entre os do norte
        	       cellNorth = cellNorth->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {
    		      cellNorth = cellNorth->singleConnector;
   		       }
                           // ----- vizinho SUL ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellSouth->level > gridCell->level )
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o da esquerda, entre os do sul
        	       cellSouth = cellSouth->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {
    		      cellSouth = cellSouth->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
                northContribui = false; // Isto denota que cellNorth e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
                southContribui = false; // Isto denota que cellSouth e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellNorth e cellSouth invalidos )
            if( (cellNorth->type == 'b') && (cellNorth->active == true) )
            {
                if( cellNorth->centerX - cellNorth->halfFaceLength !=
                    gridCell->centerX - gridCell->halfFaceLength )
                    northContribui = false; // Isto denota que cellNorth e' invalido
            }

            if( (cellSouth->type == 'b') && (cellSouth->active == true) )
            {
                if( cellSouth->centerX - cellSouth->halfFaceLength !=
                    gridCell->centerX - gridCell->halfFaceLength )
                    southContribui = false; // Isto denota que cellSouth e' invalido
            }

          // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
            deltaY =  
              ( 
                ( !northContribui )? 
                    0 
                    : 
                  (cellNorth->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( !southContribui )? 
                    0 
                    : 
                  (cellSouth->halfFaceLength + gridCell->halfFaceLength)
              );
                 
            // --------- CALCULANDO DU_DY --------
            // Caso em que nem North nem South sao invalidos
            if( northContribui && southContribui )
            {
                // Diferencas centradas
                du_dy = (cellNorth->westFaceU - cellSouth->westFaceU) 
                                / deltaY;
                // Diferencas pra tras
                //du_dy = (gridCell->westFaceU - cellSouth->westFaceU) 
                //                / deltaY;
            }
            // Divisao de casos considerando North ou South invalidos
            if( !northContribui )
            {
                du_dy = (gridCell->westFaceU - cellSouth->westFaceU) 
                                / deltaY;
            }
            if( !southContribui )
            {
                du_dy = (cellNorth->westFaceU - gridCell->westFaceU) 
                                / deltaY;
            }
            
            // --------- CALCULANDO DV_DX --------
            dv_dx = (gridCell->vAtual - gridCell->westFaceV) 
                                / (gridCell->halfFaceLength);

            // Mi
            miFace = (gridCell->westFaceMi);

        break;

        default: break;
    }  /**/
            // du_dy = du_dy + 0.0;
    // Retorna TauXY na face solicitada                                 
    return miFace*(du_dy + dv_dx); 
}

// ----------------------------- TAU_YY -------------------------------------
double faceTauYY( Cell *gridCell, char face ) 
{
    Cell *cellEast, *cellWest,
         *cellNorth, *cellSouth;

    double deltaX = 1.0;
    double deltaY = 1.0;

    double du_dx, dv_dy;
    double lambdaFace;
    double miFace;

    cellEast = gridCell->east;
    cellWest = gridCell->west;
    cellNorth = gridCell->north;
    cellSouth = gridCell->south;
    
    switch( face )
    {             // ---------------------------------------------------------
        case 'n': // -------------- FACE NORTE -------------------------------         
                        
                        // ----- vizinho LESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellEast->level > gridCell->level )
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o de cima, entre os do leste
        	       cellEast = cellEast->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {
    		      cellEast = cellEast->singleConnector;
   		       }
                           // ----- vizinho OESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellWest->level > gridCell->level )
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o de cima, entre os do oeste
        	       cellWest = cellWest->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {
    		      cellWest = cellWest->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellEast->type == 'w') || (cellEast->active == false) )
                cellEast = gridCell; // Isto denota que cellEast e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellWest->type == 'w') || (cellWest->active == false) )
                cellWest = gridCell; // Isto denota que cellWest e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellEast e cellWest invalidos )
            if( (cellEast->type == 'b') && (cellEast->active == true) )
            {
                if( cellEast->centerY + cellEast->halfFaceLength !=
                    gridCell->centerY + gridCell->halfFaceLength )
                    cellEast = gridCell; // Isto denota que cellEast e' invalido
            }

            if( (cellWest->type == 'b') && (cellWest->active == true) )
            {
                if( cellWest->centerY + cellWest->halfFaceLength !=
                    gridCell->centerY + gridCell->halfFaceLength )
                    cellWest = gridCell; // Isto denota que cellWest e' invalido
            }

          // Delta x total entre extremos (east--west ou atual--west ou east--atual)
            deltaX =  
              ( 
                ( cellWest == gridCell )? 
                    0 
                    : 
                  (cellWest->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellEast == gridCell )? 
                    0 
                    : 
                  (cellEast->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DU_DX --------
            // Caso em que nem East nem West sao invalidos
            if( cellEast != gridCell && cellWest != gridCell )
            {
                // Diferencas centradas
                du_dx = (cellEast->northFaceU - cellWest->northFaceU) 
                                / deltaX;
                // Diferencas pra tras
                //du_dx = (gridCell->northFaceU - cellWest->northFaceU) 
                //                / deltaX;
            }
            // Divisao de casos considerando East ou West invalidos
            if( cellEast == gridCell )
            {
                du_dx = (gridCell->northFaceU - cellWest->northFaceU) 
                                / deltaX;
            }
            if( cellWest == gridCell )
            {
                du_dx = (cellEast->northFaceU - gridCell->northFaceU) 
                                / deltaX;
            }
            
            // --------- CALCULANDO DV_DY --------
            dv_dy = (gridCell->northFaceV - gridCell->vAtual) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->northFaceMi);
            miFace = (gridCell->northFaceMi);

        break;

                  // ---------------------------------------------------------
        case 's': // -------------- FACE SUL -------------------------------         
                        
                        // ----- vizinho LESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellEast->level > gridCell->level )
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o de baixo, entre os do leste
        	       cellEast = cellEast->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellEast->type == 'w' ) && 
                    ( cellEast->singleConnector != 0 ) 
               )
    	       {
    		      cellEast = cellEast->singleConnector;
   		       }
                           // ----- vizinho OESTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellWest->level > gridCell->level )
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o de baixo, entre os do oeste
        	       cellWest = cellWest->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellWest->type == 'w' ) && 
                    ( cellWest->singleConnector != 0 ) 
               )
    	       {
    		      cellWest = cellWest->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellEast->type == 'w') || (cellEast->active == false) )
                cellEast = gridCell; // Isto denota que cellEast e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellWest->type == 'w') || (cellWest->active == false) )
                cellWest = gridCell; // Isto denota que cellWest e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellEast e cellWest invalidos )
            if( (cellEast->type == 'b') && (cellEast->active == true) )
            {
                if( cellEast->centerY - cellEast->halfFaceLength !=
                    gridCell->centerY - gridCell->halfFaceLength )
                    cellEast = gridCell; // Isto denota que cellEast e' invalido
            }

            if( (cellWest->type == 'b') && (cellWest->active == true) )
            {
                if( cellWest->centerY - cellWest->halfFaceLength !=
                    gridCell->centerY - gridCell->halfFaceLength )
                    cellWest = gridCell; // Isto denota que cellWest e' invalido
            }

          // Delta x total entre extremos (east--west ou atual--west ou east--atual)
            deltaX =  
              ( 
                ( cellWest == gridCell )? 
                    0 
                    : 
                  (cellWest->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellEast == gridCell )? 
                    0 
                    : 
                  (cellEast->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DU_DX --------
            // Caso em que nem East nem West sao invalidos
            if( cellEast != gridCell && cellWest != gridCell )
            {
                // Diferencas centradas
                du_dx = (cellEast->southFaceU - cellWest->southFaceU) 
                                / deltaX;
                // Diferencas pra tras
                //du_dx = (gridCell->southFaceU - cellWest->southFaceU) 
                //                / deltaX;
            }
            // Divisao de casos considerando East ou West invalidos
            if( cellEast == gridCell )
            {
                du_dx = (gridCell->southFaceU - cellWest->southFaceU) 
                                / deltaX;
            }
            if( cellWest == gridCell )
            {
                du_dx = (cellEast->southFaceU - gridCell->southFaceU) 
                                / deltaX;
            }
            
            // --------- CALCULANDO DV_DY --------
            dv_dy = (gridCell->vAtual - gridCell->southFaceV) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->southFaceMi);
            miFace = (gridCell->southFaceMi);

        break;

                  // ---------------------------------------------------------
        case 'e': // -------------- FACE LESTE -------------------------------         
                        
                        // ----- vizinho NORTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellNorth->level > gridCell->level )
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o da direita, entre os do norte
        	       cellNorth = cellNorth->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {
    		      cellNorth = cellNorth->singleConnector;
   		       }
                           // ----- vizinho SUL ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellSouth->level > gridCell->level )
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {   // DC1 e' o da direita, entre os do sul
        	       cellSouth = cellSouth->doubleConnector1;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {
    		      cellSouth = cellSouth->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
                cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
                cellSouth = gridCell; // Isto denota que cellSouth e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellNorth e cellSouth invalidos )
            if( (cellNorth->type == 'b') && (cellNorth->active == true) )
            {
                if( cellNorth->centerX + cellNorth->halfFaceLength !=
                    gridCell->centerX + gridCell->halfFaceLength )
                    cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            }

            if( (cellSouth->type == 'b') && (cellSouth->active == true) )
            {
                if( cellSouth->centerX + cellSouth->halfFaceLength !=
                    gridCell->centerX + gridCell->halfFaceLength )
                    cellSouth = gridCell; // Isto denota que cellSouth e' invalido
            }

          // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
            deltaY =  
              ( 
                ( cellNorth == gridCell )? 
                    0 
                    : 
                  (cellNorth->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellSouth == gridCell )? 
                    0 
                    : 
                  (cellSouth->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DV_DY --------
            // Caso em que nem North nem South sao invalidos
            if( cellNorth != gridCell && cellSouth != gridCell )
            {
                // Diferencas centradas
                dv_dy = (cellNorth->eastFaceV - cellSouth->eastFaceV) 
                                / deltaY;
                // Diferencas pra tras
                //dv_dy = (gridCell->eastFaceV - cellSouth->eastFaceV) 
                //                / deltaY;
            }
            // Divisao de casos considerando North ou South invalidos
            if( cellNorth == gridCell )
            {
                dv_dy = (gridCell->eastFaceV - cellSouth->eastFaceV) 
                                / deltaY;
            }
            if( cellSouth == gridCell )
            {
                dv_dy = (cellNorth->eastFaceV - gridCell->eastFaceV) 
                                / deltaY;
            }
            
            // --------- CALCULANDO DU_DX --------
            du_dx = (gridCell->eastFaceU - gridCell->uAtual) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->eastFaceMi);
            miFace = (gridCell->eastFaceMi);

        break;

                  // ---------------------------------------------------------
        case 'w': // -------------- FACE OESTE -------------------------------         
                        
                        // ----- vizinho NORTE ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellNorth->level > gridCell->level )
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o da esquerda, entre os do norte
        	       cellNorth = cellNorth->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellNorth->type == 'w' ) && 
                    ( cellNorth->singleConnector != 0 ) 
               )
    	       {
    		      cellNorth = cellNorth->singleConnector;
   		       }
                           // ----- vizinho SUL ------
            // Buscar vizinho preto quando ele e' menor que a celula atual
            if ( cellSouth->level > gridCell->level )
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {   // DC2 e' o da esquerda, entre os do sul
        	       cellSouth = cellSouth->doubleConnector2;
    	       }
            // Buscar vizinho preto quando ele e' maior que a celula atual
            else 
    	       while( ( cellSouth->type == 'w' ) && 
                    ( cellSouth->singleConnector != 0 ) 
               )
    	       {
    		      cellSouth = cellSouth->singleConnector;
   		       }

            // Verifica se algum desses vizinhos e' fronteira
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
                cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            
            // Verifica se algum desses vizinhos e' fronteira
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
                cellSouth = gridCell; // Isto denota que cellSouth e' invalido

            // Verifica se algum desses vizinhos nao compartilha face, isso 
            // ocorre algumas vezes quando o vizinho e' maior que o atual
            // ( pode ser provado que nunca havera' simultaneamente
            //   cellNorth e cellSouth invalidos )
            if( (cellNorth->type == 'b') && (cellNorth->active == true) )
            {
                if( cellNorth->centerX - cellNorth->halfFaceLength !=
                    gridCell->centerX - gridCell->halfFaceLength )
                    cellNorth = gridCell; // Isto denota que cellNorth e' invalido
            }

            if( (cellSouth->type == 'b') && (cellSouth->active == true) )
            {
                if( cellSouth->centerX - cellSouth->halfFaceLength !=
                    gridCell->centerX - gridCell->halfFaceLength )
                    cellSouth = gridCell; // Isto denota que cellSouth e' invalido
            }

          // Delta y total entre extremos (norte--sul ou atual--sul ou norte--atual)
            deltaY =  
              ( 
                ( cellNorth == gridCell )? 
                    0 
                    : 
                  (cellNorth->halfFaceLength + gridCell->halfFaceLength)
              )   
             + 
              ( 
                ( cellSouth == gridCell )? 
                    0 
                    : 
                  (cellSouth->halfFaceLength + gridCell->halfFaceLength)
              );

            // --------- CALCULANDO DV_DY --------
            // Caso em que nem North nem South sao invalidos
            if( cellNorth != gridCell && cellSouth != gridCell )
            {
                // Diferencas centradas
                dv_dy = (cellNorth->westFaceV - cellSouth->westFaceV) 
                                / deltaY;
                // Diferencas pra tras
                //dv_dy = (gridCell->westFaceV - cellSouth->westFaceV) 
                //                / deltaY;
            }
            // Divisao de casos considerando North ou South invalidos
            if( cellNorth == gridCell )
            {
                dv_dy = (gridCell->westFaceV - cellSouth->westFaceV) 
                                / deltaY;
            }
            if( cellSouth == gridCell )
            {
                dv_dy = (cellNorth->westFaceV - gridCell->westFaceV) 
                                / deltaY;
            }
            
            // --------- CALCULANDO DU_DX --------
            du_dx = (gridCell->uAtual - gridCell->westFaceU) 
                                / (gridCell->halfFaceLength);

            // Lambda e Mi
            lambdaFace = -(2.0/3.0)*(gridCell->westFaceMi);
            miFace = (gridCell->westFaceMi);

        break;

        default: break;
    }
    
    // Retorna TauYY na face solicitada                                 
    return lambdaFace*(du_dx + dv_dy) + 2.0*miFace*dv_dy;  
}    


// -------------------------------- qX ---------------------------------------
double qX( Cell *gridCell, char face ) 
{
    double dT_dx;
    double kFace;

    switch( face )
    {   
                  // ---------------------------------------------------------
        case 'e': // -------------- FACE LESTE -------------------------------         
                                   
            // --------- CALCULANDO DT_DX --------
            dT_dx = (gridCell->eastFaceT - gridCell->TIterAnterior) 
                                / (gridCell->halfFaceLength);

            // k na face
            kFace = (gridCell->eastFaceK);

        break;

                  // ---------------------------------------------------------
        case 'w': // -------------- FACE OESTE -------------------------------         
                    
            // --------- CALCULANDO DT_DX --------
            dT_dx = (gridCell->TIterAnterior - gridCell->westFaceT) 
                                / (gridCell->halfFaceLength);

            // k na face
            kFace = (gridCell->westFaceK);

        break;

        default: break;
    }
    
    // Retorna qX na face solicitada                                    
    return -kFace*dT_dx;  
}


// --------------------------------- qY ---------------------------------------
double qY( Cell *gridCell, char face )         
{
    double dT_dy;
    double kFace;

    switch( face )
    {   
                  // ---------------------------------------------------------
        case 'n': // -------------- FACE NORTE -------------------------------         
                                   
            // --------- CALCULANDO DT_DX --------
            dT_dy = (gridCell->northFaceT - gridCell->TIterAnterior) 
                                / (gridCell->halfFaceLength);

            // k na face
            kFace = (gridCell->northFaceK);

        break;

                  // ---------------------------------------------------------
        case 's': // -------------- FACE SUL -------------------------------         
                    
            // --------- CALCULANDO DT_DX --------
            dT_dy = (gridCell->TIterAnterior - gridCell->southFaceT) 
                                / (gridCell->halfFaceLength);

            // k na face
            kFace = (gridCell->southFaceK);

        break;

        default: break;
    }
    
    // Retorna qY na face solicitada                                    
    return -kFace*dT_dy; 
}
