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

    // Ajuste das variaveis de cada celula em relacao aos seus vizinhos
    setGridVariables( grid );
	
	// ITERACAO DE PICARD
    do{ 
        Cell *gridCell;
        // Percorre todas as celulas
        for(gridCell = grid->firstGridCell; 
            gridCell != 0; 
            gridCell = gridCell->next)
        {
         if( gridCell->active )
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
           
    //resolve 1a coordenada, e obtem densidade --------------------------------
           gridCell->rhoAtual = ((grid->deltaT) / (gridCell->faceLength)) *
               (
                  computeExpressionFlux(gridCell,gridCell->west,1,'w') 
                 -computeExpressionFlux(gridCell,gridCell->east,1,'e')
                 +computeExpressionFlux(gridCell,gridCell->south,5,'s')  
                 -computeExpressionFlux(gridCell,gridCell->north,5,'n')
               )
               + gridCell->rhoAnterior;

    //resolve 2a coordenada ---------------------------------------------------
           rhoU = ((grid->deltaT) / (gridCell->faceLength)) *
               (
                  computeExpressionFlux(gridCell,gridCell->west,2,'w')
                 +computeExpressionFlux(gridCell,gridCell->west,3,'w')
                 -computeExpressionFlux(gridCell,gridCell->west,4,'w') 
                 -(computeExpressionFlux(gridCell,gridCell->east,2,'e')
                   +computeExpressionFlux(gridCell,gridCell->east,3,'e')
                   -computeExpressionFlux(gridCell,gridCell->east,4,'e')
                  )
                 +computeExpressionFlux(gridCell,gridCell->south,6,'s')
                 -computeExpressionFlux(gridCell,gridCell->south,7,'s')  
                 -(computeExpressionFlux(gridCell,gridCell->north,6,'n')
                   -computeExpressionFlux(gridCell,gridCell->north,7,'n')
                  )
               )
               + (gridCell->rhoAnterior)*(gridCell->uAnterior);
		   // decodifica: u
           gridCell->uAtual = rhoU / (gridCell->rhoAtual);

    //resolve 3a coordenada ---------------------------------------------------
           rhoV = ((grid->deltaT) / (gridCell->faceLength)) *
               (
                  computeExpressionFlux(gridCell,gridCell->west,6,'w')
                 -computeExpressionFlux(gridCell,gridCell->west,7,'w') 
                 -(computeExpressionFlux(gridCell,gridCell->east,6,'e')
                   -computeExpressionFlux(gridCell,gridCell->east,7,'e')
                  )
                 +computeExpressionFlux(gridCell,gridCell->south,8,'s')
                 +computeExpressionFlux(gridCell,gridCell->south,3,'s')
                 -computeExpressionFlux(gridCell,gridCell->south,7,'s')  
                 -(computeExpressionFlux(gridCell,gridCell->north,8,'n')
                   +computeExpressionFlux(gridCell,gridCell->north,3,'n')
                   -computeExpressionFlux(gridCell,gridCell->north,7,'n')
                  )
               )
               + (gridCell->rhoAnterior)*(gridCell->vAnterior);
		   // decodifica: v
           gridCell->vAtual = rhoV / (gridCell->rhoAtual);	

    //resolve 4a coordenada --------------------------------------------------
           Et = ((grid->deltaT) / (gridCell->faceLength)) *
              (
                ( 
                   computeExpressionFlux(gridCell,gridCell->west,9,'w')
                  +computeExpressionFlux(gridCell,gridCell->west,10,'w')
                  -computeExpressionFlux(gridCell,gridCell->west,11,'w')
                  -computeExpressionFlux(gridCell,gridCell->west,12,'w')
                  +computeExpressionFlux(gridCell,gridCell->west,15,'w')
                )
               -(
                   computeExpressionFlux(gridCell,gridCell->east,9,'e')
                  +computeExpressionFlux(gridCell,gridCell->east,10,'e')
                  -computeExpressionFlux(gridCell,gridCell->east,11,'e')
                  -computeExpressionFlux(gridCell,gridCell->east,12,'e')
                  +computeExpressionFlux(gridCell,gridCell->east,15,'e')
                )
               +( 
                   computeExpressionFlux(gridCell,gridCell->south,17,'s')
                  +computeExpressionFlux(gridCell,gridCell->south,18,'s')
                  -computeExpressionFlux(gridCell,gridCell->south,13,'s')
                  -computeExpressionFlux(gridCell,gridCell->south,14,'s')
                  +computeExpressionFlux(gridCell,gridCell->south,16,'s')
                )
               -( 
                   computeExpressionFlux(gridCell,gridCell->north,17,'n')
                  +computeExpressionFlux(gridCell,gridCell->north,18,'n')
                  -computeExpressionFlux(gridCell,gridCell->north,13,'n')
                  -computeExpressionFlux(gridCell,gridCell->north,14,'n')
                  +computeExpressionFlux(gridCell,gridCell->north,16,'n')
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
         }
       }

       iteracoes++;

	   // repetir se o erro nao for suficientemente pequeno
       convergiu = solutionHasConverged( grid, grid->erroPicard );

    }while( !convergiu );

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

    // Ajustar todas as celulas com a configuracao inicial
    for(gridCell = grid->firstGridCell; gridCell != 0; gridCell=gridCell->next)
    {
        if( (gridCell->centerY)/(grid->sideLength) > 0.625 )
            gridCell->active = false;  
    }
    
    // Ajustar todas as celulas com a configuracao inicial
    for(gridCell = grid->firstGridCell; gridCell != 0; gridCell=gridCell->next)
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

    // Arrumar as variaveis de cada celula em relacao aos seus vizinhos
    setGridVariables( grid );
}


/*______________________________________________________________________________
                            
                   FUNCTIONs set<Direction>BoundaryCellVariables
                            
    Set directional physical variables for cells belonging to the four boundaries 
    of the rectangular domain, i.e., apply the boundary conditions to those cells.
     
______________________________________________________________________________*/
void setSouthBoundaryCellVariables( Grid *grid, Cell *gridCell )
{
    gridCell->southP = 2.0*(gridCell->pIterAnterior) - (gridCell->northP); // extrapolacao linear 
    gridCell->southRho = (gridCell->southP)/(R*TEMPERATURA_PAREDE);
    gridCell->southU = 0.0;    
    gridCell->southV = 0.0;
    gridCell->southE =  R*TEMPERATURA_LIVRE/(GAMMA-1.0); // e = Cv * T
    gridCell->southT = TEMPERATURA_PAREDE;
    gridCell->southMi = MI_ZERO* (pow(
                                  (gridCell->southT) / TEMPERATURA_LIVRE,
                                  (3.0/2.0)
                                )
                            )*(TEMPERATURA_LIVRE + 110.0)/
                              ((gridCell->southT) + 110.0);
    gridCell->southK = ( (gridCell->southMi) * GAMMA * R ) /
                       ( (GAMMA-1.0) * PRANDTL );    
}

void setNorthBoundaryCellVariables( Cell *gridCell )
{
    // Ajustar variaveis do vizinho do norte com condicoes de fronteira
    gridCell->northP = PRESSAO_LIVRE;
    gridCell->northRho = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
    gridCell->northU =  4.0*(VEL_SOM);    
    gridCell->northV = 0.0;
    gridCell->northE = R*TEMPERATURA_PAREDE/(GAMMA-1.0);
    gridCell->northT = TEMPERATURA_LIVRE;
    gridCell->northMi = MI_ZERO;
    gridCell->northK = ( MI_ZERO * GAMMA * R ) / 
                       ( (GAMMA - 1.0) * PRANDTL );
}

//East boundary of domain corresponds to resonator wall.
void setEastBoundaryCellVariables( Cell *gridCell )
{
    // Ajustar variaveis do vizinho do leste com condicoes de fronteira
    gridCell->eastU =  2.0*(gridCell->uIterAnterior) - (gridCell->westU); // extrapolacao linear 
    gridCell->eastV =  2.0*(gridCell->vIterAnterior) - (gridCell->westV); // extrapolacao linear 
    gridCell->eastP =  2.0*(gridCell->pIterAnterior) - (gridCell->westP); // extrapolacao linear 
    gridCell->eastT =  2.0*(gridCell->TIterAnterior) - (gridCell->westT); // extrapolacao linear     
    gridCell->eastRho = (gridCell->eastP)/(R*(gridCell->eastT));   
    gridCell->eastE = R*(gridCell->eastT)/(GAMMA-1.0);   
    gridCell->eastMi = MI_ZERO* (pow(
                                      (gridCell->eastT) / TEMPERATURA_LIVRE,
                                      (3.0/2.0)
                                    )
                                )*(TEMPERATURA_LIVRE + 110.0)/
                                  ((gridCell->eastT) + 110.0);
    gridCell->eastK = ( (gridCell->eastMi) * GAMMA * R ) / 
                      ( (GAMMA-1.0) * PRANDTL );
}

//West boundary of domain corresponds to acoustic source.
void setWestBoundaryCellVariables( Cell *gridCell )
{           
    // Ajustar variaveis do vizinho do oeste com condicoes de fronteira
    gridCell->westP = PRESSAO_LIVRE;
    gridCell->westRho = PRESSAO_LIVRE/(R*TEMPERATURA_LIVRE);
    gridCell->westU =  4.0*(VEL_SOM);    
    gridCell->westV = 0.0;
    gridCell->westE = R*TEMPERATURA_PAREDE/(GAMMA-1.0);
    gridCell->westT = TEMPERATURA_LIVRE;
    gridCell->westMi = MI_ZERO;
    gridCell->westK = (MI_ZERO*GAMMA*R)/((GAMMA-1.0)*PRANDTL);
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
