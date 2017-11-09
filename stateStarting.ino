////FUNCTION*******************************************************************************************
//CREATED AS A FUNCTION WITHIN THE STATEMANAGER CLASS
//void stateStarting(void)
//{
//	
//	int _pIn = 0, _pOut = 0, _fIn = 0, _fOut = 0;
//	int _starttime = 0;
//
//		
//	//Step 1: Set desired volume to be exchanged
//	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 1);  //Prompt to set the exchange volume
//	_starttime = millis();
//	while (gExchVolumeSetpointTemp <= 0) {
//		genie.DoEvents();
//		delay(50);
//		if ((millis() - _starttime) > k_10sTimeOut) {  //ERROR: user input timeout, bail out of fucntion
//			gFault = TRUE;
//			FaultMsg = 9; //User input timeout
//			break;
//			return;
//		}
//	}
//	VolumeSP = gExchVolumeSetpointTemp;
//	
//	//Step 2:  Connect & start vehichle, wait for pressure IN & OUT readings
//	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 3);  //Prompt to connect and start vehicle
//	while (gResumeButtonState != TRUE) {
//		genie.DoEvents();
//		delay(50);
//	}
//	gResumeButtonState = FALSE;
//	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 4);  //Waiting for system pressure
//	_starttime = millis();
//	while ((_pIn < k_MinStartPress && _pOut < k_MinStartPress)) {
//		_pIn = getAnalogData(pin_PressInSS);
//		delay(50);
//		_pOut = getAnalogData(pin_PressInSS);
//		delay(50);
//		if ((millis()-_starttime) > k_10sTimeOut) {  //ERROR: pressure detection timeout, bail out of fucntion
//			gFault = TRUE;
//			FaultMsg = 8; //Pressure not detected.
//			break;
//			return;
//		}
//	}
//	
//	//Step 3: Startup complete, move on to EXECUTE state & sequence
//	transitionTo(EXECUTE);
//	return;
//}

