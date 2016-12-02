

/*===========================================================
Hardware connection
digitube A, on the right, it's a 4 digits seven-segement tube.
digitube B, on the Left, it's a 2 digits seven-segement tube.
digitube B is placed on the left of digitube A.
digitube B could be absent, so the system can only display 4 digits (ie. 3456)
If digitube B is installed, then the system can display 6 digits (ie. 123456)

TM1637 Pin15(Grid1) connect digitube A, From Left to Right, the 4th digit
TM1637 Pin14(Grid2) connect digitube A, From Left to Right, the 3rd digit
TM1637 Pin13(Grid3) connect digitube A, From Left to Right, the 2nd digit
TM1637 Pin12(Grid4) connect digitube A, From Left to Right, the 1st digit
TM1637 Pin11(Grid5) connect digitube B, From Left to Right, the 2nd digit
TM1637 Pin10(Grid6) connect digitube B, From Left to Right, the 1st digit

Connect wires like this, can make the PCB trace design easier.

If frozen, try to add two 100pf capacitors for CLK and DIO wires as the TM1637 datasheet suggests, or use shorter wires.


//===========================================================
*/

#include <stdint.h>
#include <arduino.h>


class TM1637_Arduino_Chris
{

	static const uint8_t Command_Data_Setting_Write_Data_To_Display_Register = 0b01000000; //0x40
	static const uint8_t Command_Display_And_Control_Command_Setting = 0b10000000; //0x80
	static const uint8_t Command_Address_Command_Setting = 0b11000000; //0xc0
	static const uint8_t TM1637_Starting_Address = 0xc0;


	// TM1637 supports 6 digit-seven-segments display, but you will probably just use a 4 digit tube
	static const uint8_t MaxDigitCount = 6;

	// Do we show the leading zero ? so if 12, show 0012 ?
	bool showLeadingZero = false;
	uint8_t bitmapIndexArray[MaxDigitCount];

	static const uint8_t DigitBitmaps[];  
	const uint8_t Index_Dot_Bitmap = 16;
	const uint8_t Index_Empty_Bitmap = 17;





	//Arduino pins for clk and dio of TM1637
	uint8_t m_pin_Clk, m_pin_Dio;

public:

	bool m_debugLowLevel ;
	bool m_debugHighLevel ;

	TM1637_Arduino_Chris(int pinClk, int pinDio);
	 
	void delayCLKDIO();


	// let digit tube to show a number 1234
	void showNumber(unsigned long num);
	void debugLowLevelPrint(char *str);
	void debugLowLevelPrint(int i);
	void debugHighLevelPrint(char * str);
	void debugHighLevelPrint(int i);

	void setDisplayBuffer(uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount);
	void i2cStart();
	void i2cStop();
	void i2cAck();
	void i2cWriteByte(unsigned char oneByte);
	void clearAll();



};

