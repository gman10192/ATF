//Functions to be  completed when the system is RESETTING
//Returns a TRUE when completed
void stateResetting(void) {

	digitalWrite(gcBypassCVPin, HIGH);  //Activate bypass loop valve
	digitalWrite(gcReverseFlowCVPin, LOW);	//Flow control set to normal (non-reversed)
	sendAnalogData(0, gcPressRegSSPin); //Disable pressure regulator for tank
	digitalWrite(gcVentTankCVPin, LOW);	//Open tank vent
	delay(150);
	if (verifyNoFlowNoPressure() == TRUE) {
		transitionTo(IDLE);	//This signifies the system was Reset sucessfully, therefore a request to transition to the IDLE state is made.
		gResetComplete = TRUE;
	}
	else {
		return;
		//ERROR Handler?
	}
	return;
}
