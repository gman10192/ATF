/*
Name:		ATVv1_5.in
Purpose:	Controller software for Automatic Transmission Flushing aparatus
Created:	10/14/2017 3:10:53 PM
Author:	Seth Gillies
*/
////LIBRARIES////
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
volatile unsigned int PulseCount = 0;
double FlowRateIn = 0, FlowRateOut =0;  //IN = old fluid from vehicle to rig, OUT = new fluid to vehicle
double PidInput, PidOutput, PidSetpoint = 0;
gsCal gCal;
gsConfig gConfig;
PID myPID(&PidInput, &PidOutput, &PidSetpoint, 0, 0, 0, DIRECT);
Genie genie;
bool Fault = FALSE;
int FaultMsg = 0;
int CurrentState = 0, PrevState = 0, FaultedState = 0;
int TempVolumeSP = 0, VolumeSP = 0, ExchangedVolume = 0;		//in quarts
int AbortPB = 0, ResetPB = 0, ClearPB = 0, StartPB = 0, PausePB = 0, StopPB = 0, ProceedPB = 0, ConfigurationPB = 0, ManualModePB = 0;
double StateStartTime = 0;

//State Names
#define INITIALIZE 0
#define IDLE 1
#define LOAD_SETTINGS 2
#define STARTUP 3
#define CAPTURE_FLOW_PRESS 4
#define RUN 5
#define DONE 6
#define CAPTURE_FLOW_PRESS2 7  //Could this be the same as #4?
#define RESET 8
#define ABORT 9
#define UPDATE_DISPLAY 10
#define FAULT 20

#define ACTIVE HIGH
#define DISABLE LOW

////GLOBAL CONSTANTS////
const int k_EEsize = 4096;     //EEPROM size
const int k_AddrCal = 0;       //EEPROM address for start of calibration data array
const int k_AddrConfig = 64;     //EEPROM address for start of configuration data array

////GLOBAL CONSTANTS - Future System Settings////
const byte k_MaxStartPress = 2;  //Max pressure (psi) to be allowed for system to transition state
const byte k_MaxStartFlow = 0;	//Max flow allowed for system to transition state
const int k_MinStartPress = 5;	//Minimum pressure required when starting 
const int k_10sTimeOut = 10000;	//General timeout for faults, in milliseconds

////PIN Config////
const byte pin_FlowIn = 18;
const byte pin_FlowOut = 19;
const byte pin_LED = 13;
const byte pin_PressRegSS = 53;  //Pressure Regulator
const byte pin_PressInSS = 46;	//Pressure IN
const byte pin_PressOutSS = 45;	//Pressure OUT
const byte pin_MKBus2EN = 11;
const byte pin_MKBus3EN = 12;
const byte pin_ResetDisplay = 23;
const byte pin_BypassCV = 34;	//HIGH = Bypass is ACTIVE
const byte pin_ReverseFlowCV = 35;
const byte pin_VentTankCV = 36;
const byte pin_EStop = 3;


////SETUP*******************************************************************************************
void setup() {
	Serial.begin(9600);

	//Setup for display communication
	Serial2.begin(14400);
	genie.Begin(Serial2);
	genie.AttachEventHandler(displayEventHandler);
	pinMode(pin_ResetDisplay, OUTPUT);
	digitalWrite(pin_ResetDisplay, LOW);
	delay(100);
	digitalWrite(pin_ResetDisplay, HIGH);
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
	pinMode(pin_PressRegSS, OUTPUT);
	pinMode(pin_PressInSS, OUTPUT);
	pinMode(pin_PressOutSS, OUTPUT);

	digitalWrite(pin_PressRegSS, HIGH);
	digitalWrite(pin_PressInSS, HIGH);
	digitalWrite(pin_PressOutSS, HIGH);

	pinMode(pin_BypassCV, OUTPUT);
	pinMode(pin_ReverseFlowCV, OUTPUT);
	pinMode(pin_VentTankCV, OUTPUT);
	digitalWrite(pin_BypassCV, HIGH);  //Bypass active to start
	digitalWrite(pin_ReverseFlowCV, LOW);	//Flow reverse not active to start (normal flow)
	digitalWrite(pin_VentTankCV, LOW);	//Tank vent is OPEN to start

	attachInterrupt(digitalPinToInterrupt(pin_EStop), EStop, FALLING);

	CurrentState = INITIALIZE;	//Always set state to Initialize when power up or on reset of controller.  THis will then force a Reset.
}

////LOOP*******************************************************************************************
void loop() {

	myPID.Compute();	//Update PID loop
	genie.DoEvents();	//Update inputs from display
	genie.WriteObject(GENIE_OBJ_STRINGS, 0, CurrentState);

	switch (CurrentState) {
	case INITIALIZE:
		state_Initialize();
		break;
	case IDLE:
		state_Idle();
		break;	
	case LOAD_SETTINGS:
		state_LoadSettings();
		break;
	case STARTUP:

		break;
	case CAPTURE_FLOW_PRESS:

		break;
	case RUN:

		break;
	case DONE:
		break;
	case CAPTURE_FLOW_PRESS2:
		break;
	case RESET:
		break;
	case ABORT:
		break;
	case UPDATE_DISPLAY:
		break;
	case FAULT:
		break;
	default:
		PrevState = CurrentState;
		CurrentState = 0;
	}




}

////FUNCTIONS*******************************************************************************************

void pulse()
{
	//FUNCTION:  INTERRUPT routine to count number of pulses 
	//INPUTS:	1) NONE
	//OUTPUT:	1) NONE  

	PulseCount++;
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

