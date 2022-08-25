#include <iostream>

    using std::cin;
    using std::cout;
    using std::endl;
	
#include "Grid.h"
#include "PerfTimer.h"
	
int main()
{
    CPerfTimer time;
    time.Start();    
    
    Grid *grid;
    grid = new Grid;
    grid->solveLaplace( false );
    
    time.Stop();    
    
    grid->print();
    
    
    cout << "Tempo de Processamento (segundos): " << time.Elapsedms()/1000 << endl;
    cout << "\nEnd of program.\n" << endl;
    
    system("pause"); 
    return 0;
    
}
