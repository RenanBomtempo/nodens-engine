//initialCondition.cpp

#include "Grid.h"
#include "functions.h"
#include "table.h"

/*______________________________________________________________________________
      
                        FUNCTION initialCondition()
                        
   Percorre todas as celulas do grid e para cada uma delas chama getOccurrences,
   que retorna o numero de pontos (armazenados em table) que existem dentro da area
   delimitada pela celula. O valor retornado é copiado para cell->uBlack, e será usado
   futuramente para determinar se a celula ainda precisa ser refinada ou não.
______________________________________________________________________________*/
void initialCondition( Grid *grid, Table *table )
{
    Cell *gridCell;
    gridCell = grid->firstGridCell;
    
    while( gridCell != 0 )
    {
        gridCell->uBlack = table->getOccurrences( gridCell->centerX, gridCell->centerY, gridCell->faceLength );   
        gridCell = gridCell->next; 
    }
}//end function initialCondition()
//end file initialCondition.cpp

