#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
    
#include <fstream>
    using std::ofstream;
#include "Cell.h"
#include "functions.h"

//constructor
Cell::Cell():
    type('b'),
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
    xBlack(-1.0),
    yBlack(-1.0),
	uBlack(0),
	uWhite(0),
    group(0),
    superGroup(0)
{ }

//destructor
Cell::~Cell()
{}

// Draws a cell as a wired square.
void Cell::drawCell( )
{
    double x0, y0, x1, y1;
    x0 = centerX - halfFaceLength;
    y0 = centerY - halfFaceLength;
    x1 = centerX + halfFaceLength;
    y1 = centerY + halfFaceLength;
    drawRectangle( x0, y0, x1, y1 );
}

//Prints information for cell nodes.
void Cell::print()
{
    ofstream outputFile( "outputCell.txt", ios::app ); //trunc
    
    outputFile << "Cell " << gridPosition << "\n\n";
    
    outputFile << "Type = " << type << "\n";
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
    outputFile << "uBlack = " << uBlack << "\n\n";
}


//Prints information for bunch of sister cell nodes.
void Cell::printBunch()
{
    ofstream outputFile( "outputCellVariable.txt", ios::app );
    
    outputFile << "Cell " << gridPosition << endl;
    
    outputFile << "Type = " << type << endl;
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

//end program Cell.cpp
