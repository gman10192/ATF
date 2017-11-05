////FUNCTION*******************************************************************************************
void stateStarting(void)
{
	
	int pIn = 0, pOut = 0, fIn = 0, fOut = 0;
	int i = 0;
	int starttime = 0;

		
	//Step 1: Set desired volume to be exchanged
	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 1);  //Prompt to set the exchange volume
	starttime = millis();
	while (gExchVolumeSetpointTemp <= 0) {
		genie.DoEvents();
		delay(50);
		if ((millis() - starttime) > gcTimeOut) {  //ERROR: user input timeout, bail out of fucntion
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
	starttime = millis();
	while ((pIn < gcMinStartPress && pOut < gcMinStartPress)) {
		pIn = getAnalogData(gcPressInSSPin);
		delay(50);
		pOut = getAnalogData(gcPressInSSPin);
		delay(50);
		if ((millis()-starttime) > gcTimeOut) {  //ERROR: pressure detection timeout, bail out of fucntion
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

