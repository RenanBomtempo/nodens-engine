/**
 * @file grid_wave.cpp
 * File where all methods related to wave equation resolution are implemented.
 */

#include "grid.h"
#include <iostream>
#include "../../models/constants.h"
#include "../../models/luo_rudy.h"
//#include "fhn.h"
#include <cmath>
#include<cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <new>
#include <map>
#include <fstream>
#include <ctime>
#include "opts.h"
#include "StopWatch.h"

using std::ofstream;
using std::cout;
using std::endl;
using std::ios;

int getRefinementLevel(double sideLen, double h) {
	int numLevel = 0;
	double aux = sideLen;

	while(aux > h) {
		numLevel++;
		aux /= 2.0;
	}

	return numLevel;

}

void Grid::initialConditionMonodomain(double initialVm, double beta, double cm, double dt)
{
	CellNode *gridCell;
	gridCell = firstCell;

	double alpha, h;

	while( gridCell != 0 )
	{
		h = gridCell->faceLength;
		alpha = ( ((beta * cm)/dt ) * UM2_TO_CM2 ) * pow(h,2.0);
		gridCell->v = initialVm;
		gridCell->b = initialVm*alpha;
		gridCell = gridCell->next;
	}
}

//Essa funcao esta tratando a construcao da matriz considerando a malha alg!!
void Grid::initializeDiagonalElementsMonodomain(double beta, double Cm, double dt)
{
	CellNode *gridCell;
	gridCell = firstCell;
	double alpha, h;

	Element *element;
	while( gridCell != 0 ) {
		h = gridCell->faceLength;
		alpha = ( ((beta * Cm)/dt ) * UM2_TO_CM2 ) * pow(h,2.0);

		if ( gridCell->active )	{
			element = new Element;
			element->column = gridCell->gridPosition;
			element->cell = gridCell;
			element->value = alpha;
			if (gridCell->firstElement) {
				Element *aux = gridCell->firstElement;
				while(aux) {
					Element *temp = aux;
					aux = aux->next;
					delete temp;
				}
			}
			gridCell->firstElement = element;
		}

		gridCell = gridCell->next;
	}
}

//Prints grid discretization matrix.
void Grid::printGridMatrix()
{
	ofstream outputFile( "outputMatrix.txt", ios::app ); //trunc

	CellNode *gridCell;
	gridCell = firstCell;
	Element *element;
	while( gridCell != 0 )
	{
		if( gridCell->active )
		{
			element = gridCell->firstElement;
			outputFile << "Row " << gridCell->gridPosition + 1 << ": ";
			while ( element != 0 )
			{
				outputFile << element->value << " (" << gridCell->gridPosition + 1
						<< "," << (element->column) + 1 << ") ";
				element = element->next;
			}
			outputFile << endl;
		}
		gridCell = gridCell->next;

	}
	outputFile << "________________________________________________________________________\n";

}

void Grid::setDiscretizationMatrixMonodomain(double beta, double cm, double dt) {

	CellNode *gridCell;
	Cell *neighborGridCell;
	char direction;
	initializeDiagonalElementsMonodomain(beta, cm, dt);

	gridCell = firstCell;
	while ( gridCell != 0 )
	{
		if ( gridCell->active )
		{
			//Computes and designates the flux due to south cells.
			neighborGridCell = gridCell->south;
			direction = 's';
			fillDiscretizationMatrixElementsMonodomain(gridCell, neighborGridCell, direction);

			//Computes and designates the flux due to north cells.
			neighborGridCell = gridCell->north;
			direction = 'n';
			fillDiscretizationMatrixElementsMonodomain(gridCell, neighborGridCell, direction);

			//Computes and designates the flux due to east cells.
			neighborGridCell = gridCell->east;
			direction = 'e';
			fillDiscretizationMatrixElementsMonodomain(gridCell, neighborGridCell, direction);

			//Computes and designates the flux due to west cells.
			neighborGridCell = gridCell->west;
			direction = 'w';
			fillDiscretizationMatrixElementsMonodomain(gridCell, neighborGridCell, direction);

			neighborGridCell = gridCell->front;
			direction = 'f';
			fillDiscretizationMatrixElementsMonodomain(gridCell, neighborGridCell, direction);

			neighborGridCell = gridCell->back;
			direction = 'b';
			fillDiscretizationMatrixElementsMonodomain(gridCell, neighborGridCell, direction);


		}

		gridCell = gridCell->next;
	}
}

//Essa funcao esta tratando a construcao da matriz considerando a malha alg!!
void Grid::fillDiscretizationMatrixElementsMonodomain(CellNode *gridCell, Cell *neighborGridCell, char direction)
{
	int position;                           //Coluna da célula vizinha.
	bool hasFound;

	TransitionNode *whiteNeighborCell;
	CellNode *blackNeighborCell;

	double sigmaX = 1.2*0.0001;
	double sigmaY = 0.6226*0.0001;
	double sigmaZ = 0.25562*0.0001;

	double sigmaX1 = (2.0 * sigmaX * sigmaX) / (sigmaX + sigmaX); //Direita ou leste
	double sigmaX2 = (2.0 * sigmaX * sigmaX) / (sigmaX + sigmaX); //Esquerda ou oeste
	double sigmaY1 = (2.0 * sigmaY * sigmaY) / (sigmaY + sigmaY); //Cima ou norte
	double sigmaY2 = (2.0 * sigmaY * sigmaY) / (sigmaY + sigmaY); //Baixo ou sul
	double sigmaZ1 = (2.0 * sigmaZ * sigmaZ) / (sigmaZ + sigmaZ); //Frente
	double sigmaZ2 = (2.0 * sigmaZ * sigmaZ) / (sigmaZ + sigmaZ); //Tras

	/* When neighborGridCell is a transition node, looks for the next neighbor
	 * cell which is a cell node. */
	//Acha uma célula real que está no caixo enviado como vizinho
	if (neighborGridCell->level > gridCell->level ) {
		if((neighborGridCell->type == 'w') ) {
			whiteNeighborCell = static_cast<TransitionNode*>(neighborGridCell);
			hasFound = false;
			while( !hasFound ) {
				if( neighborGridCell->type == 'w' ) {
					whiteNeighborCell = dynamic_cast<TransitionNode*>(neighborGridCell);
					if( whiteNeighborCell->singleConnector == 0 ) {
						hasFound = true;
					}
					else {
						neighborGridCell = whiteNeighborCell->quadrupleConnector1;
					}
				}
				else {
					break;
				}
			}

		}
	}
	//Aqui, a célula vizinha tem um nivel de refinamento menor, entao eh mais simples.
	else {
		if(neighborGridCell->level <= gridCell->level && (neighborGridCell->type == 'w') ) {
			whiteNeighborCell = static_cast<TransitionNode*>(neighborGridCell);
			hasFound = false;
			while( !hasFound ) {
				if( neighborGridCell->type == 'w' ) {
					whiteNeighborCell = dynamic_cast<TransitionNode*>(neighborGridCell);
					if( whiteNeighborCell->singleConnector == 0 ) {
						hasFound = true;
					}
					else {
						neighborGridCell = whiteNeighborCell->singleConnector;
					}
				}
				else {
					break;
				}
			}
		}
	}

	//Tratamos somente os pontos interiores da malha.
	if( ( neighborGridCell->type == 'b' ) && ( neighborGridCell->active == true ) )	{

		blackNeighborCell = static_cast<CellNode*>(neighborGridCell);
		//		cout << "Direcao: " << direction << " Celula: " << gridCell->gridPosition << " Vizinha: " << blackNeighborCell->gridPosition << endl;
		/* The following piece of code avoids double computations.*/
		//		if( (gridCell->level >= blackNeighborCell->level) &&
		//				( (gridCell->centerX >= blackNeighborCell->centerX) &&
		//						(gridCell->centerY >= blackNeighborCell->centerY) &&
		//						(gridCell->centerZ > blackNeighborCell->centerZ)
		//				)
		//		)
		//		{

		//Descobrimos a coluna que temos que preencher com o vizinho
		Element *element;
		element = gridCell->firstElement;
		position = blackNeighborCell->gridPosition;

		while( element != 0 && element->column != position ) {
			element = element->next;
		}

		//TODO: Cada elemento pode ter um sigma diferente
		if (element == NULL) {

			element = new Element;
			element->column = position;
			if (direction == 'n') {
				element->value = -sigmaY1;
				gridCell->firstElement->value += sigmaY1;
			}
			else if (direction == 's') {
				element->value = -sigmaY2;
				gridCell->firstElement->value += sigmaY2;
			}
			else if (direction == 'e') {
				element->value = -sigmaX1;
				gridCell->firstElement->value += sigmaX1;
			}
			else if (direction == 'w') {
				element->value = -sigmaX2;
				gridCell->firstElement->value += sigmaX2;
			}
			else if (direction == 'f') {
				element->value = -sigmaZ1;
				gridCell->firstElement->value += sigmaZ1;
			}
			else if (direction == 'b') {
				element->value = -sigmaZ2;
				gridCell->firstElement->value += sigmaZ2;
			}

			element->cell = blackNeighborCell;
			Element *auxiliarElement;
			auxiliarElement = gridCell->firstElement->next;
			gridCell->firstElement->next = element;
			element->next = auxiliarElement;

		}

		//preenchemos a outra parte (a matrix é simetrica)
		element = blackNeighborCell->firstElement;
		position = gridCell->gridPosition;

		while( element != 0 && element->column != position ) {
			element = element->next;
		}

		if (element == NULL) {

			element = new Element;
			element->column = position;

			if (direction == 'n') {
				element->value = -sigmaY1;
				blackNeighborCell->firstElement->value += sigmaY1;
			}
			else if (direction == 's') {
				element->value = -sigmaY2;
				blackNeighborCell->firstElement->value += sigmaY2;
			}
			else if (direction == 'e') {
				element->value = -sigmaX1;
				blackNeighborCell->firstElement->value += sigmaX1;
			}
			else if (direction == 'w') {
				element->value = -sigmaX2;
				blackNeighborCell->firstElement->value += sigmaX2;
			}
			else if (direction == 'f') {
				element->value = -sigmaZ1;
				blackNeighborCell->firstElement->value += sigmaZ1;
			}
			else if (direction == 'b') {
				element->value = -sigmaZ2;
				blackNeighborCell->firstElement->value += sigmaZ2;
			}

			element->cell = gridCell;
			Element *auxiliarElement;
			auxiliarElement = blackNeighborCell->firstElement->next;
			blackNeighborCell->firstElement->next = element;
			element->next = auxiliarElement;

		}
		//}
	}

}

void Grid::setNewSV() {
	CellNode *gridCell;
	gridCell = firstCell;

	while( gridCell != 0 )
	{
		if ( gridCell->active )
		{
			gridCell->sv[0] = gridCell->v;

		}
		gridCell = gridCell->next;
	}
}

void Grid::setInitalConditionsODEs()
{
	CellNode *gridCell = firstCell;
	while(gridCell != 0) {
		setIC_ode_cpu(gridCell->sv);
		gridCell = gridCell->next;
	}
}


void Grid::solveODEs(double time, double stimStart, double stimDur,
		double beta, double cm, double dt, double i_stim) {

	CellNode *gridCell;
	gridCell = firstCell;
	double alpha, h;
	bool stim = false;

	while( gridCell != 0 ) {

		if ( gridCell->active )	{
			h = gridCell->faceLength;
			alpha = ( ((beta * cm)/dt ) * UM2_TO_CM2 ) * pow(h,2.0);

			stim = (time >= stimStart) && (time <= stimStart + stimDur);

			stim &= (gridCell->centerX >= (sideLength/2.0) - 400.0) && (gridCell->centerX <= (sideLength/2.0) + 400.0);
			stim &= (gridCell->centerY >= (sideLength/2.0) - 400.0) && (gridCell->centerY <= (sideLength/2.0) + 400.0);
			stim &= (gridCell->centerZ >= (sideLength/2.0) - 400.0) && (gridCell->centerZ <= (sideLength/2.0) + 400.0);

			if (stim) {
				solve_ode_cpu(dt, gridCell->sv, i_stim);
			}
			else {
				solve_ode_cpu(dt, gridCell->sv, 0.0);
			}

			gridCell->b = gridCell->sv[0]*alpha;

		}
		gridCell = gridCell->next;
	}
}

/*______________________________________________________________________________

                        FUNCTION conjugateGradient()

    Conjugate gradient method for symmetric positive definite matrix.
    The grid discretization matrix and all variables used in the conjugate
    gradient method are stored on the grid's cells, which improves performance.
    There is no independent linear algebra package.

______________________________________________________________________________*/
void Grid::conjugateGradient(int maxIts)
{
	double 	error = 0.0,
			rTr,
			r1Tr1,
			pTAp,
			alpha,
			beta,
			//normR = 0.0,
			normPP1 = 0.0,
			precision = 1e-8; //Maximum precision which error must not exceed.

	//int maximumNumberOfIterations = 41;
	int maximumNumberOfIterations = maxIts;

	pError = 1.0;
	numberOfIterations = 0;

	CellNode *gridCell;
	Element *element;

	//__________________________________________________________________________
	//Computes vector A*x, residue r = b - Ax, scalar rTr = r^T * r and
	//sets initial search direction p.
	gridCell = firstCell;
	rTr = 0.0;
	while( gridCell != 0 )
	{
		if( gridCell->active )
		{
			gridCell->Ax = 0.0;
			element = gridCell->firstElement;
			while( element != 0 )
			{
				gridCell->Ax += element->value * element->cell->v;
				element = element->next;
			}
			gridCell->r = gridCell->b - gridCell->Ax;
			rTr += gridCell->r * gridCell->r;
			gridCell->p = gridCell->r;
			//if( fabs(gridCell->r) > error )
			//error = fabs(gridCell->r);
			error = rTr;
			pError = error;
		}
		gridCell = gridCell->next;
	}
	//__________________________________________________________________________
	//Conjugate gradient iterations.
	if( error >= precision )
	{
		while( numberOfIterations < maximumNumberOfIterations )
		{
			pError = 0.0;
			normPP1 = 0.0;
			//__________________________________________________________________
			// Computes Ap and pTAp. Uses Ax to store Ap.
			gridCell = firstCell;
			pTAp = 0.0;
			while( gridCell != 0 )
			{
				if( gridCell->active )
				{
					gridCell->Ax = 0.0;
					element = gridCell->firstElement;
					while( element != 0 )
					{
						gridCell->Ax += element->value * element->cell->p;
						element = element->next;
					}
					pTAp += gridCell->p * gridCell->Ax;
				}
				gridCell = gridCell->next;
			}
			//__________________________________________________________________
			// Computes alpha.
			alpha = rTr/pTAp;
			//__________________________________________________________________
			// Computes new value of solution: u = u + alpha*p.
			gridCell = firstCell;
			while( gridCell != 0 )
			{
				if( gridCell->active )
					gridCell->v += alpha * gridCell->p;
				gridCell = gridCell->next;
			}
			//__________________________________________________________________
			//Upgrades residue r1 = r - alpha*Ap and computes r1Tr1 = r1^T * r1.
			gridCell = firstCell;
			r1Tr1 = 0.0;
			while( gridCell != 0 )
			{
				if( gridCell->active )
				{
					gridCell->r -= alpha * gridCell->Ax;
					r1Tr1 += gridCell->r * gridCell->r;
				}
				gridCell = gridCell->next;
			}
			//__________________________________________________________________
			//Computes beta.
			beta = r1Tr1/rTr;
			// ROD
			//normR = r1Tr1;
			pError = r1Tr1;
			numberOfIterations++;
			if( pError <= precision ) {
				break;
			}
			//__________________________________________________________________
			//Computes vector p1 = r1 + beta*p and uses it to upgrade p.
			gridCell = firstCell;
			while( gridCell != 0 )
			{
				if( gridCell->active )
				{
					gridCell->p1 = gridCell->r + beta * gridCell->p;
					gridCell->p = gridCell->p1;
				}
				gridCell = gridCell->next;
			}

			//if( normR < precision )
			//	pError = 0.0;
			//else if( normR > pError )
			//	pError = normPP1;
			//__________________________________________________________________
			//Upgrades rTr.
			rTr = r1Tr1;

		}

	}//end of conjugate gradient iterations.
}//end conjugateGradient() function.

void Grid::printOrdered(string outfile) {
	ofstream outputFile( outfile.c_str(), ios::trunc ); //trunc

	CellNode *gridCell;
	gridCell = firstCell;

	while( gridCell != 0 ) {
		outputFile << gridCell->centerX << "," << gridCell->centerY << "," << gridCell->centerZ << ","<< gridCell->halfFaceLength << "," << gridCell->v << endl;
		gridCell = gridCell->next;
	}

}//end print() function.

void Grid::solveMonodomain( int argc, char** argv  ) {

	parseOptions(argc, argv);


	double initialV;
	bool redoMatrix = false;


	//Inicializa o grid
	sideLength = globalArgs.sideLenght;
	initializeGrid();

	int max_its;
	int count = 0;
	int print_rate = globalArgs.print_rate;

	int maximumRefinementLevel = getRefinementLevel(sideLength, globalArgs.min_h);
	int maximumDerefinementeLevel = getRefinementLevel(sideLength, globalArgs.max_h);

	double refinementBound = globalArgs.ref_bound,
	derefinementBound = globalArgs.deref_bound;

	bool adaptive = globalArgs.adaptative;
	bool saveToFile = globalArgs.saveToFile;
    

	int initRef = getRefinementLevel(sideLength, globalArgs.start_h)-1;

	for( int i = 0; i < initRef; i++ ) {
		refineAll();
    }


	double beta = 0.14, cm = 1.0;

	double h = (double)(sideLength) / (double)(pow(numberOfCells,1.0/3.0));

	double deltaT = globalArgs.dt;
	double finalT = globalArgs.final_time;
	double stim_start = 0.0;
	double stim_dur = globalArgs.stim_dur;
	double i_stim;


	setInitalConditionsODEs();


	initialV = firstCell->sv[0];
	if(isinf(globalArgs.stim_cur)) {
		i_stim = initialV;
	}
	else {
		i_stim = globalArgs.stim_cur;
	}

	
    clock_t mat_init, mat_final;

	mat_init = clock();

	initialConditionMonodomain( initialV, beta, cm, deltaT );
	setDiscretizationMatrixMonodomain( beta, cm, deltaT );


	mat_final = clock() - mat_init;
	cout << "Creating Matrix time: " << (double) mat_final/(double)CLOCKS_PER_SEC << " s" << endl;


	if(globalArgs.max_its > 0) {
		max_its = globalArgs.max_its;
	}
	else {
		max_its = numberOfCells;
	}

    printf("System parameters: \n");


	printf("Time discretization: %lf\n", deltaT);
	printf("Initial V: %lf\n", initialV);
	printf("Initial Space Discretization: %lf um\n", h);

	if(adaptive) {
		printf("Initial Refinement Level: %d\n", getRefinementLevel(sideLength, globalArgs.start_h));
		printf("Minimum Space Discretization: %lf um\n", globalArgs.min_h);
		printf("Minimum Refinement Level: %d\n", getRefinementLevel(sideLength, globalArgs.max_h));
		printf("Maximum Space Discretization: %lf um\n", globalArgs.max_h);
		printf("Maximum Refinement Level: %d\n", getRefinementLevel(sideLength, globalArgs.min_h));
	}

	printf("Width = %lf um, height = %lf um \n", sideLength, sideLength);
	printf("Volumes X = %d, Volumes Y = %d \n", int(ceil(sideLength/h)), int(ceil(sideLength/h)));
	printf("Initial N. of Elements = %d\n", numberOfCells);
	printf("PDE time step = %lf\n", deltaT);
	printf("Simulation Final Time = %lf\n", finalT);
	printf("Print Rate = %d\n", print_rate);
	printf("Stimulus start = %lf\n", stim_start);
	printf("Stimulus duration = %lf\n", stim_dur);
	printf("Stimulus value = %lf\n", i_stim);
	printf("Maximum CG iterations = %d\n", max_its);
	if (adaptive) {
		printf("Refinement Bound = %lf\n", refinementBound);
		printf("Derefinement Bound = %lf\n", derefinementBound);
		printf("Using adaptativity\n");
	}
	if (saveToFile) {
#ifdef VTK
		printf("Saving to VTK output in %s dir\n", globalArgs.outDirName);
#else
		printf("Saving to plain text output in %s dir\n", globalArgs.outDirName);
#endif
	}
	else {
		printf("The solution will not be saved\n");
	}


	fflush(stdout);

	
	fflush(stdout);

	StopWatch solver_time, ode_time, cg_time, ref_time, deref_time, order_time;


	long total_write_time = 0, total_mat_time = 0, total_ref_time = 0,
			ode_total_time = 0, cg_total_time=0, total_cg_it=0,
			total_deref_time = 0, total_ordering_time=0, aux;
	StopWatch part_solver, part_mat, write_time;
	int num_deref, num_ref;


	solver_time.start();

	for( double t = 0.0; t < finalT; t += deltaT )	{

		if(saveToFile) {
			if(count % print_rate == 0) {
#ifdef TIMING
				write_time.start();
#endif
#ifdef VTK
				stringstream file_name (stringstream::in | stringstream::out);
				file_name << globalArgs.outDirName << "/V_t_" << setw(10) << setfill('0') <<  count << ".vtu";
				cria_vtk_xml_alg(this, file_name.str());
#else
				std::stringstream file_name2 (std::stringstream::in | std::stringstream::out);
				file_name2 << globalArgs.outDirName << "/V_t_" << std::setw(10) <<  std::setfill('0') <<  count;
				printOrdered(file_name2.str());
#endif
#ifdef TIMING
				total_write_time += write_time.stop();
#endif
			}
		}

		count++;

		if (t > 0.0) {
			setNewSV();
		}

		ode_time.start();
		solveODEs(t, stim_start, stim_dur, beta, cm, deltaT, i_stim);
		ode_total_time += ode_time.stop();

		cg_time.start();
		conjugateGradient(max_its);
		cg_total_time += cg_time.stop();
		total_cg_it += numberOfIterations;


		if(count % print_rate == 0) {
			printf("t = %lf, Iterations = %d, Error Norm = %e, Number of Cells: %lld\n", t, numberOfIterations, pError, numberOfCells);
		}

		if (adaptive) {

			if (t > (stim_dur/2.0) ) {
				ref_time.start();
				redoMatrix = refine( maximumRefinementLevel, refinementBound, num_ref );
				aux = ref_time.stop();
				printf("REFINED: %d, %llu., %ld\n", num_ref, numberOfCells, aux);
				total_ref_time += aux;

				deref_time.start();
				redoMatrix |= derefine( derefinementBound, maximumDerefinementeLevel, num_deref );
				aux = deref_time.stop();
				printf("DEREFINED: %d, %llu., %ld\n", num_deref, numberOfCells, aux);
				total_deref_time += aux;

				order_time.start();
				orderGridCells();
                aux = order_time.stop();
                printf("ORDER: %llu, %ld, \n", numberOfCells, aux);
				total_ordering_time += aux;

			}

			if(redoMatrix) {
				part_mat.start();
				setDiscretizationMatrixMonodomain( beta, cm, deltaT );
				total_mat_time += part_mat.stop();

			}

		}

	}

	printf("Resolution Time: %ld us\n", solver_time.stop());;
	printf("ODE Total Time: %ld us\n", ode_total_time);
	printf("CG Total Time: %ld us\n", cg_total_time);
	printf("Mat time: %ld us\n", total_mat_time);
	printf("Ref time: %ld us\n", total_ref_time);
	printf("Deref time: %ld us\n", total_deref_time);;
	printf("Ordering time: %ld us\n", total_ordering_time);

	printf("CG Total Iterations: %ld\n", total_cg_it);

}
