/*  ========== solve.cpp =========
    Contem funcoes para resolver as equacoes
    de Navier-Stokes
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>
#include <iostream>
#include <stdio.h>

    using std::cout;
    using std::endl;
    using std::ios;
    using std::cin;

#include "constants.h"

/*______________________________________________________________________________

                              solveSuperSonic()

           Resolver por iteracao de Picard o escoamento supersonico sobre
    a placa plana.
______________________________________________________________________________*/
void solveSuperSonic( Grid *grid )
{   
    // Variaveis
    bool convergiu;
    int iteracoes = 0;

    // Incrementa tempo
    grid->tempoAtual += grid->deltaT;

    //Avancar um passo no tempo das variaveis de todas as celulas
    Cell *gridCell;
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
        //Avancar um passo
        gridCell->rhoAnterior = gridCell->rhoAtual;
        gridCell->uAnterior = gridCell->uAtual;
        gridCell->vAnterior = gridCell->vAtual;
        gridCell->pAnterior = gridCell->pAtual;
        gridCell->TAnterior = gridCell->TAtual;
        gridCell->miAnterior = gridCell->miAtual;
        gridCell->kAnterior = gridCell->kAtual;
        gridCell->eAnterior = gridCell->eAtual;
    }
	
	// ITERACAO DE PICARD
    do{
        Cell *gridCell;
        // Percorre todas as celulas
        for(gridCell = grid->firstGridCell; 
            gridCell != 0; 
            gridCell = gridCell->next)
        {
           // Variaveis temporarias, que serao utilizadas na decodificacao
           double rhoU, rhoV, Et;
                         
           //Avancar um passo na iteracao
           gridCell->rhoIterAnterior = gridCell->rhoAtual;
           gridCell->uIterAnterior = gridCell->uAtual;
           gridCell->vIterAnterior = gridCell->vAtual;
           gridCell->pIterAnterior = gridCell->pAtual;
           gridCell->TIterAnterior = gridCell->TAtual;
           gridCell->miIterAnterior = gridCell->miAtual;
           gridCell->kIterAnterior = gridCell->kAtual;
           gridCell->eIterAnterior = gridCell->eAtual;

           // Ajuste das variaveis de cada celula em relacao aos seus vizinhos
           setGridFaces( grid );
              
    //resolve 1a coordenada, e obtem densidade --------------------------------
           gridCell->rhoAtual = ((grid->deltaT) / (gridCell->faceLength)) *
               (
                  (gridCell->westFaceRho)*(gridCell->westFaceU)
                 -(gridCell->eastFaceRho)*(gridCell->eastFaceU)
                 +(gridCell->southFaceRho)*(gridCell->southFaceV)
                 -(gridCell->northFaceRho)*(gridCell->northFaceV) 
               )
               + gridCell->rhoAnterior;
             
    //resolve 2a coordenada ---------------------------------------------------
           rhoU = ((grid->deltaT) / (gridCell->faceLength)) *
               (
                  (gridCell->westFaceRho)*(gridCell->westFaceU)*(gridCell->westFaceU)
                 +(gridCell->westFaceP)
                 -( faceTauXX(gridCell, 'w') )

                 -(
                      (gridCell->eastFaceRho)*(gridCell->eastFaceU)*(gridCell->eastFaceU)    
                     +(gridCell->eastFaceP)
                     -( faceTauXX(gridCell, 'e') )
                  )

                 +(gridCell->southFaceRho)*(gridCell->southFaceU)*(gridCell->southFaceV)
                 -( faceTauXY(gridCell, 's') )

                 -(
                      (gridCell->northFaceRho)*(gridCell->northFaceU)*(gridCell->northFaceV)
                     -( faceTauXY(gridCell, 'n') )
                  )
               )
               + (gridCell->rhoAnterior)*(gridCell->uAnterior);
		   // decodifica: u
           gridCell->uAtual = rhoU / (gridCell->rhoAtual);
              
    //resolve 3a coordenada ---------------------------------------------------
           rhoV = ((grid->deltaT) / (gridCell->faceLength)) *
               (
                 (gridCell->westFaceRho)*(gridCell->westFaceU)*(gridCell->westFaceV)   
                -( faceTauXY(gridCell, 'w') )

                -(
                     (gridCell->eastFaceRho)*(gridCell->eastFaceU)*(gridCell->eastFaceV)     
                    -( faceTauXY(gridCell, 'e') )
                 )

                +(gridCell->southFaceRho)*(gridCell->southFaceV)*(gridCell->southFaceV) 
                +(gridCell->southFaceP) 
                -( faceTauXY(gridCell, 's') )

                -(
                     (gridCell->northFaceRho)*(gridCell->northFaceV)*(gridCell->northFaceV) 
                    +(gridCell->northFaceP) 
                    -( faceTauXY(gridCell, 'n') )
                 )
               )
               + (gridCell->rhoAnterior)*(gridCell->vAnterior);
		   // decodifica: v
           gridCell->vAtual = rhoV / (gridCell->rhoAtual);	
                 
    //resolve 4a coordenada --------------------------------------------------
           Et = ((grid->deltaT) / (gridCell->faceLength)) *
            (        // face W
                 (gridCell->westFaceRho)*(gridCell->westFaceU)*
                 (   gridCell->westFaceE + 
                      (  (gridCell->westFaceU)*(gridCell->westFaceU)
                            +  
                        (gridCell->westFaceV)*(gridCell->westFaceV) ) / 2.0
                 )
                +(gridCell->westFaceP)*(gridCell->westFaceU)
                -(gridCell->westFaceU)*( faceTauXX(gridCell, 'w') )
                -(gridCell->westFaceV)*( faceTauXY(gridCell, 'w') )
                +( qX(gridCell, 'w') )

              -(    // face E
                 (gridCell->eastFaceRho)*(gridCell->eastFaceU)*
                 (   gridCell->eastFaceE + 
                      (  (gridCell->eastFaceU)*(gridCell->eastFaceU)
                            +  
                        (gridCell->eastFaceV)*(gridCell->eastFaceV) ) / 2.0
                 )
                +(gridCell->eastFaceP)*(gridCell->eastFaceU)
                -(gridCell->eastFaceU)*( faceTauXX(gridCell, 'e') )
                -(gridCell->eastFaceV)*( faceTauXY(gridCell, 'e') )
                +( qX(gridCell, 'e') )
               )
                       // face s
                +(gridCell->southFaceRho)*(gridCell->southFaceV)*
                 (   gridCell->southFaceE + 
                      (  (gridCell->southFaceU)*(gridCell->southFaceU)
                            +  
                        (gridCell->southFaceV)*(gridCell->southFaceV) ) / 2.0
                 )
                +(gridCell->southFaceP)*(gridCell->southFaceV)
                -(gridCell->southFaceU)*( faceTauXY(gridCell, 's') )
                -(gridCell->southFaceV)*( faceTauYY(gridCell, 's') )
                +( qY(gridCell, 's') )

             -(            // face n
                 (gridCell->northFaceRho)*(gridCell->northFaceV)*
                 (   gridCell->northFaceE + 
                      (  (gridCell->northFaceU)*(gridCell->northFaceU)
                            +  
                        (gridCell->northFaceV)*(gridCell->northFaceV) ) / 2.0
                 )
                +(gridCell->northFaceP)*(gridCell->northFaceV)
                -(gridCell->northFaceU)*( faceTauXY(gridCell, 'n') )
                -(gridCell->northFaceV)*( faceTauYY(gridCell, 'n') )
                +( qY(gridCell, 'n') )
              )                
            )
            + (gridCell->rhoAnterior)
                 *(
                      (gridCell->eAnterior) 
                            +
                      (
                          (gridCell->uAnterior)   
                         *(gridCell->uAnterior)
                                  +
                          (gridCell->vAnterior)
                         *(gridCell->vAnterior)
                      )/2.0
                  ); 
                
		   // decodifica: e
           gridCell->eAtual = (Et)/(gridCell->rhoAtual)
                            - (
                                 (gridCell->uAtual)   
                                *(gridCell->uAtual)
                                    +
                                 (gridCell->vAtual)
                                *(gridCell->vAtual)
                              )/2.0;
              
           // decodifica: T
           gridCell->TAtual = (gridCell->eAtual)
                                    /
                               ( R/(GAMMA-1.0) );

           // decodifica: p 
           gridCell->pAtual = R*(gridCell->rhoAtual)*(gridCell->TAtual);

           // decodifica: mi
           gridCell->miAtual = MI_ZERO*(pow(
                                      (gridCell->TAtual) / TEMPERATURA_LIVRE,
                                      (3.0/2.0)
                                    )
                                )*(TEMPERATURA_LIVRE + 110.0)/
                                  ((gridCell->TAtual) + 110.0);

           // decodifica: k
           gridCell->kAtual = ( (gridCell->miAtual) * GAMMA * R ) 
                                / 
                              ( (GAMMA-1.0) * PRANDTL );

           // Calcula numero de Reynolds no eixo x
           gridCell->ReynoldsX = (gridCell->rhoAtual)*
                                 (gridCell->uAtual)*(gridCell->faceLength)/
                                 (gridCell->miAtual);                  
        /**/
       }

       iteracoes++;
                     
	   // repetir se o erro nao for suficientemente pequeno
       convergiu = solutionHasConverged( grid, grid->erroPicard );

    // Refinar e desrefinar a malha toda
    refineGrid( grid, grid->refinamentoMaximo, 5.0 );
    derefineGrid( grid, 0.5, false );             

    }while( !convergiu ); 

    cout << " Tempo atual: " << grid->tempoAtual << " s,";
    cout << " Total de passos na iteracao de Picard: " << iteracoes << "\n";	
}


/*______________________________________________________________________________

                              initialCondition()

           Configura as variaveis na condicao inicial, ou seja, em t = 0.
______________________________________________________________________________*/
void initialCondition( Grid *grid)        
{
    Cell *gridCell;

    // Ajusta tempo para zero
    grid->tempoAtual = 0.0;

    // TESTE
    /*  for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell = gridCell->next)
    {
        if( gridCell->gridPosition == 42 )
        {
            Cell *auxiliarGridCell = gridCell;
            //Ter a referencia para a proxima celula na curva de Hilbert
            gridCell = gridCell->next;
            // Refinar
            refineCell( auxiliarGridCell );
            grid->orderGridCells();
            grid->numberOfCells += 3;
        }               
    }   */
    
    // Ajustar todas as celulas com a configuracao inicial
    for(gridCell = grid->firstGridCell; 
        gridCell != 0; 
        gridCell=gridCell->next)
    {
	   // Variaveis de fluido
	   gridCell->rhoAtual = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
	   gridCell->rhoAnterior = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
	   gridCell->rhoIterAnterior = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE); 

	   gridCell->uAtual = 4.0*(VEL_SOM); // mach 4
	   gridCell->uAnterior =  4.0*(VEL_SOM); // mach 4
	   gridCell->uIterAnterior =  4.0*(VEL_SOM); // mach 4

	   gridCell->vAtual = 0.0; 
	   gridCell->vAnterior = 0.0; 
	   gridCell->vIterAnterior = 0.0;

	   gridCell->eAtual = R*TEMPERATURA_LIVRE/(GAMMA-1.0); // e = Cv * T
	   gridCell->eAnterior = R*TEMPERATURA_LIVRE/(GAMMA-1.0); // e = Cv * T 
	   gridCell->eIterAnterior = R*TEMPERATURA_LIVRE/(GAMMA-1.0); // e = Cv * T
                          
	   gridCell->TAtual = TEMPERATURA_LIVRE;
	   gridCell->TAnterior = TEMPERATURA_LIVRE;
	   gridCell->TIterAnterior = TEMPERATURA_LIVRE;
	
	   gridCell->pAtual = PRESSAO_LIVRE;
	   gridCell->pAnterior = PRESSAO_LIVRE;
	   gridCell->pIterAnterior = PRESSAO_LIVRE;

	   gridCell->miAtual = MI_ZERO;
	   gridCell->miAnterior = MI_ZERO;
	   gridCell->miIterAnterior = MI_ZERO;

	   gridCell->kAtual = (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL);
	   gridCell->kAnterior = (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL);
	   gridCell->kIterAnterior = (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL);
    }

    // Arrumar as variaveis das faces de cada celula
    setGridFaces( grid );
}


/*______________________________________________________________________________
                            
                   FUNCTIONs set<Direction>BoundaryCellVariables
                            
    Set directional physical variables for cells belonging to the four boundaries 
    of the rectangular domain, i.e., apply the boundary conditions to those cells.
     
______________________________________________________________________________*/
void setSouthBoundaryCellVariables( Cell *gridCell )
{
    gridCell->southFaceP = 2.0*(gridCell->pIterAnterior) 
                           - (gridCell->northFaceP); // extrapolacao linear 
    gridCell->southFaceRho = (gridCell->southFaceP)/(R*TEMPERATURA_PAREDE);
    gridCell->southFaceU = 0.0;    
    gridCell->southFaceV = 0.0;
    gridCell->southFaceE =  R*TEMPERATURA_LIVRE/(GAMMA-1.0); // e = Cv * T
    gridCell->southFaceT = TEMPERATURA_PAREDE;
    gridCell->southFaceMi = MI_ZERO* (pow(
                                  (gridCell->southFaceT) / TEMPERATURA_LIVRE,
                                  (3.0/2.0)
                                )
                            )*(TEMPERATURA_LIVRE + 110.0)/
                              ((gridCell->southFaceT) + 110.0);
    gridCell->southFaceK = ( (gridCell->southFaceMi) * GAMMA * R ) /
                       ( (GAMMA-1.0) * PRANDTL );    
}

void setNorthBoundaryCellVariables( Cell *gridCell )
{
    // Ajustar variaveis do vizinho do norte com condicoes de fronteira
    gridCell->northFaceP = PRESSAO_LIVRE;
    gridCell->northFaceRho = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
    gridCell->northFaceU =  4.0*(VEL_SOM);    
    gridCell->northFaceV = 0.0;
    gridCell->northFaceE = R*TEMPERATURA_PAREDE/(GAMMA-1.0);
    gridCell->northFaceT = TEMPERATURA_LIVRE;
    gridCell->northFaceMi = MI_ZERO;
    gridCell->northFaceK = ( MI_ZERO * GAMMA * R ) / 
                       ( (GAMMA - 1.0) * PRANDTL );
}

//East boundary of domain corresponds to resonator wall.
void setEastBoundaryCellVariables( Cell *gridCell )
{
    // Ajustar variaveis do vizinho do leste com condicoes de fronteira
    gridCell->eastFaceU =  2.0*(gridCell->uIterAnterior) 
                            - (gridCell->westFaceU); // extrapolacao linear 
    gridCell->eastFaceV =  2.0*(gridCell->vIterAnterior) 
                            - (gridCell->westFaceV); // extrapolacao linear 
    gridCell->eastFaceP =  2.0*(gridCell->pIterAnterior) 
                            - (gridCell->westFaceP); // extrapolacao linear
    gridCell->eastFaceT =  2.0*(gridCell->TIterAnterior) 
                            - (gridCell->westFaceT); // extrapolacao linear     
    gridCell->eastFaceRho = (gridCell->eastFaceP)/(R*(gridCell->eastFaceT));   
    gridCell->eastFaceE = R*(gridCell->eastFaceT)/(GAMMA-1.0);   
    gridCell->eastFaceMi = MI_ZERO* (pow(
                                      (gridCell->eastFaceT) / TEMPERATURA_LIVRE,
                                      (3.0/2.0)
                                    )
                                )*(TEMPERATURA_LIVRE + 110.0)/
                                  ((gridCell->eastFaceT) + 110.0);
    gridCell->eastFaceK = ( (gridCell->eastFaceMi) * GAMMA * R ) / 
                      ( (GAMMA-1.0) * PRANDTL );
}

//West boundary of domain corresponds to acoustic source.
void setWestBoundaryCellVariables( Cell *gridCell )
{           
    // Ajustar variaveis do vizinho do oeste com condicoes de fronteira
    gridCell->westFaceP = PRESSAO_LIVRE;
    gridCell->westFaceRho = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
    gridCell->westFaceU =  4.0*(VEL_SOM);    
    gridCell->westFaceV = 0.0;
    gridCell->westFaceE = R*TEMPERATURA_PAREDE/(GAMMA-1.0);
    gridCell->westFaceT = TEMPERATURA_LIVRE;
    gridCell->westFaceMi = MI_ZERO;
    gridCell->westFaceK = (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL);
}

/*______________________________________________________________________________
                            
                            FUNCTION solutionHasConverged()
                            
    Verifies if the solution of the supersonic problem has converged, 
    according to the formula
                        2 * |a-b| < error * ||a|+|b||,
    where the error margin "error" is set by the user.
______________________________________________________________________________*/
bool solutionHasConverged( Grid *grid, double error )
{
    Cell *gridCell;
    bool converges = true;
    double epsilon = error/2;
    double densityDifference,
           densitySum,
           pressureDifference,
           pressureSum,
           viscosityDifference,
           viscositySum,
           conductivityDifference,
           conductivitySum,
           velocityXDifference,
           velocityXSum,
           velocityYDifference,
           velocityYSum,
           temperatureDifference,
           temperatureSum,
           energyDifference,
           energySum;
    
    gridCell = grid->firstGridCell;    
    while( gridCell != 0 && converges == true )
    {
        if( gridCell->active )
        {
            densityDifference = fabs( gridCell->rhoIterAnterior 
                                      - gridCell->rhoAtual );
            densitySum = epsilon * ( fabs(gridCell->rhoIterAnterior) 
                                         + fabs(gridCell->rhoAtual) );
            
            pressureDifference = fabs( gridCell->pIterAnterior 
                                       - gridCell->pAtual );
            pressureSum = epsilon * ( fabs(gridCell->pIterAnterior) 
                                          + fabs(gridCell->pAtual) );
            
            viscosityDifference = fabs( gridCell->miIterAnterior 
                                        - gridCell->miAtual );
            viscositySum = epsilon * ( fabs(gridCell->miIterAnterior) 
                                           + fabs(gridCell->miAtual) );
            
            conductivityDifference = fabs( gridCell->kIterAnterior 
                                           - gridCell->kAtual );
            conductivitySum = epsilon * ( fabs(gridCell->kIterAnterior) 
                                              + fabs(gridCell->kAtual) );
            
            velocityXDifference = fabs( gridCell->uIterAnterior 
                                        - gridCell->uAtual );
            velocityXSum = epsilon * ( fabs(gridCell->uIterAnterior) 
                                           + fabs(gridCell->uAtual) );
            
            velocityYDifference = fabs( gridCell->vIterAnterior 
                                        - gridCell->vAtual );
            velocityYSum = epsilon * ( fabs(gridCell->vIterAnterior)
                                           + fabs(gridCell->vAtual) );
            
            temperatureDifference = fabs( gridCell->TIterAnterior 
                                          - gridCell->TAtual );
            temperatureSum = epsilon * ( fabs(gridCell->TIterAnterior) 
                                             + fabs(gridCell->TAtual) );

            energyDifference = fabs( gridCell->eIterAnterior 
                                           - gridCell->eAtual );
            energySum = epsilon * ( fabs(gridCell->eIterAnterior) 
                                              + fabs(gridCell->eAtual) );
            
            if (  
               ( (densityDifference > densitySum) && (densityDifference > error) )
            || ( (pressureDifference > pressureSum) && (pressureDifference > error) )
            || ( (viscosityDifference > viscositySum) && (viscosityDifference > error) )
            || ( (conductivityDifference > conductivitySum) && (conductivityDifference > error) )
            || ( (velocityXDifference > velocityXSum) && (velocityXDifference > error) )
            || ( (velocityYDifference > velocityYSum) && (velocityYDifference > error) )
            || ( (temperatureDifference > temperatureSum) && (temperatureDifference > error) )
            )
                converges = false;
        }
        gridCell = gridCell->next;
    }
    
    return converges;
}
