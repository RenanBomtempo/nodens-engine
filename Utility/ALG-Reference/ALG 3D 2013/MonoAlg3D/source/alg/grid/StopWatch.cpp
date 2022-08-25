/*
 * StopWatch.cpp
 *
 *  Created on: 29/07/2011
 *      Author: sachetto
 */

#include "StopWatch.h"

#include <sys/time.h>
#include <cstdlib>
#include <cstdio>

StopWatch::StopWatch(void) {
	running = false;
}

void StopWatch::start(void) {
	if (gettimeofday(&tv, NULL) < 0) {
		perror("gettimeofday");
		return;
	}
	running = true;
}

long StopWatch::stop(void) {

    timeval tv_stop;

	if (!running) {
		fprintf(stderr,"Stopwatch already not running.");
		return -1;
	}

	if (gettimeofday(&tv_stop, NULL) < 0) {
		perror("gettimeofday");
		return -1;
	}

	running = false;
	return ((tv_stop.tv_sec - tv.tv_sec) * 1000000
				+ tv_stop.tv_usec - tv.tv_usec);
}
