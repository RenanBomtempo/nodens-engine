#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../controller.h"

/*==============================================================================
                                   Keyboard
                                    
    This class is singleton. It means that it is impossible to create more than
    one object of this class.
==============================================================================*/
class Keyboard {
private:
    static Keyboard *keyboardInstance;
    Keyboard(){}
    Keyboard(Keyboard &){}
	Keyboard& operator=(Keyboard &);
	
	static Controller *controller;
    
public:
	static Keyboard *createKeyboard( Controller *controller );
    static void execute( unsigned char, int, int );  
	static void executeSpecialKey( int, int, int ) throw (Exception);
	static void printCommands();
};

#endif
