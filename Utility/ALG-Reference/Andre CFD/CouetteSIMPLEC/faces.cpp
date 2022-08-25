//flux.cpp

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>
#include <float.h>

#include <iostream>

/*______________________________________________________________________________

								FUNCTION setGridFaces()
								
    Traverses the grid, computing the variables of each cell due to all 
    neighboring faces in order to compute the value of that variable on that
    face.
    In order to do this, calls function setCellFaceVariable().	
	
______________________________________________________________________________*/
void setGridFaces( Grid *grid )
{
    Cell *gridCell,
         *neighborGridCell;
    char direction;  
    
    // Percorrer todas as celulas e definir suas faces com valores nulos
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {         
        gridCell->northFaceRho = 0.0;
        gridCell->southFaceRho = 0.0;
        gridCell->eastFaceRho  = 0.0;
        gridCell->westFaceRho  = 0.0;

        gridCell->northFaceU = 0.0;
        gridCell->southFaceU = 0.0;
        gridCell->eastFaceU  = 0.0;
        gridCell->westFaceU  = 0.0;

        gridCell->northFaceV = 0.0;
        gridCell->southFaceV = 0.0;
        gridCell->eastFaceV  = 0.0;
        gridCell->westFaceV  = 0.0;

        gridCell->northFaceEt = 0.0;
        gridCell->southFaceEt = 0.0;
        gridCell->eastFaceEt  = 0.0;
        gridCell->westFaceEt  = 0.0;

        gridCell->northFaceT = 0.0;
        gridCell->southFaceT = 0.0;
        gridCell->eastFaceT  = 0.0;
        gridCell->westFaceT  = 0.0;

        gridCell->northFaceP = 0.0;
        gridCell->southFaceP = 0.0;
        gridCell->eastFaceP  = 0.0;
        gridCell->westFaceP  = 0.0;

        gridCell->northFaceMi = 0.0;
        gridCell->southFaceMi = 0.0;
        gridCell->eastFaceMi  = 0.0;
        gridCell->westFaceMi  = 0.0;
    
        gridCell->northFaceK = 0.0;
        gridCell->southFaceK = 0.0;
        gridCell->eastFaceK  = 0.0;
        gridCell->westFaceK  = 0.0;
    
        gridCell = gridCell->next;
    }        
         
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {       
       // Calcula as variaveis na face do norte
       neighborGridCell = gridCell->north;
       direction = 'n';
       setCellFaceVariable( gridCell, neighborGridCell, direction);
             
       // Calcula as variaveis na face do sul
       neighborGridCell = gridCell->south;
       direction = 's';
       setCellFaceVariable( gridCell, neighborGridCell, direction);
       
       // Calcula as variaveis na face do oeste
       neighborGridCell = gridCell->west;
       direction = 'w';
       setCellFaceVariable( gridCell, neighborGridCell, direction );

       // Calcula as variaveis na face do leste
       neighborGridCell = gridCell->east;
       direction = 'e';
       setCellFaceVariable( gridCell, neighborGridCell, direction);
                                             
       gridCell = gridCell->next;
    }
}


/*______________________________________________________________________________
    
                      FUNCTION visitaVizinhosCalculaSubFaces()
    
    Visita recursivamente vizinhos menores para calcular valores de variaveis
numa face de acordo com os valores dessas variaveis nas subfaces menores. Essa
funcao e' usada em setCellFaceVariable. 
______________________________________________________________________________*/

// Tipo necessario para retornar todos os valores das variaveis calculadas
struct TipoResultadoFaces 
{      
    double rho, u, v, e, T, p, mi, k;
};

// A funcao visitaVizinhosCalculaSubFaces ------------------------------------
struct TipoResultadoFaces 
visitaVizinhosCalculaSubFaces( Cell *gridCell, Cell *vertice )
{
    struct TipoResultadoFaces final;
 
    // Se vertice e' branco, retorna a media dos seus doubleConectors
    if( vertice->type == 'w' )
    {
        struct TipoResultadoFaces parte1, parte2;

        parte1 = visitaVizinhosCalculaSubFaces( gridCell, 
                                              vertice->doubleConnector1 );

        parte2 = visitaVizinhosCalculaSubFaces( gridCell, 
                                              vertice->doubleConnector2 );
  
        final.rho = (parte1.rho + parte2.rho) / 2.0;
        final.u   = (parte1.u + parte2.u)     / 2.0;
        final.v   = (parte1.v + parte2.v)     / 2.0;
        final.e   = (parte1.e + parte2.e)     / 2.0;
        final.T   = (parte1.T + parte2.T)     / 2.0;        
        final.p   = (parte1.p + parte2.p)     / 2.0;
        final.mi  = (parte1.mi + parte2.mi)   / 2.0;
        final.k   = (parte1.k + parte2.k)     / 2.0;

        return final;
    }

    // Se vertice e' preto, retorna a subface desse preto com a celula pai
    if( vertice->type == 'b' )
    {     
        double peso;
        int deltaRefinamento;

        // Buscar diferenca: ref(vertice) - ref(pai)
        // E computar peso para a media ponderada
        deltaRefinamento = vertice->level - gridCell->level;

        if(deltaRefinamento > 0){
            while(deltaRefinamento > 0){
                peso *= 2.0;
                deltaRefinamento--;    
            }
        }
            /////////////////////////////////////
        final.rho = ((gridCell->rhoIterAnterior) 
                                   +
                      peso*(vertice->rhoIterAnterior)
                     )
                     / (1.0 + peso);
            /////////////////////////////////////
        final.u = ((gridCell->uIterAnterior) 
                                   +
                      peso*(vertice->uIterAnterior)
                     )
                     / (1.0 + peso);
            /////////////////////////////////////
        final.v = ((gridCell->vIterAnterior) 
                                   +
                      peso*(vertice->vIterAnterior)
                     )
                     / (1.0 + peso);
            /////////////////////////////////////
        final.e = ((gridCell->eIterAnterior) 
                                   +
                      peso*(vertice->eIterAnterior)
                     )
                     / (1.0 + peso);
            /////////////////////////////////////
        final.T = ((gridCell->TIterAnterior) 
                                   +
                      peso*(vertice->TIterAnterior)
                     )
                     / (1.0 + peso);
            /////////////////////////////////////
        final.p = ((gridCell->pIterAnterior) 
                                   +
                      peso*(vertice->pIterAnterior)
                     )
                     / (1.0 + peso);
            /////////////////////////////////////
        final.mi = ((gridCell->miIterAnterior) 
                                   +
                      peso*(vertice->miIterAnterior)
                     )
                     / (1.0 + peso); 
            /////////////////////////////////////
        final.k = ((gridCell->kIterAnterior) 
                                   +
                      peso*(vertice->kIterAnterior)
                     )
                     / (1.0 + peso);       
    }

    return final;
}

/*______________________________________________________________________________
    
                            FUNCTION setCellFaceVariable()
    
    Computes values of variables at cell faces by linear interpolations.
 
______________________________________________________________________________*/
void setCellFaceVariable( Cell *gridCell, Cell *neighborGridCell, char direction )
{
    int deltaRefinamento = 0;  // Entre celula vizinho e celula atual
    double peso = 1.0; // Para a media ponderada na interpolacao

    // Se a face e' compartilhada com so' um vizinho, calcula diretamente a
    // variavel nesta face
    if( gridCell->level >= neighborGridCell->level )
    {          
        // Buscar no' preto ou no' de fronteira
        while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
    	    neighborGridCell = neighborGridCell->singleConnector;
   		}

        // Buscar diferenca: ref(vizinho) - ref(atual)
        // E computar peso para a media ponderada
        deltaRefinamento = neighborGridCell->level - gridCell->level;

        // No caso presente, deltaRef e' sempre inteiro negativo
        while( deltaRefinamento < 0 )
        {
            peso /= 2.0;
            deltaRefinamento++;
        }                        
                         
        // Calcula variaveis na face entre um vizinho preto
        if( neighborGridCell->type == 'b' )
        {
            switch( direction )
            { 
                case 'n': // -------- NORTE ------------------------
                    gridCell->northFaceRho = (
                          (gridCell->rhoIterAnterior) 
                                        +
                           peso*(neighborGridCell->rhoIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceU = (
                            (gridCell->uIterAnterior) 
                                         +
                            peso*(neighborGridCell->uIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceV = (
                            (gridCell->vIterAnterior) 
                                         +
                            peso*(neighborGridCell->vIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceE = (
                            (gridCell->eIterAnterior) 
                                         +
                            peso*(neighborGridCell->eIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceT = (
                            (gridCell->TIterAnterior) 
                                        +
                            peso*(neighborGridCell->TIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceP = (
                            (gridCell->pIterAnterior) 
                                         +
                            peso*(neighborGridCell->pIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceMi = (
                            (gridCell->miIterAnterior) 
                                         +
                            peso*(neighborGridCell->miIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->northFaceK = (
                            (gridCell->kIterAnterior) 
                                         +
                            peso*(neighborGridCell->kIterAnterior)
                        )
                        / (1.0 + peso); 
                break;

                case 's': // -------- SUL ------------------------
                    gridCell->southFaceRho = (
                            (gridCell->rhoIterAnterior) 
                                         +
                            peso*(neighborGridCell->rhoIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceU = (
                            (gridCell->uIterAnterior) 
                                         +
                            peso*(neighborGridCell->uIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceV = (
                            (gridCell->vIterAnterior) 
                                         +
                            peso*(neighborGridCell->vIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceE = (
                            (gridCell->eIterAnterior) 
                                         +
                            peso*(neighborGridCell->eIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceT = (
                            (gridCell->TIterAnterior) 
                                         +
                            peso*(neighborGridCell->TIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceP = (
                            (gridCell->pIterAnterior) 
                                         +
                            peso*(neighborGridCell->pIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceMi = (
                            (gridCell->miIterAnterior) 
                                         +
                            peso*(neighborGridCell->miIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->southFaceK = (
                            (gridCell->kIterAnterior) 
                                         +
                            peso*(neighborGridCell->kIterAnterior)
                        )
                        / (1.0 + peso); 
                break;

                case 'e': // -------- LESTE ------------------------
                    gridCell->eastFaceRho = (
                            (gridCell->rhoIterAnterior) 
                                         +
                            peso*(neighborGridCell->rhoIterAnterior)
                        )
                        / (1.0 + peso);                        
                      ////////////////////////////////////////////////
                    gridCell->eastFaceU = (
                            (gridCell->uIterAnterior) 
                                         +
                            peso*(neighborGridCell->uIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->eastFaceV = (
                            (gridCell->vIterAnterior) 
                                         +
                            peso*(neighborGridCell->vIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->eastFaceE = (
                            (gridCell->eIterAnterior) 
                                         +
                            peso*(neighborGridCell->eIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->eastFaceT = (
                            (gridCell->TIterAnterior) 
                                       +
                            peso*(neighborGridCell->TIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->eastFaceP = (
                            (gridCell->pIterAnterior) 
                                         +
                            peso*(neighborGridCell->pIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->eastFaceMi = (
                            (gridCell->miIterAnterior) 
                                         +
                            peso*(neighborGridCell->miIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->eastFaceK = (
                            (gridCell->kIterAnterior) 
                                        +
                            peso*(neighborGridCell->kIterAnterior)
                        )
                        / (1.0 + peso); 
                break;

                case 'w': // -------- OESTE ------------------------
                    gridCell->westFaceRho = (
                            (gridCell->rhoIterAnterior) 
                                     +
                           peso*(neighborGridCell->rhoIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceU = (
                            (gridCell->uIterAnterior) 
                                         +
                            peso*(neighborGridCell->uIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceV = (
                            (gridCell->vIterAnterior) 
                                         +
                            peso*(neighborGridCell->vIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceE = (
                            (gridCell->eIterAnterior) 
                                       +
                            peso*(neighborGridCell->eIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceT = (
                            (gridCell->TIterAnterior) 
                                         +
                            peso*(neighborGridCell->TIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceP = (
                            (gridCell->pIterAnterior) 
                                         +
                            peso*(neighborGridCell->pIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceMi = (
                            (gridCell->miIterAnterior) 
                                         +
                            peso*(neighborGridCell->miIterAnterior)
                        )
                        / (1.0 + peso);
                      ////////////////////////////////////////////////
                    gridCell->westFaceK = (
                            (gridCell->kIterAnterior) 
                                        +
                            peso*(neighborGridCell->kIterAnterior)
                        )
                        / (1.0 + peso); 
                break;

                default: break;
            }
        }
      
        // Calcula variaveis na face entre um vizinho branco (fronteira)
        if( neighborGridCell->type == 'w' )
        {
            switch( direction )
            {
                case 'n':
                    setNorthBoundaryCellVariables( gridCell );
                break;

                case 's':
                    setSouthBoundaryCellVariables( gridCell );
                break;

                case 'e':
                    setEastBoundaryCellVariables( gridCell );
                break;

                case 'w':
                    setWestBoundaryCellVariables( gridCell );
                break;

                default: break;
            }
        }                       
    }
          
    // Se a face e' compartilhada com mais de um vizinho, realiza chamadas
    // recursivas, fazendo medias aritmeticas equivalentes
    if( gridCell->level < neighborGridCell->level )
    {          
        struct TipoResultadoFaces resultadoFaces;
        // Calcula recursivamente
        resultadoFaces = visitaVizinhosCalculaSubFaces( gridCell, 
                                                        neighborGridCell );
        
        switch( direction )
        {                
            case 'n':
                gridCell->northFaceRho = resultadoFaces.rho;
                gridCell->northFaceU   = resultadoFaces.u;
                gridCell->northFaceV   = resultadoFaces.v;
                gridCell->northFaceE   = resultadoFaces.e;
                gridCell->northFaceT   = resultadoFaces.T;
                gridCell->northFaceP   = resultadoFaces.p;
                gridCell->northFaceMi  = resultadoFaces.mi;
                gridCell->northFaceK   = resultadoFaces.k;    
            break;

            case 's':
                gridCell->southFaceRho = resultadoFaces.rho;
                gridCell->southFaceU   = resultadoFaces.u;
                gridCell->southFaceV   = resultadoFaces.v;
                gridCell->southFaceE   = resultadoFaces.e;
                gridCell->southFaceT   = resultadoFaces.T;
                gridCell->southFaceP   = resultadoFaces.p;
                gridCell->southFaceMi  = resultadoFaces.mi;
                gridCell->southFaceK   = resultadoFaces.k;    
            break;

            case 'e':
                gridCell->eastFaceRho = resultadoFaces.rho;
                gridCell->eastFaceU   = resultadoFaces.u;
                gridCell->eastFaceV   = resultadoFaces.v;
                gridCell->eastFaceE   = resultadoFaces.e;
                gridCell->eastFaceT   = resultadoFaces.T;
                gridCell->eastFaceP   = resultadoFaces.p;
                gridCell->eastFaceMi  = resultadoFaces.mi;
                gridCell->eastFaceK   = resultadoFaces.k;    
            break;

            case 'w':
                gridCell->westFaceRho = resultadoFaces.rho;
                gridCell->westFaceU   = resultadoFaces.u;
                gridCell->westFaceV   = resultadoFaces.v;
                gridCell->westFaceE   = resultadoFaces.e;
                gridCell->westFaceT   = resultadoFaces.T;                    
                gridCell->westFaceP   = resultadoFaces.p;
                gridCell->westFaceMi  = resultadoFaces.mi;
                gridCell->westFaceK   = resultadoFaces.k;    
            break;
        
            default: break;
        }
    }
}
