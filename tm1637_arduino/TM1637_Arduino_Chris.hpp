

/*===============================================================
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


  //===============================================================
*/


#ifndef __TM1637_Arduino_Chris__
#define __TM1637_Arduino_Chris__

#include <string.h>
#include <arduino.h>
#include "MyI2CDriver.hpp"
 
class TM1637_Arduino_Chris
{
    static const uint8_t Command_Data_Setting_Write_Data_To_Display_Register = 0b01000000; //0x40
    static const uint8_t TM1637_Starting_Address = 0xc0;
    static const uint8_t BitMapDot = 0b10000000; // dot bitmap
    static const uint8_t BitMapEmpty = 0b00000000; // empty, shows nothing
    static const uint8_t BitMapUV = 0b00111110;    // U V
    static const uint8_t BitMapMinus = 0b01000000;    // U V


    static const uint8_t MaxDigitCount = 6;
    MyI2CDriver *i2cDriver_p;
    // TM1637 supports 6 digit-seven-segments display, but you will probably just use a 4 digit tube

    // Do we show the leading zero ? so if 12, show 0012 ?
    bool showLeadingZero = false;

    // contains all the bitmaps for 0 1 2 ... F
    static const uint8_t DigitBitmapArray[];
    const uint8_t Index_Empty_Bitmap = 16; //this need to be removed

    //Arduino pins for clk and dio of TM1637
    uint8_t m_pin_Clk, m_pin_Dio;
    uint8_t charToInt(char numberChar);
    uint8_t charToBitMap(char numberChar);
    
    //void setDisplayBuffer(const uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount);
  public:

    bool m_debugPrint;

    TM1637_Arduino_Chris(uint8_t pinClk, uint8_t pinDio);

    // let digit tube to show a number 1234
    void display(unsigned long number);

    //
    //void showNumber(const char* number);
    void display(String str);

    void debugPrint(const char * str);
    void debugPrint(int i);
    void debugPrint(const char *name, unsigned long int value);
    void debugPrint(const char *name, const char * str);
 

    void clearAll();

    void doTest();
    
    ~TM1637_Arduino_Chris();


};
#endif  // __TM1637_Arduino_Chris__



