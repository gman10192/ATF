////FUNCTION*******************************************************************************************

void stateExecute(void) {

	int flowIn = 0, flowOut = 0, pressIn = 0, pressOut = 0;
	

	//Step 1: Capture flow data
	genie.WriteObject(GENIE_OBJ_STRINGS, 2, 4);  //Waiting for system pressure & flow to stabilize



}
