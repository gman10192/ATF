// stateManager.h

//STATE MANAGER
//Based on PackML v3.0 Mode & States document.


#ifndef _STATEMANAGER_h
#define _STATEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class StateManagerClass
{
 protected:


 public:
	void init();
	void update(void);
	void transitionTo(byte _requestedState);
	void immediateTransitionTo(byte _requestedState);
	byte getCurrentState(void);
	bool isInState(byte _currentState)


};

extern StateManagerClass StateManager;

#endif

