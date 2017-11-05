//Simple function to make sure the pressure sensors and flow meters are reading near zero

bool verifyNoFlowNoPressure(void)
{
	int p1 = 0, p2 = 0, flow1 = 0, flow2 = 0;

	//Capture flow and pressure readings
	p1 = getAnalogData(gcPressInSSPin);
	p2 = getAnalogData(gcPressOutSSPin);
	flow1 = flowTime(gcFlowInPin, 200);
	flow2 = flowTime(gcFlowOutPin, 200);

	if (p1 >= gcMaxStartPress) {
		gFaultMessage = 1;  //Pressure IN not zero
		gFault = TRUE;
		return FALSE;
	}
	else if (p2 >= gcMaxStartPress) {
		gFaultMessage = 2;  //Pressure OUT not zero
		gFault = TRUE;
		return FALSE;
	}
	else if (flow1 > gcMaxStartFlow){
		gFaultMessage = 3;  //Flow IN not zero
		gFault = TRUE;
		return FALSE;
	}
	else if (flow2 > gcMaxStartFlow){
		gFaultMessage = 4;  //Flow OUT not zero
		gFault = TRUE;
		return FALSE;
	}
	else {
		return TRUE;
	}
}


	




