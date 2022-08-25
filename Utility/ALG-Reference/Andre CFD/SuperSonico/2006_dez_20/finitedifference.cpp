 /*  ========== finitedifference.cpp =========
    Contem funcoes que resolvem as tensoes XX, XY, YY, e fluxo de calor QX, QY
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>
#include <iostream>

#include "constants.h"

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
            dv_dy = (gridCell->northFaceV - gridCell->vIterAnterior) 
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
            dv_dy = (gridCell->vIterAnterior - gridCell->southFaceV) 
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
            du_dx = (gridCell->eastFaceU - gridCell->uIterAnterior) 
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
            du_dx = (gridCell->uIterAnterior - gridCell->westFaceU) 
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
            du_dy = (gridCell->northFaceU - gridCell->uIterAnterior) 
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
            du_dy = (gridCell->uIterAnterior - gridCell->southFaceU) 
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
            dv_dx = (gridCell->eastFaceV - gridCell->vIterAnterior) 
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
            dv_dx = (gridCell->vIterAnterior - gridCell->westFaceV) 
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
            dv_dy = (gridCell->northFaceV - gridCell->vIterAnterior) 
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
            dv_dy = (gridCell->vIterAnterior - gridCell->southFaceV) 
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
            du_dx = (gridCell->eastFaceU - gridCell->uIterAnterior) 
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
            du_dx = (gridCell->uIterAnterior - gridCell->westFaceU) 
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
