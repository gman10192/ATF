void stateWaiting(void) {
	int _starttime = 0;

	while (StateManager.isInState(WAITING)==TRUE) {
		genie.DoEvents();
		delay(50);
		if ((millis() - _starttime) > gcTimeOut) {  //ERROR: user input timeout, bail out of fucntion
			gFault = TRUE;
			gFaultMessage = 9; //User input timeout
			break;
			return;
		}
	}
	StateManager.transitionTo(DONE_WAITING);

}
