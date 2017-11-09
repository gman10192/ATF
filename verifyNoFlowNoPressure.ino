//Simple function to make sure the pressure sensors and flow meters are reading near zero

bool verifyNoFlowNoPressure(void)
{
	int p1 = 0, p2 = 0, flow1 = 0, flow2 = 0;

	//Capture flow and pressure readings
	p1 = getAnalogData(pin_PressInSS);
	p2 = getAnalogData(pin_PressOutSS);
	flow1 = flowTime(pin_FlowIn, 200);
	flow2 = flowTime(pin_FlowOut, 200);

	if (p1 >= k_MaxStartPress) {
		FaultMsg = 1;  //Pressure IN not zero
		gFault = TRUE;
		return FALSE;
	}
	else if (p2 >= k_MaxStartPress) {
		FaultMsg = 2;  //Pressure OUT not zero
		gFault = TRUE;
		return FALSE;
	}
	else if (flow1 > k_MaxStartFlow){
		FaultMsg = 3;  //Flow IN not zero
		gFault = TRUE;
		return FALSE;
	}
	else if (flow2 > k_MaxStartFlow){
		FaultMsg = 4;  //Flow OUT not zero
		gFault = TRUE;
		return FALSE;
	}
	else {
		return TRUE;
	}
}


	




