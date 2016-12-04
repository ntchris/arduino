

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
    static const uint8_t Command_SetDisplayBrightness = 0b10000000;
    static const uint8_t DisplayBrightness = 0b011; // brightness is 10/16
    static const uint8_t DisplayOnBit = 0b1000; // ON
    static const uint8_t DisplayOffBit = 0b0000; // OFF
    static const uint8_t TM1637_Starting_Address = 0xc0;
    static const uint8_t BitMapDot = 0b10000000; // dot bitmap
    static const uint8_t BitMapEmpty = 0b00000000; // empty, shows nothing
    static const uint8_t BitMapUV = 0b00111110;    // U V
    static const uint8_t BitMapMinus = 0b01000000;    // U V
    uint8_t m_displayBrightness;
    uint8_t m_currentDisplayOnOffBit;



    //    static const uint8_t BitMapOverflow = 0b11111111;    // 8.
    //TM1637 supports 6 digits, however we may only connect a 4 digits digitube, or a 3 digits digitube.
    //so when user input a longer string, it's overflow, we should report error by showing  overflow digits 8.8.8.8.8.
    //otherwise, if usr input "-1234" , a 4 digits digitube display 1234, then it's very misleading.
    int m_maxDisplayDigits;
    static const uint8_t MaxDigitCount = 6;  // TM1637 supports 6 digits
    MyI2CDriver *i2cDriver_p;
    // TM1637 supports 6 digit-seven-segments display, but you will probably just use a 4 digit tube


    // contains all the bitmaps for 0 1 2 ... F
    static const uint8_t DigitBitmapArray[];
    const uint8_t Index_Empty_Bitmap = 16; //this need to be removed

    //Arduino pins for clk and dio of TM1637
    uint8_t m_pin_Clk, m_pin_Dio;
    uint8_t charToInt(char numberChar);
    uint8_t charToBitMap(char numberChar);
    void   displayOverflow();
    bool checkIfOverflow(String str);
    void switchOnOff(bool on);
    //void setDisplayBuffer(const uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount);

    void setDisplayBuffer(uint8_t *bitmapArray, uint8_t arraySize);
  public:
    static const uint8_t MaxBrightness = 7;

    bool m_debugPrint;

    TM1637_Arduino_Chris(uint8_t pinClk, uint8_t pinDio, uint8_t digitubeDigits);

    // let digit tube to show a number 1234
    void display(unsigned long number);

    void display(String str);
    void display(float f);
    //valid value is 0 to 7, larger than 7 then take it as 7
    void setBrightness(uint8_t level);
    void debugPrint(const char * str);
    void debugPrint(int i);
    void debugPrint(const String name, unsigned long int value);
    void debugPrint(const String name, const String str);


    void clearAll();

    void doTest();
    void doTestIntLoop();
    void doTest8888();
    void doTestSwitchOnOff();
    void doTestBrightnessLevel();


    ~TM1637_Arduino_Chris();


};
#endif  // __TM1637_Arduino_Chris__



