//stateManager.cpp

//STATE MANAGER
//Based on PackML v3.0 Mode & States document.

//Used tutorial from: http://www.cplusplus.com/doc/tutorial/classes/ 
 
 

#include "stateManager.h"

//StateManagerClass::StateManagerClass(byte a, byte b, byte c) : _requestedState(a), _currentState(b), _previousState(c) {}
	//Class constrcutor, only executed once upon the creation of an object of this class type.
	//i.e.  StateManagerClass stateManager(0,0,0)

StateManagerClass::StateManagerClass() {
	//Class default constrcutor, only executed once upon the creation of an object of this class type and only used if no values are sent when creating the object
	//i.e. StateManagerClass stateManager{}
	_requestedState = 0;
	_currentState = ABORTED;
	_previousState = 0;
}


void StateManagerClass::transitionTo(byte _tempState){
	//Accepts request to transition to the specified state and determines if that is acceptable and then allows the state change.
	//**NOTE**: Only code to verify it is approrpriate to transition to requested state should be here.
	//Actual actions and machine operation will be handled by state modules: functions for the corresponding state (i.e. stateStarting, stateResetting, etc.)
	//Any faults generated by an attempt to transition from one state to another should be handled in the individual state modules.

	_requestedState = _tempState;
	
	switch (_requestedState) {
	case IDLE:
		if (_previousState == RESETTING && gResetComplete == TRUE) {
			_previousState = _currentState;
			_currentState = IDLE;
			genie.WriteObject(GENIE_OBJ_STRINGS, 0, IDLE);
			gResetComplete = FALSE;
		}
		else {
			gFault = TRUE;
			gFaultMessage = 6;  //Unable to enter requested IDLE state
		}
		break;
	case STARTING:
		if (_currentState == IDLE) {
			_previousState = _currentState;
			_currentState = STARTING;
			genie.WriteObject(GENIE_OBJ_STRINGS, 0, STARTING);
			stateStarting();
		}
		else
			_currentState = _currentState;
		break;
	case EXECUTE:
		break;
	case COMPLETING:
		break;
	case COMPLETE:
		break;
	case HOLDING:
		break;
	case HELD:
		break;
	case UNHOLDING:
		if (_currentState == STARTING) {
			break;
		}
		else if () {
		}
		break;
	case PAUSING:
		break;
	case PAUSED:
		break;
	case UNPAUSING:
		break;
	case RESETTING:
		if (_currentState == MANUAL_MODE || _currentState == CONFIGURATION || _currentState == STOPPED) {
			_previousState = _currentState;
			_currentState = RESETTING;
			genie.WriteObject(GENIE_OBJ_STRINGS, 0, RESETTING);
			stateResetting();
		}
		else {
			gFault = TRUE;
			gFaultMessage = 7;  //Unable to enter requested RESETTING state
		}
		break;
	case STOPPED:
		break;
	case STOPPING:
		break;
	case ABORTED:
		break;
	case ABORTING:
		break;
	case CONFIGURATION:  //Enter Settings screen
		if (_currentState == IDLE) {
			genie.WriteObject(GENIE_OBJ_FORM, 3, 0);  //Activate Settings screen on the display
			_currentState = CONFIGURATION;
			genie.WriteObject(GENIE_OBJ_STRINGS, 0, CONFIGURATION);
		}
		break;
	case MANUAL_MODE:
		if (_currentState == IDLE) {
			genie.WriteObject(GENIE_OBJ_FORM, 4, 0);  //Activate manual mode screen on the display
			_currentState = MANUAL_MODE;
			genie.WriteObject(GENIE_OBJ_STRINGS, 0, MANUAL_MODE);
		}
		break;
	case FAULTED:  //Not sure if I need this state in here or not...
		break;
	case WAITING:
		break;
	default:
		gFaultMessage = 5;
		_currentState = FAULTED;
		break;
	}
}

void StateManagerClass::immediateTransitionTo(byte _requestedState){
	//Force state transitioto the state indicated, to heck with it all
}

byte StateManagerClass::getCurrentState(void){
	//Request the current state of the system - returns a byte
	
	return byte();
}

bool StateManagerClass::isInState(byte _instate)
{
	if (_instate == _currentState){return TRUE;}
	else{return FALSE;}
}

void StateManagerClass::update(void) {
	//Tell the state manager to update its self

}

bool StateManagerClass::stateStarting(void) {
	//Acting State: STARTING (2) Function for starting system.
	//NEED TO REWRITE FOR INCLUSION IN THE STATEMANAGER CLASS
	int _pIn = 0, _pOut = 0, _fIn = 0, _fOut = 0;
	int _starttime = 0;
	//Step 1: Set desired volume to be exchanged
	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 1);  //Prompt to set the exchange volume
	_starttime = millis();
	while (gExchVolumeSetpointTemp <= 0) {
		genie.DoEvents();
		delay(50);
		if ((millis() - _starttime) > gcTimeOut) {  //ERROR: user input timeout, bail out of fucntion
			gFault = TRUE;
			gFaultMessage = 9; //User input timeout
			break;
			return;
		}
	}
	gExchVolumeSetpoint = gExchVolumeSetpointTemp;
	//Step 2:  Connect & start vehichle, wait for pressure IN & OUT readings
	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 3);  //Prompt to connect and start vehicle
	while (gResumeButtonState != TRUE) {
		genie.DoEvents();
		delay(50);
	}
	gResumeButtonState = FALSE;
	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 4);  //Waiting for system pressure
	_starttime = millis();
	while ((_pIn < gcMinStartPress && _pOut < gcMinStartPress)) {
		_pIn = getAnalogData(gcPressInSSPin);
		delay(50);
		_pOut = getAnalogData(gcPressInSSPin);
		delay(50);
		if ((millis() - _starttime) > gcTimeOut) {  //ERROR: pressure detection timeout, bail out of fucntion
			gFault = TRUE;
			gFaultMessage = 8; //Pressure not detected.
			break;
			return;
		}
	}
		//Step 3: Startup complete, move on to EXECUTE state & sequence
	transitionTo(EXECUTE);
	return;
}




StateManagerClass StateManager;

