////FUNCTION*******************************************************************************************
float flowTime(short inputpin, int capturetime)
{
	//FUNCTION: Capture pulses and convert to flow rate.
	//			Uses a FIXED sample time (capturetime).
	//INPUTS:	1) Pin number of the pulse input from the flow meter
	//			2) Time duration to capture the flow data, in ms 
	//OUTPUT:	1) flow rate (in quarts per minute)

	float result;
	double timestart = millis();
	double timeend, timethen;
	timeend = timestart + capturetime;
	PulseCount = 0;	//Reset the pulse counter to zero

	attachInterrupt(digitalPinToInterrupt(inputpin), pulse, CHANGE);  //activate interupt for pin sent to function

	while (millis() < timeend) // capture data for time requried by input to function
	{
		if (gAbort = true) //allow escape if there is an Abort signal given by SM/C
		{
			detachInterrupt(digitalPinToInterrupt(inputpin));  //deactivate interupt pin
			return 0;
		}
		;  //Nothing is actualy being done in this function in this loop, it is accomplished by the interrupt function "pulse"
	}
	timethen = millis(); //get the actual time that data wasn't being recorded anymore
	detachInterrupt(digitalPinToInterrupt(inputpin));  //deactivate interupt pin

	switch (inputpin) //denpending upon which pulse input pin was used, do the math with the correct calibration constant
	{
	case pin_FlowIn:
		result = ((PulseCount * gCal.meterIN) / (millis() - timethen)) * 60000;
		break;
	case pin_FlowOut:
		result = ((PulseCount * gCal.meterOUT) / (millis() - timethen)) * 60000;
		break;
	}
	return result;
}


//Old Method - Kept here for future reference if needed.
/*The pulse detection is active until the time elapsed is greater than 'sampletime', then
the following code is run to do the math for calculating flow rate etc.  During this time
the interupts are disabled, and enabled once calculations are complete*/
//if ((millis() - gPrevTime) > gConfig.samplet) {
//	detachInterrupt(digitalPinToInterrupt(18));
//	detachInterrupt(digitalPinToInterrupt(19));
//	FlowRateIn = (pulsecountIN / ((millis() - gPrevTime) / 1000))*gCal.meterIN;  //gallons per actual time elapsedof 
//	FlowRateOut = (pulsecountOUT / ((millis() - gPrevTime) / 1000))*gCal.meterOUT; //gallons per actual time elapsed
//	gPrevTime = millis();
//	//calculated flow rate delta and store for use with PID controller
//	PidInput = FlowRateOut - FlowRateIn;  //Negative value here is BAD for car!
//											 //Reset pulse counters and reactivate the interupt pins
//	pulsecountIN = 0;
//	pulsecountOUT = 0;
//
//	attachInterrupt(digitalPinToInterrupt(19), IRQPulseOUT, CHANGE);
//}
