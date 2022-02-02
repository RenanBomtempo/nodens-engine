/* PROGRAMA: main.cpp                                  */
/* AUTORA  : DENISE BURGARELLI DUCZMAL                 */
/* DATA    : 96/08/14                                  */
/* MODIFICAÇÃO: OPENGL E C++ EM ABRIL DE 2004          */
/*              DENISE BURGARELLI DUCZMAL              */
/*              RODNEY JOSUÉ BIEZUNER                  */
/*******************************************************/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h> 
#include "funcoes.h"

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;
        
#include <fstream>
    using std::ofstream;



#define null 0
/*************variaveis externas ***************************** */
int flagpeano = 0;
int flagglobal = 0;
int flagproblem;
int flagonda_inicio=0;
double cotaref=1.0; //3.0
double cotades=1.0; //3.0
double cotalaplace=0.0001;//0.1
double cotadeslap=0.0008;//0.06;
double cotaref_term=10.0;
double cotades_term=10.0;
	
int main()
{
    flagproblem = 3;      //Escolhe o problema a ser rodado 
                   //            Laplace [flagproblem = 0], 
                   //               Onda [flagproblem = 1] 
                   //              Calor [flagproblem = 2]
                   //      Termoacústico [flagproblem = 3]
    Dchange_peano(0);      //Mostra (1) ou nao (0) a curva de Peano
    init();        
    play(flagproblem);
    
    
    cout << "End of Program." << endl;
    
    system("pause"); 
    return 0;
    
}
