/*
 * opts.h
 *
 *  Created on: 27/05/2011
 *      Author: sachetto
 */

#ifndef OPTS_H_
#define OPTS_H_

#include <getopt.h>
#include<cstdlib>
#include <cstdio>

struct globalArgs_t {
	bool saveToFile;				/* -s option */
	double stim_dur;				/* -d option */
	double final_time;				/* -f option */
	double sideLenght;			    /* -l option */
	const char *outDirName;	        /* -o option */
	bool adaptative;	            /* -a option */
	int print_rate;	            	/* -p option */
	double start_h;					/* -i option*/
	double min_h;					/* -m option*/
	double max_h;					/* -x option*/
	int max_its;					/* -t option*/
	double ref_bound;				/* -r option*/
	double deref_bound;				/* -b option*/
	double stim_cur;				/* -c option*/
	double dt;						/* -z option*/



} globalArgs;

static const struct option longOpts[] = {
		{ "save-to-file", no_argument, NULL, 's' },
		{ "stim-duration", required_argument, NULL, 'd' },
		{ "side-lenght", required_argument, NULL, 'l' },
		{ "out-dir", required_argument , NULL, 'o' },
		{ "adaptative", no_argument, NULL, 'a' },
		{ "print-rate",required_argument , NULL, 'p' },
		{ "start-h", required_argument, NULL, 'i' },
		{ "min-h", required_argument, NULL, 'm' },
		{ "max-h", required_argument, NULL, 'x' },
		{ "max-cg-its", required_argument, NULL, 't' },
		{ "refinement-bound", required_argument, NULL, 'r' },
		{ "derefinement-bound", required_argument, NULL, 'b' },
		{ "stim-current", required_argument, NULL, 'c' },
		{ "dt", required_argument, NULL, 'z' },
		{ "help", no_argument, NULL, 'h' },
		{ NULL, no_argument, NULL, 0 }
};

static const char *optString = "z:c:r:b:t:i:m:x:sd:f:p:l:o:ah?";

/* Display program usage, and exit.
 */
void display_usage( char** argv ) {

	printf("Usage: %s [options]\n\n", argv[0]);
	printf("Options:\n");
	printf("--save-to-file, -s. No argument required. Save the output. Default No save.\n");
	printf("--stim-duration, -d [stim_dur]. Stimulus duration (ms). Default: 2 ms \n");
	printf("--side-lenght, -l [length]. Tissue side lenght (micro m). Default: 12800 micro m \n");
	printf("--out-dir, -o [output-dir]. Simulation output directory. Default: ./ \n");
	printf("--adaptative, -a. No argument required. Use adaptativity. Default No use.\n");
	printf("--print-rate, -p [output-print-rate]. Output print rate (in number of iterations). Default: 1 \n");
	printf("--start-h, -i [start-h]. Initial space discretization (micro m). Default: 12.5 micro m \n");
	printf("--min-h, -m [min-h].Minimum space discretization (micro m). Default: 12.5 micro m \n");
	printf("--max-h, -x [max-h].Maximum space discretization (micro m). Default: 200.0 micro m \n");
	printf("--max-cg-its, -t [max-its]. Maximum number of CG iterations. Default: number of volumes \n");
	printf("--refinement-bound, -r [ref-bound]. ALG refinement bound (flux). Default: 0.34 \n");
	printf("--derefinement-bound, -b [deref-bound]. ALG derefinement bound (flux). Default: 0.25 \n");
	printf("--stim-current, -c [stim-cur]. Stimulus current. Default: Cell model Vm \n");
	printf("--dt, -z [dt]. Simulation time discretization. Default: 0.01 \n");
	printf("--help, -h. Shows this help and exit \n");
	exit( EXIT_FAILURE );
}

void parseOptions(int argc, char**argv) {
	int opt = 0;

	/* Initialize globalArgs before we get to work. */
	globalArgs.saveToFile = false;
	globalArgs.outDirName = "./";
	globalArgs.sideLenght = 12800.0;
	globalArgs.adaptative = false;
	globalArgs.final_time = 10.0;
	globalArgs.stim_dur = 2.0;
	globalArgs.print_rate = 1;
	globalArgs.start_h = 12.5;
	globalArgs.min_h = 12.5;
	globalArgs.max_h = 200.0;
	globalArgs.max_its = 0;
	globalArgs.ref_bound = 0.34;
	globalArgs.deref_bound = 0.25;
	globalArgs.stim_cur = INFINITY;
	globalArgs.dt = 0.01;

	int longIndex;

	opt = getopt_long( argc, argv, optString, longOpts, &longIndex );

	while( opt != -1 ) {
		switch( opt ) {

		case 't':
			globalArgs.max_its = atoi(optarg);
			break;

		case 's':
			globalArgs.saveToFile = true;
			break;

		case 'l':
			globalArgs.sideLenght = atof(optarg);
			break;

		case 'p':
			globalArgs.print_rate = atoi(optarg);
			break;

		case 'o':
			globalArgs.outDirName = optarg;
			break;

		case 'f':
			globalArgs.final_time = atof(optarg);
			break;

		case 'i':
			globalArgs.start_h = atof(optarg);
			break;

		case 'm':
			globalArgs.min_h = atof(optarg);
			break;

		case 'x':
			globalArgs.max_h = atof(optarg);
			break;

		case 'r':
			globalArgs.ref_bound = atof(optarg);
			break;

		case 'b':
			globalArgs.deref_bound = atof(optarg);
			break;

		case 'd':
			globalArgs.stim_dur= atof(optarg);
			break;
		case 'a':
			globalArgs.adaptative = true;
			break;
		case 'c':
			globalArgs.stim_cur= atof(optarg);
			break;
		case 'z':
			globalArgs.dt = atof(optarg);
			break;
		case 'h':	/* fall-through is intentional */
		case '?':
			display_usage(argv);
			break;

		default:
			/* You won't actually get here. */
			break;
		}

		opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	}
}

#endif /* OPTS_H_ */
