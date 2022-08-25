#include <list>
#include <iostream>
#include <string>
#include <GL/glut.h>

#include "Grid.h"
#include "Table.h"
#include "functions.h"

using std::list;
using std::string;

/**
 * Class Data
 */
class Data {
public:
       int cellNumber;
       int activeBlack; /* tell the number of black points in the cell */
       int activeWhite; /* tell the number of white points in the cell */
       double centerX;
       double centerY;
       double faceLength;

       Data( int, int, int, double, double, double );
       ~Data();
};

/**
 * Class Group
 */
class Group {
private:
        int groupNumber;
        int nBlack;
        int nWhite;
        list<Data> *cells;
        
public:
       Group( int groupNumber );
       ~Group();
       
       static bool compare( Group group1, Group group2 );
       int getNumber();
       void setNumber( int number );
       bool isEmpty();
       void insert( Data data );
       bool searchGroup( int group );
       int searchNextGroup( int group );
       list<Data> *getCellList();
       void putnBlack( int numberOfBlackPoints );
       void putnWhite( int numberOfWhitePoints );
       int returnsnBlack();
       int returnsnWhite();
};

/**
 * Class SuperGroup
 */
class SuperGroup {
private:
        list<Group> *groups;
        int numberOfGroups;
        int numberOfBlackPoints;
        int numberOfWhitePoints;
        double intensity;
        
public:
       SuperGroup();
       ~SuperGroup();
       
       bool isEmpty();
       void insert( Group group );
       bool searchGroup( int groupNumber );
       int searchNextGroup( int groupNumber );
       int getNumberOfGroups();
       void sort();
       double getIntensity();
       void setIntensity( int TOTAL, int NBlack );
       void getNumberOfPoints();
       int getNumberOfWhitePoints();
       int getNumberOfBlackPoints();
       list<Group> *getGroupList();
};

/**
 * Class Graph
 */
class Graph {
private:
        SuperGroup **superGroups;
        int numberOfSuperGroups;
        int numberOfGroups;
        double cohesion;
        
public:
       Graph( int numberOfSuperGroups );
       ~Graph();
       
       SuperGroup *getSuperGroupAt( int index );
       void getNumberOfAllPoints();
       int getNumberOfSuperGroups();
       int getNumberOfAllBlackPoints();
       int getNumberOfAllWhitePoints();
       void superGroup( int i, int k, int **matrix );
       void mainSuperGroups( int numberOfGroups, int **matrix );
       int searchSuperGroup( int group );
       void numberCellSuperGroup( Grid *grid );
       void completeGraph( Grid *grid );
       void getCohesion( int NWhite, int NBlack );
       void setIntensity( int TOTAL, int NBlack );
       int* sortInsertionIntensity();
       double highestIntensity( int &numberOfTheBest );
       int getNMaxBlackPoints( double &scan );
       void printScan();
       void writeStatisticScan( int color );
       void print( char *fileName, Table *blackTable, Table *whiteTable);
       void printSummarySuperGroups( char *fileName );

};
