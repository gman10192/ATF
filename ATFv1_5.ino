/*
Name:		ATVv1_5.in
Purpose:	Controller software for Automatic Transmission Flushing aparatus
Created:	10/14/2017 3:10:53 PM
Author:	Seth Gillies
*/

////LIBRARIES////
#include "stateManager.h"
#include "sgPID.h"
#include "EEPROM.h"
#include <SPI.h>
#include "genieArduino.h"


////GLOBAL VARIABLE STRUCTURES////
struct gsCal {
	double meterIN;
	double meterOUT;
	double pressIN;
	double pressOUT;
	double tankpress;
};
struct gsConfig {
	double Kp;
	double Ki;
	double Kd;
	double Ksample; //sample time frequency for PID loop
	int samplet; //duration to sample flow meter in ms
};



////GLOBAL VARIABLES////
volatile unsigned int gPulseCount = 0;
boolean gAbort = false;  //Universal abort bit
int unsigned long gPrevTime = 0;
int gTimeElapsed = 0;
double gFlowRateIn = 0;  //Flow rate of the IN flow meter (old fluid into system from vehicle)
double gFlowRateOut = 0; //flow rate of the OUT flow meter (new fluid out of system to vehicle)
double gPidInput, gPidOutput, gPidSetpoint = 0;
gsCal gCal;
gsConfig gConfig;
PID myPID(&gPidInput, &gPidOutput, &gPidSetpoint, 0, 0, 0, DIRECT);
Genie genie;
StateManagerClass stateManager{};  //Intialize object with defualt states (all zero)
bool gFault = FALSE;
byte gCurrentState = 0;
byte gRequestedState = 0;
byte gPreviousState = 0;
byte gExchVolumeSetpoint = 0;		//in quarts
byte gExchVolumeSetpointTemp = 0;  //In quarts
byte gFaultMessage = 0;
bool gResumeButtonState = FALSE;




#define IDLE 1
#define STARTING 2
#define EXECUTE 3
#define COMPLETING 4
#define COMPLETE 5
#define HOLDING 6
#define HELD 7
#define UNHOLDING 8
#define PAUSING 9
#define PAUSED 10
#define UNPAUSING 11
#define RESETTING 12
#define STOPPED 13
#define STOPPING 14
#define ABORTED 16
#define ABORTING 17
#define CONFIGURATION 18
#define MANUAL_MODE 19
#define FAULTED 20
#define WAITING 21
#define DONE_WAITING 22


////GLOBAL CONSTANTS////
const int gcEEsize = 4096;     //EEPROM size
const int gcAddrCal = 0;       //EEPROM address for start of calibration data array
const int gcAddrConfig = 64;     //EEPROM address for start of configuration data array

////GLOBAL CONSTANTS - Future System Settings////
const byte gcMaxStartPress = 2;  //Max pressure (psi) to be allowed for system to transition state
const byte gcMaxStartFlow = 0;	//Max flow allowed for system to transition state
const int gcMinStartPress = 5;	//Minimum pressure required when starting 
const int gcTimeOut = 10000;	//General timeout for faults, in milliseconds



////PIN Config////
const byte gcFlowInPin = 18;
const byte gcFlowOutPin = 19;
const byte gcLEDpin = 13;
const byte gcPressRegSSPin = 53;  //Pressure Regulator
const byte gcPressInSSPin = 46;	//Pressure IN
const byte gcPressOutSSPin = 45;	//Pressure OUT
const byte gcMKBus2ENpin = 11;
const byte gcMKBus3ENpin = 12;
const byte gcResetDisplayPin = 23;
const byte gcBypassCVPin = 34;
const byte gcReverseFlowCVPin = 35;
const byte gcVentTankCVPin = 36;
const byte gcEStopPin = 3;


////SETUP*******************************************************************************************
void setup() {
	Serial.begin(9600);

	//Setup for display communication
	Serial2.begin(14400);
	genie.Begin(Serial2);
	genie.AttachEventHandler(displayEventHandler);
	pinMode(gcResetDisplayPin, OUTPUT);
	digitalWrite(gcResetDisplayPin, LOW);
	delay(100);
	digitalWrite(gcResetDisplayPin, HIGH);
	delay(3500);	//Allow time for display to reset.
	genie.WriteStr(0, GENIE_VERSION);
	genie.WriteContrast(15);


	//wait for serial connection to attached PC (serial port 0)
	while (!Serial) {
		;
	}
	Serial.println("Serial Connection Active");

	//Configure PID parameters
	myPID.SetSampleTime(gConfig.Ksample);
	myPID.SetOutputLimits(0, 1300);
	myPID.SetTunings(gConfig.Kp, gConfig.Ki, gConfig.Kd);
	myPID.SetMode(AUTOMATIC);

	//Setup SS Pins for SPI communication with DAC/ADC modules
	pinMode(gcPressRegSSPin, OUTPUT);
	pinMode(gcPressInSSPin, OUTPUT);
	pinMode(gcPressOutSSPin, OUTPUT);

	digitalWrite(gcPressRegSSPin, HIGH);
	digitalWrite(gcPressInSSPin, HIGH);
	digitalWrite(gcPressOutSSPin, HIGH);

	pinMode(gcBypassCVPin, OUTPUT);
	pinMode(gcReverseFlowCVPin, OUTPUT);
	pinMode(gcVentTankCVPin, OUTPUT);
	digitalWrite(gcBypassCVPin, HIGH);  //Bypass active to start
	digitalWrite(gcReverseFlowCVPin, LOW);	//Flow reverse not active to start (normal flow)
	digitalWrite(gcVentTankCVPin, LOW);	//Tank vent is OPEN to start

	attachInterrupt(digitalPinToInterrupt(gcEStopPin), EStop, FALLING);




}

////LOOP*******************************************************************************************
void loop() {

	myPID.Compute();

	genie.DoEvents();	//Initiate recieving data from the Display





}

////FUNCTIONS*******************************************************************************************

void pulse()
{
	//FUNCTION:  INTERRUPT routine to count number of pulses 
	//INPUTS:	1) NONE
	//OUTPUT:	1) NONE  

	gPulseCount++;
}

void EStop()
{
	//FUNCTION:  INTERRUPT routine for EStop Button
	//INPUTS:	1) NONE
	//OUTPUT:	1) NONE

	//***TO DO: what to do when Estop has been triggered***
	//Disable Analog Output to Pressure Regulator

	//Vent tank pressure
	//Enable BYPASS
	//Send immediate state change to ABORT


}

