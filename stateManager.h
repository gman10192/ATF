// stateManager.h

//STATE MANAGER
//Based on PackML v3.0 Mode & States document.

//Used tutorial from: http://www.cplusplus.com/doc/tutorial/classes/ 


#ifndef _STATEMANAGER_h
#define _STATEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class StateManagerClass
{
	static byte _requestedState, _currentState, _previousState;
	bool stateStarting(void);
	bool stateResetting(void);
	bool stateStopping(void);
	bool stateWaiting(void);
	bool stateHolding(void);
	bool stateUnHolding(void);
	bool statePausing(void);
	bool stateUnPausing(void);
	bool stateCompleting(void);
	bool stateAborting(void);



protected:


public:
	//StateManagerClass(byte, byte, byte);
	StateManagerClass();
	void update(void);
	void transitionTo(byte);
	void immediateTransitionTo(byte);
	byte getCurrentState(void);
	bool isInState(byte);
};

extern StateManagerClass StateManager;

#endif

