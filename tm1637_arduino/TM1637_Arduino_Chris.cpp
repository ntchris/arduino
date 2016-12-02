#include "TM1637_Arduino_Chris.hpp"


static const uint8_t TM1637_Arduino_Chris::DigitBitmaps[]  = {
	0b00111111,    // 0
	0b00000110,    // 1
	0b01011011,    // 2
	0b01001111,    // 3
	0b01100110,    // 4
	0b01101101,    // 5
	0b01111101,    // 6
	0b00000111,    // 7
	0b01111111,    // 8
	0b01101111,    // 9
	0b01110111,    // A
	0b01111100,    // b
	0b00111001,    // C
	0b01011110,    // d
	0b01111001,    // E
	0b01110001,    // F
	0b10000000,    // dot
	0b00000000     // empty bitmap
};

TM1637_Arduino_Chris::TM1637_Arduino_Chris(int pinClk, int pinDio)
{
	m_pin_Clk = pinClk;
	m_pin_Dio = pinDio;

	m_debugLowLevel = false;
    m_debugHighLevel = false;

	pinMode(m_pin_Clk, OUTPUT);
	pinMode(m_pin_Dio, OUTPUT);
	digitalWrite(m_pin_Clk, LOW);
	digitalWrite(m_pin_Dio, LOW);
	delayCLKDIO();

}



void TM1637_Arduino_Chris::delayCLKDIO()
{
	//delayMicroseconds(30);
	delayMicroseconds(5);

	//delay(1);
}

void TM1637_Arduino_Chris::debugHighLevelPrint(int i)
{
	if (m_debugHighLevel)
	{
		Serial.print(i);
	}
}


void TM1637_Arduino_Chris::debugHighLevelPrint(char * str)
{
	if (m_debugHighLevel)
	{
		Serial.print(str);
	}
}
void TM1637_Arduino_Chris::debugLowLevelPrint(int i)
{
	if (m_debugLowLevel)
	{
		Serial.print(i);
	}

}

void TM1637_Arduino_Chris::debugLowLevelPrint(char *str)
{
	if (m_debugLowLevel)
	{
		Serial.print(str);
	}

}






void TM1637_Arduino_Chris::setDisplayBuffer(uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount)
{

	if (!indexArray_p)
	{
		debugHighLevelPrint("null pointer for indexArray\n");
		return;
	}
	if (digitCount >= MaxDigitCount)
	{
		digitCount = MaxDigitCount;
	}

	i2cStart();
	i2cWriteByte(Command_Data_Setting_Write_Data_To_Display_Register); //0x40
	i2cAck();
	i2cStop();
	i2cStart();


	i2cWriteByte(TM1637_Starting_Address); //starting address
	debugLowLevelPrint("write starting addr 0xC0 done\n");
	i2cAck();
	int i;

	//showing digit from the back so the 1st digit is always need to show
	bool isLeadingZero = true;
	for (i = 0; i < MaxDigitCount; i++)
	{
		debugHighLevelPrint("for in setDisplayBuffer: ");
		debugHighLevelPrint(i);
		debugHighLevelPrint(" : ");
		uint8_t digit = indexArray_p[i];
		debugHighLevelPrint(digit);
		debugHighLevelPrint("\n");

		i2cWriteByte(DigitBitmaps[digit]);

		i2cAck();
	}

	i2cStop();
	//digitalWrite(LED_BUILTIN, LOW);

	debugLowLevelPrint("for is done\n");
	i2cStart();
	i2cWriteByte(0x8f);
	i2cAck();
	i2cStop();

	debugLowLevelPrint("setDisplayBuffer\n");

}






// let digit tube to show a number 1234
void TM1637_Arduino_Chris::showNumber(unsigned long num)
{
	uint8_t i;
	const unsigned int Ten = 10;
	bool isLeadingZero = true;
	//clear the previous index buffer
	memset(bitmapIndexArray, 0, MaxDigitCount);
	for (i = 0; i < MaxDigitCount; i++)
	{
		uint8_t digit = num % Ten;
		bitmapIndexArray[i] = digit;
		num = num / Ten;
		if (num == 0)
		{
			break;
		}
	}

	for (i = 0; i < MaxDigitCount; i++)
	{
		debugHighLevelPrint(bitmapIndexArray[i]);
		debugHighLevelPrint(",");
	}
	debugHighLevelPrint("\n");


	if (!showLeadingZero)
	{ //now deal with leading zero
		for (i = MaxDigitCount - 1; i >= 0; i--)
		{
			debugHighLevelPrint("bitmapIndexArray[i]: ");
			debugHighLevelPrint(bitmapIndexArray[i]);
			debugHighLevelPrint("\n");
			if (bitmapIndexArray[i] == 0)
			{
				//this is a leading zero, so use empty bitmap index instead of 0 bitmap index
				debugHighLevelPrint("replacing 0 to empty\n");
				bitmapIndexArray[i] = Index_Empty_Bitmap;
			}
			else
			{
				//stop processing leading zero once we meet the first non-zero number
				break;
			}
		}
	}

	setDisplayBuffer(bitmapIndexArray);

}


 

void TM1637_Arduino_Chris::i2cStart()
{
	debugLowLevelPrint("i2cStart begin\n");
	digitalWrite(m_pin_Dio, HIGH);
	delayCLKDIO();

	digitalWrite(m_pin_Clk, HIGH);
	delayCLKDIO();
	digitalWrite(m_pin_Dio, LOW);
	debugLowLevelPrint("i2cStart done\n");

}

void TM1637_Arduino_Chris::i2cStop()
{
	debugLowLevelPrint("stop begin\n");
	digitalWrite(m_pin_Clk, LOW);
	delayCLKDIO();
	digitalWrite(m_pin_Dio, LOW);
	delayCLKDIO();

	digitalWrite(m_pin_Clk, HIGH);
	delayCLKDIO();
	digitalWrite(m_pin_Dio, HIGH);
	debugLowLevelPrint("stop done\n");

}

void TM1637_Arduino_Chris::i2cAck()
{
	debugLowLevelPrint("ack begin\n");
	//digitalWrite(LED_BUILTIN, LOW);

	digitalWrite(m_pin_Clk, LOW);
	delayCLKDIO();

	unsigned char dio;
	bool on = true;
	do {
		dio = digitalRead(m_pin_Dio);
	} while (dio);
	//The 9th clock high
	digitalWrite(m_pin_Clk, HIGH);
	delayCLKDIO();
	//The 9th clock low
	digitalWrite(m_pin_Clk, LOW);
	//digitalWrite(LED_BUILTIN, HIGH);
	//now DIO should be released by TM1637
	debugLowLevelPrint("ack end\n");
}


void TM1637_Arduino_Chris::i2cWriteByte(unsigned char oneByte)
{
	const uint8_t BitsPerByte = 8; //always 8, dont' change it
	uint8_t i = 0;

	debugLowLevelPrint("write byte begin\n");
	for (i = 0; i < BitsPerByte; i++)
	{
		debugLowLevelPrint("for in writeByte ");
		debugLowLevelPrint(i); debugLowLevelPrint("\n");

		digitalWrite(m_pin_Clk, LOW);
		delayCLKDIO();
		digitalWrite(m_pin_Dio, oneByte & 0x01);


		delayCLKDIO();
		oneByte = oneByte >> 1;
		digitalWrite(m_pin_Clk, HIGH);
		delayCLKDIO();

	}
	debugLowLevelPrint("write byte done\n");
}





void TM1637_Arduino_Chris::clearAll()
{
	i2cStart();
	i2cWriteByte(Command_Data_Setting_Write_Data_To_Display_Register); //0x40
	i2cAck();
	i2cStop();
	i2cStart();


	i2cWriteByte(TM1637_Starting_Address); //starting address
	debugHighLevelPrint("write starting addr 0xC0 done\n");
	i2cAck();
	int i;
	for (i = 0; i < 6; i++)
	{
		i2cWriteByte(0);
		i2cAck();
	}
	i2cStop();
	debugHighLevelPrint("clearAll done\n");
	delay(300);

}
