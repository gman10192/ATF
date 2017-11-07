//Functions to be  completed when the system is RESETTING
//Returns a TRUE when completed
void stateResetting(void) {

	digitalWrite(gcBypassCVPin, HIGH);  //Activate bypass loop valve
	digitalWrite(gcReverseFlowCVPin, LOW);	//Flow control set to normal (non-reversed)
	sendAnalogData(0, gcPressRegSSPin); //Disable pressure regulator for tank
	digitalWrite(gcVentTankCVPin, LOW);	//Open tank vent
	delay(150);
	
	if (verifyNoFlowNoPressure() == TRUE) {
		StateManager.transitionTo(IDLE);	//This signifies the system was Reset sucessfully, therefore a request to transition to the IDLE state is made.
	}
	else {
		//ERROR Handler
		gFault = TRUE;
		gFaultMessage = 10; //Unable to reset the system
		return;
		
	}
	return;
}
