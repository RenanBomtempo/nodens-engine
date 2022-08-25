#ifndef __STOPWATCH_H
#define __STOPWATCH_H

#include <sys/time.h>

/* simple stopwatch class */
class StopWatch {
private:
    struct timeval tv;
    bool running;

public:
    StopWatch(void);
    void start(void); /* start the stopwatch */
    long stop(void); /* stop the stopwatch and get the value in usecs */
};

#endif /* __STOPWATCH_H */
