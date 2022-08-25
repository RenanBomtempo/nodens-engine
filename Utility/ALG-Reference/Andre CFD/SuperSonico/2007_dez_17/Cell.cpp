//Cell.cpp
//Cell class implementation. 

#include "Cell.h"
#include "constants.h"

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
    
#include <fstream>
    using std::ofstream;

//constructor
Cell::Cell():
    // Do ALG
    type('b'),
    active(false),
    level(0),
    bunchNumber(0),
    north(0),
    south(0),
    east(0),
    west(0),
    previous(0),
    next(0),
    singleConnector(0),
    doubleConnector1(0),
    doubleConnector2(0),
    gridPosition(0),
    hilbertShapeNumber(0),
    centerX(0.0),
    centerY(0.0),
    faceLength(1.0),
    halfFaceLength(0.5),
    // Variaveis fisicas
    rhoAtual(0.0),
    rhoAnterior(0.0),
    rhoIterAnterior(0.0),
    uAtual(0.0),
    uAnterior(0.0),
    uIterAnterior(0.0),
    vAtual(0.0),
    vAnterior(0.0),
    vIterAnterior(0.0),
    TAtual(0.0),
    TAnterior(0.0),
    TIterAnterior(0.0),
    PhiAtual(0.0),
    PhiAnterior(0.0),
    pAtual(0.0),
    pAnterior(0.0),
    pIterAnterior(0.0),
    pLinha(0.0),
    miAtual(0.0),
    miAnterior(0.0),
    miIterAnterior(0.0),
    kAtual(0.0),
    kAnterior(0.0),
    northFaceRho(0.0),
    southFaceRho(0.0),
    eastFaceRho(0.0),
    westFaceRho(0.0),
    eastFaceU(0.0),
    eastFaceUAnterior(0.0),
    westFaceU(0.0),
    westFaceUAnterior(0.0),
    northFaceU(0.0),
    southFaceU(0.0),
    northFaceV(0.0),
    northFaceVAnterior(0.0),
    southFaceV(0.0),
    southFaceVAnterior(0.0),
    eastFaceV(0.0),
    westFaceV(0.0),
    northFaceT(0.0),
    southFaceT(0.0),
    eastFaceT(0.0),
    westFaceT(0.0),
    northFaceP(0.0),
    southFaceP(0.0),
    eastFaceP(0.0),
    westFaceP(0.0),
    northFaceMi(0.0),
    southFaceMi(0.0),
    eastFaceMi(0.0),
    westFaceMi(0.0),
    northFaceK(0.0),
    southFaceK(0.0),
    eastFaceK(0.0),
    westFaceK(0.0),
    // Variaveis de interpolacao
    northFaceGamma(0.0),
    southFaceGamma(0.0),
    eastFaceGamma(0.0),
    westFaceGamma(0.0),
    northFaceBeta(0.0),
    southFaceBeta(0.0),
    eastFaceBeta(0.0),
    westFaceBeta(0.0),
    // Do SIMPLEC
    I_n(0.0),
    I_s(0.0),
    I_e(0.0),
    I_w(0.0),
    d_n(0.0),
    d_s(0.0),
    d_e(0.0),
    d_w(0.0),
    volumeFicticio(false),
    direcaoFronteira(NAO_FRONTEIRA),
    // Variaveis do sistema linear
    b_MomentoX(0.0),    
    b_MomentoY(0.0),    
    b_PressaoLinha(0.0),    
    b_Energia(0.0),
    // Resolucao por gradiente biconjugado    
    Ax(0.0),
    r(0.0),
    p(0.0),
    p1(0.0),
    Ap(0.0),
    r_(0.0),
    As(0.0),
    s(0.0)
{
    firstElementMomentoX = NULL;
    firstElementMomentoY = NULL;
    firstElementPressaoLinha = NULL;
    firstElementEnergia = NULL;
}

//destructor
Cell::~Cell()
{}


/*______________________________________________________________________________
                            
                            iniciaElement()
                            
    Cria o primeiro elemento da lista encadeada representada por 
escolheElementEquacao.

    Parametros:
        valor: valor numerico do primeiro elemento da lista encadeada;
        escolheElementEquacao: referencia para qual equacao a lista encadeada corresponde.

Criado por Andre STALTZ dia 23/04/07
______________________________________________________________________________*/
void Cell::iniciaElement( double valor, Element **escolheElementEquacao )
{
    // Esvazia lista encadeada anterior (antiga funcao freeMemory)
    Element *element,
            *auxiliarElement;
            
    element = (*escolheElementEquacao);
              
    while( element != NULL )
    {
        auxiliarElement = element->next; 
        delete element;
        element = auxiliarElement;    
    }
   
    // Aloca novo element
    (*escolheElementEquacao) = new Element;

    // O novo element reconhece a celula atual
    (*escolheElementEquacao)->column = this->gridPosition;
    (*escolheElementEquacao)->cell = this;
    (*escolheElementEquacao)->value = valor;
    (*escolheElementEquacao)->next = NULL;
}


/*______________________________________________________________________________

                            insereElement()

    Insere um elemento de lista encadeada (relativo a escolheElementEquacao) apos
o primeiro elemento dessa lista.

    Parametros:
        valor: valor numerico do primeiro elemento da lista encadeada;
        escolheElementEquacao: referencia para qual equacao a lista encadeada corresponde.

Criado por Andre STALTZ dia 24/04/07
______________________________________________________________________________*/
void Cell::insereElement( double valor, Cell *celulaDesteElement, Element **escolheElementEquacao )
{
    if( celulaDesteElement->type == 'b' && celulaDesteElement->active == true ){

        Element *novo_element;

        // Aloca novo element
        novo_element = new Element;

        // O novo element reconhece a celula dada
        novo_element->column = celulaDesteElement->gridPosition;
        novo_element->cell = celulaDesteElement;
        novo_element->value = valor;

        // O novo element e' colocado depois do primeiro element
        Element *auxiliarElement;
        auxiliarElement = (*escolheElementEquacao)->next;
        (*escolheElementEquacao)->next = novo_element;
        novo_element->next = auxiliarElement;
    }
}



//Prints information for cell nodes.
/*void Cell::print()
{
    ofstream outputFile( "outputCell.txt", ios::app ); //trunc
    
    outputFile << "Cell " << gridPosition << "\n\n";
    
    outputFile << "Type = " << type << "\n";
    outputFile << "Active = " << active << "\n";
    outputFile << "Level = " << level << "\n";
    outputFile << "bunchNumber = " << bunchNumber << "\n";
    if( east != 0 )
    {
        outputFile << "   East neighbor = " << east->gridPosition << " has type " 
             << east->type  << " and has level " << east->level << "\n";
        if( east->type == 'w' )
        {
            if( east->singleConnector != 0 )
            outputFile << "\tsingleConnector = " << east->singleConnector->gridPosition
                 << " has type " << east->singleConnector->type << "\n";
            if( east->doubleConnector1 != 0 )
            outputFile << "\tdoubleConnector1 = " << east->doubleConnector1->gridPosition
                 << " has type " << east->doubleConnector1->type << "\n";
            if( east->doubleConnector2 != 0 )
            outputFile << "\tdoubleConnector2 = " << east->doubleConnector2->gridPosition
                 << " has type " << east->doubleConnector2->type << "\n";
        }
    }    
    if( north != 0 )
    {
        outputFile << "   North neighbor = " << north->gridPosition << " has type " 
             << north->type  << " and has level " << north->level << "\n";
        if( north->type == 'w' )
        {
            if( north->singleConnector != 0 )
            outputFile << "\tsingleConnector = " << north->singleConnector->gridPosition
                 << " has type " << north->singleConnector->type << "\n";
            if( north->doubleConnector1 != 0 )
            outputFile << "\tdoubleConnector1 = " << north->doubleConnector1->gridPosition
                 << " has type " << north->doubleConnector1->type << "\n";
            if( north->doubleConnector2 != 0 )
            outputFile << "\tdoubleConnector2 = " << north->doubleConnector2->gridPosition
                 << " has type " << north->doubleConnector2->type << "\n";
        }
    }    
    if( west != 0 )
    {
        outputFile << "   West neighbor = " << west->gridPosition << " has type " 
             << west->type  << " and has level " << west->level << "\n";
        if( west->type == 'w' )
        {
            if( west->singleConnector != 0 )
            outputFile << "\tsingleConnector =" << west->singleConnector->gridPosition
                 << " has type " << west->singleConnector->type << "\n";
            if( west->doubleConnector1 != 0 )
            outputFile << "\tdoubleConnector1 = " << west->doubleConnector1->gridPosition
                 << " has type " << west->doubleConnector1->type << "\n";
            if( west->doubleConnector2 != 0 )
            outputFile << "\tdoubleConnector2 = " << west->doubleConnector2->gridPosition
                 << " has type " << west->doubleConnector2->type << "\n";
        }
    }    
    if( south != 0 )
    {
        outputFile << "   South neighbor = " << south->gridPosition << " has type " 
             << south->type  << " and has level " << south->level << "\n";
        if( south->type == 'w' )
        {
            if( south->singleConnector != 0 )
            outputFile << "\tsingleConnector = " << south->singleConnector->gridPosition
                 << " has type " << south->singleConnector->type << "\n";
            if( south->doubleConnector1 != 0 )
            outputFile << "\tdoubleConnector1 = " << south->doubleConnector1->gridPosition
                 << " has type " << south->doubleConnector1->type << "\n";
            if( south->doubleConnector2 != 0 )
            outputFile << "\tdoubleConnector2 = " << south->doubleConnector2->gridPosition
                 << " has type " << south->doubleConnector2->type << "\n";
            outputFile << "\n";
        }
    }    
    if( previous != 0 )
        outputFile << "Previous cell = " << previous->gridPosition << "\n";
    if( next != 0 )
        outputFile << "Next cell = " << next->gridPosition << "\n";
    outputFile << "Center = ( " << centerX << ", " << centerY << " )\n";
    outputFile << "Face length = " << faceLength << "\n";
    outputFile << "Half face length " << halfFaceLength << "\n";
    outputFile << 
    "________________________________________________________________________\n" 
    << endl;
}

//Prints information for transition nodes.
void Cell::printTransitionNode()
{
    ofstream outputFile( "outputTransitionNode.txt", ios::app ); //trunc
    
    outputFile << "Transition Node" << endl;    
    outputFile << "Type = " << type << endl;
    outputFile << "Level = " << level << endl; 
    if( singleConnector != 0 )
        outputFile << "Single Connector: " << singleConnector->type << endl;
    else
        outputFile << "Single Connector = 0" << endl;    
    if( doubleConnector1 != 0 )
        outputFile << "Double Connector 1: " << doubleConnector1->type << endl;
    else
        outputFile << "Double Connector 1 = 0" << endl;    
    if( doubleConnector2 != 0 )
        outputFile << "Double Connector 2: " << doubleConnector2->type << endl;
    else
        outputFile << "Double Connector 2 = 0" << endl;   
}

//Prints specific variable information for cell nodes.
void Cell::printVariable()
{
    ofstream outputFile( "outputCellVariable.txt", ios::app );
    
    outputFile << "Cell " << gridPosition << endl;
    // Trecho abaixo comentado pois 'u' e' uma variavel obsoleta
    //outputFile << "u = " << u << "\n\n";
}


//Prints information for bunch of sister cell nodes.
void Cell::printBunch()
{
    ofstream outputFile( "outputCellVariable.txt", ios::app );
    
    outputFile << "Cell " << gridPosition << endl;
    
    outputFile << "Type = " << type << endl;
    outputFile << "Active = " << active << endl;
    outputFile << "Level = " << level << endl;
    outputFile << "bunchNumber = " << bunchNumber << "\n\n";
    
    outputFile << "Cell Number 1: " << bunchNumber << endl;
    outputFile << "Cell Number 2: " << next->bunchNumber << endl;
    outputFile << "Cell Number 3: " << next->next->bunchNumber << endl;
    outputFile << "Cell Number 4: " << next->next->next->bunchNumber << endl;
    outputFile << 
    "________________________________________________________________________\n" 
    << endl;  
}

*/

//end program Cell.cpp
