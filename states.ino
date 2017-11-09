//STATE Functions

void state_Initialize(void) {
	//INITIALIZE State (0)
	PrevState = CurrentState;
	StateStartTime = millis();
	if (AbortPB > 0) {	//Monitor for Abort button press.
		AbortPB = 0;
		CurrentState = ABORT;
		return;
	}
	else if (ResetPB > 0) {	//Wait for Reset PB press before state change.
		ResetPB = 0;
		CurrentState = RESET;
	}
	return;
}

void state_Idle(void) {
	//IDLE State (1)
	PrevState = CurrentState;
	StateStartTime = millis();
	if (AbortPB > 0) {
		AbortPB = 0;
		CurrentState = ABORT;
		return;
	}
	else if (StartPB > 0) {
		StartPB = 0;
		CurrentState = STARTUP;
		return;
	}
	return;
}

void state_LoadSettings(void) {
	//LOAD SETTINGS State (2)
	//Read settings from EEPROM and save to structure variables.
	//Not currently used, values hard coded.
	PrevState = CurrentState;
	StateStartTime = millis();
	if (AbortPB > 0) {
		AbortPB = 0;
		CurrentState = ABORT;
		return;
	}
	//Timeout Fault Handler
	if ((millis() - StateStartTime) > k_10sTimeOut) {	
		FaultedState = CurrentState;
		FaultMsg = 12;	//Load settings timeout
		Fault = TRUE;
		CurrentState = FAULT;
		return;
	}
	CurrentState = STARTUP;
	return;
}

void state_Startup(void) {
	PrevState = CurrentState;
	//StateStartTime = millis();
	if (AbortPB > 0) {
		AbortPB = 0;
		CurrentState = ABORT;
		return;
	}
	//Timeout Fault Handler
	//if ((millis() - StateStartTime) > k_10sTimeOut) {
	//	FaultedState = CurrentState;
	//	FaultMsg = 12;	//Load settings timeout
	//	Fault = TRUE;
	//	CurrentState = FAULT;
	//	return;
	//}
	CurrentState = CAPTURE_FLOW_PRESS;
	return;
}





void state_Abort(void) {
	CurrentState = ABORT;
	if (ResetPB > 0) {
		PrevState = CurrentState;
		ResetPB = 0;

	}
}

