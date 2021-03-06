/////////////////////////////////////////////////////////////////////
//
// This is the user's event handler. It is called by genieDoEvents()
// when the following conditions are true
//
//		The link is in an IDLE state, and
//		There is an event to handle
//
// The event can be either a REPORT_EVENT frame sent asynchronously
// from the display or a REPORT_OBJ frame sent by the display in
// response to a READ_OBJ (genie.ReadObject) request.
//


//COMPACT VERSION HERE, LONGHAND VERSION BELOW WHICH MAY MAKE MORE SENSE
void displayEventHandler(void)
{
genieFrame Event;
	genie.DequeueEvent(&Event);
	
	//Event handlers for buttons pressed on Main Screen (Form0)
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_TRACKBAR, 0)) gExchVolumeSetpointTemp = genie.GetEventData(&Event);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 0)) stateManager.transitionTo(STARTING);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 1)) stateManager.transitionTo(STOPPING);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 2)) stateManager.transitionTo(ABORTING);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 3)) stateManager.transitionTo(HOLDING);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 4)) gResumeButtonState = TRUE; // transitionTo(UNHOLDING);  // This button is used in sequence, so this might not work...  Unles the StateManager is configured for it...
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 5)) stateManager.transitionTo(RESETTING);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 7)) stateManager.transitionTo(CONFIGURATION);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_WINBUTTON, 9)) stateManager.transitionTo(MANUAL_MODE);
	
	//Event handlers for buttons pressed on Manual Mode screen (Form4), actual event is from the indicator LED object
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_USER_LED, 1)) digitalWrite(gcBypassCVPin, HIGH);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_USER_LED, 2)) digitalWrite(gcReverseFlowCVPin, HIGH);
	if (genie.EventIs(&Event, GENIE_REPORT_EVENT, GENIE_OBJ_USER_LED, 3)) digitalWrite(gcVentTankCVPin, HIGH);


	
} 

//// LONG HAND VERSION, MAYBE MORE VISIBLE AND MORE LIKE VERSION 1 OF THE LIBRARY
//void displayEventHandler(void)
//{
//	genieFrame Event;
//	genie.DequeueEvent(&Event); // Remove the next queued event from the buffer, and process it below
//
//	int slider_val = 0;
//
//	//If the cmd received is from a Reported Event (Events triggered from the Events tab of Workshop4 objects)
//	if (Event.reportObject.cmd == GENIE_REPORT_EVENT)
//	{
//		if (Event.reportObject.object == GENIE_OBJ_SLIDER)                // If the Reported Message was from a Slider
//		{
//			if (Event.reportObject.index == 0)                              // If Slider0 (Index = 0)
//			{
//				slider_val = genie.GetEventData(&Event);                      // Receive the event data from the Slider0
//				genie.WriteObject(GENIE_OBJ_LED_DIGITS, 0, slider_val);       // Write Slider0 value to to LED Digits 0
//			}
//		}
//	}
//
//	//If the cmd received is from a Reported Object, which occurs if a Read Object (genie.ReadOject) is requested in the main code, reply processed here.
//	if (Event.reportObject.cmd == GENIE_REPORT_OBJ)
//	{
//		if (Event.reportObject.object == GENIE_OBJ_USER_LED)              // If the Reported Message was from a User LED
//		{
//			if (Event.reportObject.index == 0)                              // If UserLed0 (Index = 0)
//			{
//				bool UserLed0_val = genie.GetEventData(&Event);               // Receive the event data from the UserLed0
//				UserLed0_val = !UserLed0_val;                                 // Toggle the state of the User LED Variable
//				genie.WriteObject(GENIE_OBJ_USER_LED, 0, UserLed0_val);       // Write UserLed0_val value back to to UserLed0
//			}
//		}
//	}

	/********** This can be expanded as more objects are added that need to be captured *************
	*************************************************************************************************
	Event.reportObject.cmd is used to determine the command of that event, such as an reported event
	Event.reportObject.object is used to determine the object type, such as a Slider
	Event.reportObject.index is used to determine the index of the object, such as Slider0
	genie.GetEventData(&Event) us used to save the data from the Event, into a variable.
	*************************************************************************************************/

