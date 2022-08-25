#ifndef DEMO_H
#define DEMO_H

#include "../../ALG/Exceptions/exceptions.h"

class Controller;

/*==============================================================================
                                    Demo
==============================================================================*/
class Demo {
private:
	// Statements to make this class be singleton.	
	static Demo *demoInstance;	
	Demo(): running(false), currentState(0) {}
	Demo( Demo& ){}
	Demo& operator = (Demo &);	
	
	// Poiter to the interface controller.
	Controller *controller;
	
	// Keeps the current state of the object Demo.
	bool running;
	short currentState;

public:
	static Demo *createDemo( Controller *controller );

	void start();
    void stop();
	void pause();
	void restart();
    void play() throw( Exception );
    void replay();
    bool isRunning();
};

#endif
