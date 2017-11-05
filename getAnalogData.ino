////FUNCTION*******************************************************************************************
float getAnalogData(byte sspin)
{
	//FUNCTION: Get analog data from 4-20mA Analog Input click Module
	//			which uses a MCP3201 chip interfaced via SPI
	//INPUTS:	1) Slave Select Pin (SS)
	//			2)  
	//OUTPUT:	1) Pressure in PSI

	word data = 0;
	byte upper = 0, lower = 0;
	float result = 0;

	digitalWrite(sspin, LOW);
	SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

	upper = SPI.transfer(0x00);	//Send nothing to initiate reading by ADC
	upper = 0b00011111 & upper;
	lower = SPI.transfer(0x00);	//Send nothing to initiate second part of data from ADC (it uses 16-bit words)
	data = (upper << 8) | lower;
	data = data >> 1;
	data = 0b0000111111111111 & data;

	result = map(data, 0, 4095, 0, 100);

	return result;
}

