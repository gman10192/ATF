////FUNCTION*******************************************************************************************
float flowPulse(short inputpin, int capturepulses)
{
	//FUNCTION: Capture pulses and convert to flow rate.
	//			Uses a FIXED number of samples (capturepulses).
	//INPUTS:	1) Pin number of the pulse input from the flow meter
	//			2) Number of pulses to capture the flow data, in whole numbers 
	//OUTPUT:	1) flow rate (in quarts per minute)

	float result;
	double timestart = 0, timeend = 0;


	attachInterrupt(digitalPinToInterrupt(inputpin), pulse, CHANGE);  //activate interupt for pin sent to function
	timestart = millis();

	while (gPulseCount < capturepulses) // capture data for number of puluses requried by input to function
	{
		if (gAbort = true) //allow escape if there is an Abort signal given by SM/C
		{
			detachInterrupt(digitalPinToInterrupt(inputpin));  //deactivate interupt pin
			return 0;
		}
		;  //Nothing is actualy being done in this function in this loop, it is accomplished by the interrupt function "pulse"
	}

	timeend = millis();
	detachInterrupt(digitalPinToInterrupt(inputpin));  //deactivate interupt pin

	switch (inputpin) //denpending upon which pulse input pin was used, do the math with the correct calibration constant
	{
	case gcFlowInPin:
		result = ((gCal.meterIN * capturepulses) / (timeend - timestart)) * 60000;  //result converted to quarts per minute.
		break;
	case gcFlowOutPin:
		result = ((gCal.meterOUT * capturepulses) / (timeend - timestart)) * 60000;
		break;
	}
	return result;
}
