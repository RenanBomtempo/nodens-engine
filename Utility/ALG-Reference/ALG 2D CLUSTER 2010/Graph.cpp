#include <fstream>   // Used for manipulation of files.
#include <sstream>   // Used for string streams.
#include <iostream>  // Used for input and output of streams.
#include <math.h>
#include "Graph.h"

using namespace std;

/*=============================================================================*
 *                                Data methods                                 *
 *=============================================================================*/
 
/**
 * Data constructor. It receives as parameter the cell number, number of white 
 * points and number of black points.
 */
Data:: Data( int cellNumber, int activeWhite, int activeBlack, double centerX,
             double centerY, double faceLength) {
    this->cellNumber  = cellNumber;
    this->activeWhite = activeWhite;
    this->activeBlack = activeBlack;
    this->centerX = centerX;
    this->centerY = centerY;
    this->faceLength = faceLength;
}

/**
 * Data destructor
 */
Data::~Data() {}

/*
/*=============================================================================*
 *                                Group methods                                *
 *=============================================================================*/

/**
 * Group constructor
 */
Group::Group( int groupNumber ) {
    this->groupNumber = groupNumber;
    this->nBlack = 0;
    this->nWhite = 0;
    cells = new list<Data>();
}

/**
 * Group destructor
 */
Group::~Group() {
    cells->clear();
}

/**
 * Returns true if the list of cells is empty, or false otherwise.
 */
bool Group::isEmpty() {
     return cells->empty();
}

/**
 * Inserts a new data in the list.
 */
void Group::insert( Data data ) {
     cells->push_back( data );
}

/**
 * Returns true if group1 is less than to group2, otherwise it returns false.
 * The comparison is made based on the number of each Group. The purpose
 * of this function is to provide a comparison method to sort methods, where a
 * list of groups can be sorted acording to the number of each group.
 */
bool Group::compare( Group group1, Group group2 ) {
     return (group1.getNumber() < group2.getNumber());
}

/**
 * Returns the number of a group.
 */
int Group::getNumber() {
    return groupNumber;
}

/**
 * Returns a reference to group's list of cells.
 */
list<Data> *Group::getCellList() {
    return cells;
}

/**
 * Gets the value of nBlack of a group.
 */
void Group::putnBlack( int numberOfBlackPoints ) {
    nBlack = numberOfBlackPoints;
}

/**
 * Gets the value of nWhite of a group.
 */
void Group::putnWhite( int numberOfWhitePoints ) {
    nWhite = numberOfWhitePoints;
}

/**
 * Returns the value of nBlack of a group.
 */
int Group::returnsnBlack() {
    return nBlack;
}

/**
 * Returns the value of nWhite of a group.
 */
int Group::returnsnWhite() {
    return nWhite;
}

/*=============================================================================*
 *                         SuperGroups methods                                 *
 *=============================================================================*/

/**
 * SuperGroup constructor
 */
SuperGroup::SuperGroup() {
    groups = new list<Group>();
    numberOfGroups = 0;
    numberOfBlackPoints = 0;
    numberOfWhitePoints = 0;
    intensity = 0.0;
}

/**
 * SuperGroup destructor
 */
SuperGroup::~SuperGroup() {
    
    groups->clear();
}

/**
 * Returns true if a given group is in the super group, otherwise it returns false.
 * The parameter is the group's number which will be searched.
 */
bool SuperGroup::searchGroup( int groupNumber ) {
     if( getNumberOfGroups() == 0 ) return false;

     int i = 0;
     list<Group>::iterator iterator = groups->begin();
     while( i < getNumberOfGroups() && iterator->getNumber() != groupNumber ) {
            iterator++;
            i++;
     }
     return (i != getNumberOfGroups());
}

/**
 * Returns the sucessor group number of a given group in a super group . If the 
 * group does not exist or it has no sucessor, this function returns -1.
 */
int SuperGroup::searchNextGroup( int groupNumber ) {
     if( getNumberOfGroups() == 0 ) return false;
    
     int i = 0;
     list<Group>::iterator iterator = groups->begin();
     while( iterator->getNumber() != groupNumber && i < getNumberOfGroups() ) {
            iterator++;
            i++;
     }
     if( (i == getNumberOfGroups()) || (i == getNumberOfGroups() - 1) ) return -1;
     else return (++iterator)->getNumber();
}

/**
 * Returns true if a super group has no group, otherwise it returns false.
 */
bool SuperGroup::isEmpty() {
     return groups->empty();
}

/**
 * Inserts a group in a super group.
 */
void SuperGroup::insert( Group group ) {
     groups->push_back( group );
     numberOfGroups++;
}

/**
 * Returns the number of groups in a super group.
 */
int SuperGroup::getNumberOfGroups() {
    return numberOfGroups;
}

/**
 * Sorts the super group's list of groups  acording to the number of each group
 * in the list.
 */
void SuperGroup::sort() {
     
     //Group::compare() is the method used by the sorting algorithm to compare
     // two groups. It is the parameter of the sorting method.
     groups->sort(Group::compare);
}

/**
 * Returns a pointer to the list of groups of a super group.
 */
list<Group> *SuperGroup::getGroupList() {
    return groups;
}

/**
 * Returns the number of white points in a super group.
 */
int SuperGroup::getNumberOfWhitePoints() {
    return numberOfWhitePoints;
}

/**
 * Returns the number of black points in a super group.
 */
int SuperGroup::getNumberOfBlackPoints() {
    return numberOfBlackPoints;
}


/**
 * Gets the number of white and black points in each group of a supergroup,
 * and gets the number of white and black points of the supergroup.
 */
void SuperGroup::getNumberOfPoints() {
    int nWhiteOfTheGroup = 0;
    int nBlackOfTheGroup = 0;
    int numberOfCells = 0;
    list<Group>::iterator group = this->getGroupList()->begin();
    list<Data>::iterator cell;
    
    for( int i = 0; i < numberOfGroups; i++ ) {
         nWhiteOfTheGroup = 0;
         nBlackOfTheGroup = 0;
         numberOfCells = group->getCellList()->size();
         cell = group->getCellList()->begin();
         for( int j = 0; j < numberOfCells; j++ ) {
              nWhiteOfTheGroup += cell->activeWhite;
              nBlackOfTheGroup += cell->activeBlack;
              cell++;
         }
         numberOfWhitePoints += nWhiteOfTheGroup;
         numberOfBlackPoints += nBlackOfTheGroup;
         
         group->putnWhite( nWhiteOfTheGroup );
         group->putnBlack( nBlackOfTheGroup );

         group++;         
    }
}

/**
 * Returns the intensity of a super group.
 */
double SuperGroup::getIntensity(){
       return intensity;
}

/**
 * Set the intensity of a super group.
 */
void SuperGroup::setIntensity( int TOTAL, int NBlack ) {
       double black = (double) numberOfBlackPoints;
       double white = (double) numberOfWhitePoints;
       double mi = NBlack*( ( black+white )/TOTAL );
       
       if (black>mi)
          intensity = black*log( black/mi ) + ( NBlack-black )*log(( NBlack-black )/( NBlack-mi ));
       else
          intensity=0; 
}


/*=============================================================================*
 *                               Graph methods                                 *
 *=============================================================================*/

/**
 * Graph constructor
 */
Graph::Graph( int numberOfSuperGroups ) {
    this->numberOfSuperGroups = numberOfSuperGroups;
    this->numberOfGroups = 0;
    this->superGroups = new SuperGroup*[numberOfSuperGroups];
    for( int i = 0; i < numberOfSuperGroups; i++ ) {
         superGroups[i] = new SuperGroup();
    }
}

/**
 * Graph destructor
 */
Graph::~Graph() {
    for( int i = 0; i < numberOfSuperGroups; i++ ) {
         delete superGroups[i];
    }
    delete [] superGroups;
}

/**
 * Returns a super group reference at the position indicated by index. If the 
 * index informed is out of bounds, a null value (0) is returned and a message
 * is printed on the stardard error output.
 * Warning: The list index goes from 1 to the number of super groups.
 */
SuperGroup *Graph::getSuperGroupAt( int index ) {
    if( index < 1 || index > numberOfSuperGroups ) {
        cerr << "Error on function Graph::getSuperGroupAt(): \n"
             << "Index out of bounds. Value received: "
             << index << endl << endl;
        return NULL;
    }
    else {
        return superGroups[index - 1];
    }
}

/**
 * Returns the super group's number of a given group. If the group does not exist,
 * it returns zero (0).
 */
int Graph::searchSuperGroup( int group ) {
    bool hasFound = false;
    int i = 0;
    
    if ( group == 0 ) return 0;
    while( !hasFound && i < numberOfSuperGroups ) {
           if( superGroups[i]->searchGroup(group) ) hasFound = true;
           else i++;
    }
    if( hasFound ) return i+1;
    else {
         cerr<<"\nERROR in function Graph::searchSuperGroup()"
               " - The group searched, "<<group+1<<", doesn't exist."; 
         return 0;
    }
}

/**
 * Returns the number of super-groups in the graph.
 */
int Graph::getNumberOfSuperGroups(){
    return numberOfSuperGroups;
}

/**
 * Gets the number of all points (black and white) in the graph, for each supergroup.
 */
void Graph::getNumberOfAllPoints() {
     int i;
     for ( i=0; i<numberOfSuperGroups; i++ ){
           superGroups[i]->getNumberOfPoints();
     }
}

int Graph::getNumberOfAllBlackPoints(){
     int i, number=0;
     
     for ( i=0; i<numberOfSuperGroups; i++ ){
           number = number + superGroups[i]->getNumberOfBlackPoints();
     }
     return number;
}

int Graph::getNumberOfAllWhitePoints(){
     int i, number=0;
     
     for ( i=0; i<numberOfSuperGroups; i++ ){
           number = number + superGroups[i]->getNumberOfWhitePoints();
     }
     return number;
}

/*______________________________________________________________________________

                             FUNCTION SuperGroup()
        Cria o k-esimo SuperGroup a partir do grupo i.
        Percorre toda a linha i da Matrix, e para cada coneccao encontrada (Matrix[i][j]!=0)
        verifica se o grupo j ja nao pertence ao k-esimo superGroup. Caso ainda nao
        pertença, insere-o no fim da k-esima lista do Grafo.
        Quando chegar ao fim da linha i, faz i igual ao grupo posterior ao atual i
        na lista k do Grafo, executando o mesmo processo descrito acima.
        Os processos se repetem ate que se chegue ao fim da lista k.
        Dessa forma, garantimos que todos os grupos que tenham ligacoes entre si (com
        a condicao inicial de que o grupo i recebido como parametro pela funcao seja o
        ponto de partida dessas ligacoes) tenham sido classificados como pertencentes
        a um mesmo superGroup. 
        Ao fim, quando o SuperGroup jah estiver completo, organiza os itens da lista,
        colocando os numeros dos grupos em ordem crescente.
*/
void Graph::superGroup( int i, int k, int **matrix ) {
     Group *newGroup = new Group(i);
     getSuperGroupAt(k)->insert(*newGroup);
     int j = i + 1;
     bool END = false;

     while( !END ) {
            while( j <= numberOfGroups ) {
                if( matrix[i][j]!=0 && !getSuperGroupAt(k)->searchGroup(j) ) {
                    newGroup = new Group(j);
                    getSuperGroupAt(k)->insert(*newGroup);
                }
                j++;
            }
            i = getSuperGroupAt(k)->searchNextGroup(i);
            j = 1;
            if( i < 0 ) // o grupo i eh o ultimo da lista.
                END = true;
     }
     getSuperGroupAt(k)->sort();

     delete newGroup;
}

/**
 * Sets the super group of each cell of a grid.
 */
void Graph::numberCellSuperGroup( Grid *grid ) {
     Cell *gridCell = grid->firstGridCell;
     int superGroup = 0;
     bool continueSearching = true;

     while ( gridCell->next != 0 ){
            superGroup = this->searchSuperGroup( gridCell->group );
            gridCell->superGroup = superGroup;
            while( continueSearching && gridCell->next != 0 ) {
                   if( gridCell->next->group == gridCell->group ) {
                       gridCell->next->superGroup = superGroup;
                       gridCell = gridCell->next;
                   }
                   else {
                        continueSearching = false;
                   }
            }
            if( gridCell->next != 0 ) {
                gridCell = gridCell->next;
            }

            continueSearching = true;
     }

}

/*______________________________________________________________________________

                          FUNCTION mainSuperGroups()
      Recebe como parametros a matriz que contem as ligacoes entre os grupos e o
      numero total de grupos. Percorre todas as linhas da Matrix (i), e como cada
      linha representa o numero de um grupo, verifica se aquele grupo i jah existe
      no Grafo, ou seja, se jah pertence a algum superGroup. Se nao existir, chama
      a funcao SuperGroup() para criar o k-esimo superGroup, cujo elemento inicial
      sera o grupo i. Depois de formar todos os k SuperGroups, armazena na variavel
      numberOfSuperGroups o numero total k.
*/
void Graph::mainSuperGroups( int numberOfGroups, int **matrix ) { 
    int i, j; 
    int k = 0;
    bool isInSuperGroup = false; 
   
    this->numberOfGroups = numberOfGroups;

    for( i = 1; i <= numberOfGroups; i++ ) {
         // Percorre cada super-groupo j procurando o grupo i.
         j = 1;
         while( !isInSuperGroup && j <= numberOfGroups ) {
              if( !getSuperGroupAt(j)->isEmpty() && getSuperGroupAt(j)->searchGroup(i) ) {
                  isInSuperGroup = true;
              }
              j++;
         }
         if( !isInSuperGroup ) {            
             k++;
             this->superGroup(i, k, matrix);
         }
         isInSuperGroup = false;
    }
    this->numberOfSuperGroups = k;

}

/* _____________________________________________________________________________
                               FUNCTION completeGraph()

          Insere as informaçoes de cada celula que pertence a um SuperGroup
          na posicao do Grafo referente a esse SuperGroup.
          As variaveis inteiras "activeBlack" e "activeWhite" indicam o numero
          de pontos pretos e brancos respectivamente que a celula contém dentro
          de seus limites no grid.

*/
void Graph::completeGraph( Grid *grid ) {
     Cell *gridCell       = grid->firstGridCell;
     Data *data           = 0;
     int superGroupNumber = 0;
     int i                = 0;
     int groupListSize    = 0;
     list<Group>::iterator group;
     
     for( gridCell = grid->firstGridCell; gridCell != 0; gridCell = gridCell->next ) {
         superGroupNumber = gridCell->superGroup;
         if( superGroupNumber != 0 ) {

             // Searches for the current cell's group on the group list.
             group = getSuperGroupAt(superGroupNumber)->getGroupList()->begin();
             i = 0;
             groupListSize = getSuperGroupAt(superGroupNumber)->getNumberOfGroups();
             while( i < groupListSize && group->getNumber() != gridCell->group ) {
                 group++;
                 i++;
             }

             if( i == groupListSize ) {
                 std::cerr << "Error on function completeGraph(). The group " << gridCell->group 
                           << " could not be found in the super group "
                           << gridCell->superGroup << std::endl << std::endl;
                 system("pause");
                 exit(1);
             }
           
             // Inserts the new data in the cell's group.
             data = new Data( gridCell->gridPosition, gridCell->uWhite, gridCell->uBlack,
                              gridCell->centerX, gridCell->centerY, gridCell->faceLength );
                         
             group->insert(*data);             
         }
     }   
}

/* _____________________________________________________________________________
                           FUNCTION getCohesion()
          Calculates the cohesion of each graph as:
          
                cohesion = cohesion * pow( ( b/(mi*(b+w)) ), b )
          
          b = number of black points of a supergroup
          w = number of white points of a supergroup
          mi = (Total of black points in the grid)/(Total of points in the grid)
*/
void Graph::getCohesion( int NWhite, int NBlack ){
     int i;
     double mi, b, w;
     
     mi = ( NBlack / double(NBlack + NWhite) );
     cohesion = 1.0;
     for (i = 0; i<numberOfSuperGroups; i++){
         
         b = ( superGroups[i]->getNumberOfBlackPoints() );
         w = ( superGroups[i]->getNumberOfWhitePoints() );
         if ( (b+w)== 0 )
            cout<<"\nError on function Graph::getCohesion(): there is no points"
                <<" in the supergroup "<<i+1<<"\n";
         
         cohesion = cohesion * pow( ( b/(mi*(b+w)) ), b );
     }
     b = NBlack - getNumberOfAllBlackPoints();
     w = NWhite - getNumberOfAllWhitePoints();
     
     if ( (b+w)== 0 )
        cout<<"\nError on function Graph::getCohesion(): there is no points"
            <<" out of the supergroups' region\n";
     
     cohesion = cohesion * pow( ( b/(mi*(b+w)) ), b );
     
}

/* 
   Sets the intensity of each supergroup of the graph 
*/
void Graph::setIntensity( int TOTAL, int NBlack ){
       for (int i=0; i<numberOfSuperGroups; i++){
           superGroups[i]->setIntensity(TOTAL,NBlack);
       }
}

/*
  Returns a vector with the numbers of the supergroups in order of intensity.
*/
int* Graph::sortInsertionIntensity(){
     int *order, i, j;
     double chave, intenses[numberOfSuperGroups];
     
     order = new int[numberOfSuperGroups];
     for ( i=0; i<numberOfSuperGroups; i++ ){
         order[i] = i+1;
         intenses[i] = superGroups[i]->getIntensity();
     }
     
     for (j=1; j<numberOfSuperGroups; j++){
         chave = intenses[j];
		 i = j-1;
		 while(i >= 0 && intenses[i] > chave){
			intenses[i+1] = intenses[i];
			order[i+1] = order[i];
			i--;
		 }		
		 intenses[i+1] = chave;
		 order[i+1] = j+1;
	 }
     return order;
}

/*
  Returns the highest intensity of all supergroups in the Graph.
*/
double Graph::highestIntensity( int &numberOfTheBest ){
       double intensity = 0.0, auxIntensity;
       int m;
       
       for (int i=0; i<numberOfSuperGroups; i++){
           auxIntensity = superGroups[i]->getIntensity();
         
           if ( intensity < auxIntensity ){
              intensity = auxIntensity;
              numberOfTheBest = i+1;
           }
       }
  
       return intensity;
}

/*
  Returns the higher number of blackpoints in the supergroups in the Graph.
*/
int Graph::getNMaxBlackPoints( double &scan ){
       int nMax=0, n;
       for (int i=0; i<numberOfSuperGroups; i++){
           n = superGroups[i]->getNumberOfBlackPoints();
           if ( n > nMax ){
              nMax = n;
              scan = superGroups[i]->getIntensity();
           }
       }
       return nMax;
}

/*
  Print in the screen the statistic scan of all supergroups in the Graph.
*/
void Graph::printScan(){
       for (int i=0; i<numberOfSuperGroups; i++){
           cout << "\nSuper Grupo " << i+1 << ", scan = " << superGroups[i]->getIntensity();
       }
}

/*______________________________________________________________________________
                           
                           FUNCTION drawStatisticScan()
                        	
    Print the value of statistic scan of each supergroup.
______________________________________________________________________________*/
void Graph::writeStatisticScan( int color )
{
    setColor( color );
    char statisticScan[5];
    for (int i=0; i<numberOfSuperGroups; i++){
           sprintf( statisticScan, "%f",  superGroups[i]->getIntensity() );
           double x = superGroups[i]->getGroupList()->begin()->getCellList()->begin()->centerX + 0.005;
           double y = superGroups[i]->getGroupList()->begin()->getCellList()->begin()->centerY + 0.005;
           drawString( statisticScan, GLUT_BITMAP_HELVETICA_12, x, y); 
    }
    
}//end writeStatisticScan() function.



/**
 * Prints a graph on a file whose name is informed as parameter.
 */
void Graph::print( char *fileName, Table *blackTable, Table *whiteTable ) {
     ofstream file( fileName, ios::trunc );
     if( !file.is_open() ) {
         cerr << "Error on function Graph::print(): The file " << fileName 
              << " could not be opened." << endl;
         return;
     }

     int k;
     list<Group>::iterator group;
     list<Data>::iterator cell;
     int nGroups = 0;
     long int *vector;
     
     file << "Number of super groups: " << this->numberOfSuperGroups << endl;
     file << "Cohesion = " << this->cohesion << endl << endl;
     for ( int i = 1; i <= numberOfSuperGroups; i++ ) {
         file << "SUPER GROUP " << i << endl;
         file << "Number of groups: " << getSuperGroupAt(i)->getNumberOfGroups() << endl;
         file << "Intensity = " << getSuperGroupAt(i)->getIntensity() << endl;
         file << "Number of black points: " << getSuperGroupAt(i)->getNumberOfBlackPoints() << endl;
         file << "Number of white points: " << getSuperGroupAt(i)->getNumberOfWhitePoints() << endl;
         group   = getSuperGroupAt(i)->getGroupList()->begin();
         nGroups = getSuperGroupAt(i)->getGroupList()->size();
         
         for ( int j = 0; j < nGroups; j++ ) {
             file << "\nGROUP " << group->getNumber() << endl;
             file << "Number of black points: " << group->returnsnBlack() << endl;
             file << "Number of white points: " << group->returnsnWhite() << endl << endl;
             for ( cell=group->getCellList()->begin(); cell!=group->getCellList()->end(); ++cell ){
                 file << "Cell " << cell->cellNumber << endl;
                 
                 if ( cell->activeBlack > 0 ){
                    vector = blackTable->searchPoints(cell->centerX, cell->centerY,
                                                   cell->faceLength, cell->activeBlack);
                    file << cell->activeBlack << " Black Points:";
                    for ( k=0; k < cell->activeBlack; k++ ){
                        file << " (" << blackTable->getsXPoint()[vector[k]] << ", "
                                     << blackTable->getsYPoint()[vector[k]] << ")";
                    }
                    file << endl;                    
                 }
                 else
                    file << "Black Points: None." << endl;
                    
                 if ( cell->activeWhite > 0 ){
                    vector = whiteTable->searchPoints(cell->centerX, cell->centerY,
                                                   cell->faceLength, cell->activeWhite);
                    file << cell->activeWhite << " White Points:";
                    for ( k=0; k < cell->activeWhite; k++ ){
                        file << " (" << whiteTable->getsXPoint()[vector[k]] << ", "
                                     << whiteTable->getsYPoint()[vector[k]] << ")";
                    }
                    file << endl;
                 }
                 else
                    file << "White Points: None." << endl;
                 file << "-----------------------------------------" << endl << endl;
             }
             file << endl;
             group++;
         }
         file << "=============================================================\n\n";
    }
    file.close();

}

void Graph::printSummarySuperGroups( char *fileName ){

     ofstream file( fileName, ios::trunc );
     if( !file.is_open() ) {
         cerr << "Error on function Graph::print(): The file " << fileName 
              << " could not be opened." << endl;
         return;
     }
     
     file << "Number of super groups: " << this->numberOfSuperGroups << endl;
     file << "Cohesion = " << this->cohesion << endl << endl;
     
     file << "SUPER-GROUP  Statistic Scan   Number of black points   Number of white points";
     for ( int i = 1; i <= numberOfSuperGroups; i++ ) {
         file << "\n  " << i;
         file << "\t\t" << getSuperGroupAt(i)->getIntensity();
         file << "\t\t\t" << getSuperGroupAt(i)->getNumberOfBlackPoints();
         file << "\t\t\t" << getSuperGroupAt(i)->getNumberOfWhitePoints();
     }
}

