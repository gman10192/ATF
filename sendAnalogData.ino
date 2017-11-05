////FUNCTION*******************************************************************************************
void sendAnalogData(float setpoint, byte sspin)
{
	//FUNCTION: Send 4-20mA analog signal to electonric pressure regulator
	//			via click Module which uses a MCP4921 chip.
	//			which uses a MCP3201 chip interfaced via SPI
	//INPUTS:	1) Pressure value in PSI 
	//			2) Slave Select Pin (SS)
	//OUTPUT:	1) NONE

	word outputvalue = 0;
	byte data = 0;

	outputvalue = map(setpoint, 3, 120, 0, 4095);	//Convert pressure value to range accepted by module, *TO DO* in the future convert the 3 & 120 to variables.

	digitalWrite(sspin, LOW);
	SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

	data = highByte(outputvalue);
	data = 0b00001111 & data;
	data = 0b00110000 | data;
	SPI.transfer(data);
	data = lowByte(outputvalue);
	SPI.transfer(data);
	digitalWrite(sspin, HIGH);
}

