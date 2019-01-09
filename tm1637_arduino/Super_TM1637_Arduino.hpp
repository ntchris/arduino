

/*  ===============================================================
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

    Update 2018 Jan 21:
    ==========================
    TM1637 Pin15(Grid1) connect digitube A, From Left to Right, the 1st digit(digitube pin12, digit1)
    TM1637 Pin14(Grid2) connect digitube A, From Left to Right, the 2nd digit(digitube pin19, digit2)
    TM1637 Pin13(Grid3) connect digitube A, From Left to Right, the 3rd digit(digitube pin18, digit3)
    TM1637 Pin12(Grid4) connect digitube A, From Left to Right, the 4th digit(digitube pin16, digit4)

    Support clock :
    dp is : when digit2 is active, otherwise dp is .Well is has to be : 12:34  (so : is like a . dp for 2)
    some(or all?) clock digitube has no dot dp for all digits except 2nd digit.
    so 1.2.3.4. can only display 12:34

    For clock digit tube, dp . is special, if input 12.34, it will show 12:34.
    dp is displayed only when 1st digit high and dp., but dp is fixed for 2nd digit
    so input 1.234 , display is 12.34 (since dp is fixed for 2nd digit)
    this makes handling dp in clock digit tube quite difficult or confusing. easiest way is not do handling dp for clock tube
    //===============================================================
*/


#ifndef __Super_TM1637_Arduino__
#define __Super_TM1637_Arduino__

#include <string.h>
#include "Arduino.h"
#include "SuperI2CDriver.hpp"

class Super_TM1637_Arduino
{
        static const uint8_t Command_Data_Setting_Write_Data_To_Display_Register = 0b01000000; //0x40
        static const uint8_t Command_SetDisplayBrightness = 0b10000000;
        static const uint8_t Command_SetReadKey = 0b01000010;// 0x42;

        static const uint8_t DisplayBrightness = 0b011; // brightness is 10/16
        static const uint8_t DisplayOnBit = 0b1000; // ON
        static const uint8_t DisplayOffBit = 0b0000; // OFF
        static const uint8_t TM1637_Starting_Address = 0xc0;
        //colon clock :  is the same as dp, to show : , enable digit1 with dp.   for enabled digit234 with dp, it's dp. only digit1 is :
        static const uint8_t BitMapDot = 0b10000000; // dot bitmap
        static const uint8_t BitMapEmpty = 0b00000000; // empty, shows nothing
        static const uint8_t BitMapUV = 0b00111110;    // U V
        static const uint8_t BitMapMinus = 0b01000000;    // U V
        static const uint8_t ColonDigitIndex = 1; // For clock digitube, set digit2 (index=1), and active DP, its clock colon :

     

        uint8_t m_displayBrightness;
        uint8_t m_currentDisplayOnOffBit;



        //    static const uint8_t BitMapOverflow = 0b11111111;    // 8.
        //TM1637 supports 6 digits, however we may only connect a 4 digits digitube, or a 3 digits digitube.
        //so when user input a longer string, it's overflow, we should report error by showing  overflow digits 8.8.8.8.8.
        //otherwise, if usr input "-1234" , a 4 digits digitube display 1234, then it's very misleading.
        int m_maxDisplayDigits;
        static const uint8_t MaxDigitCount = 6;  // TM1637 supports 6 digits
        SuperI2CDriver *i2cDriver_p;
        // TM1637 supports 6 digit-seven-segments display, but you will probably just use a 4 digit tube


        // contains all the bitmaps for 0 1 2 ... F
        static const uint8_t DigitBitmapArray[];
        
        static const uint8_t DigitBitmapArrayLetterS;//  = 0b01101101;
        static const uint8_t DigitBitmapArrayLetters;//  = 0b01101101;

        static const uint8_t DigitBitmapArrayLettert;//  = 0b00111001;    // t
        static const uint8_t DigitBitmapArrayLetterT;//  = 0b00111001;    // t
        static const uint8_t DigitBitmapArrayLettery;
        static const uint8_t DigitBitmapArrayLetterY;
        static const uint8_t DigitBitmapArrayLetterO;

        const uint8_t Index_Empty_Bitmap = 16; //this need to be removed

        //Arduino pins for clk and dio of TM1637
        uint8_t m_pin_Clk, m_pin_Dio;
        uint8_t charToInt(char numberChar);
        uint8_t charToBitMap(char numberChar);
        void   displayOverflow();
        bool checkIfOverflow(String str);
        int checkRequiredDigitCount(String str);

        //void setDisplayBuffer(const uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount);

        void setDisplayBuffer(uint8_t *bitmapArray, uint8_t arraySize);


    public:
        static const uint8_t nokey = 0b11111111;
        static const uint8_t key1 = 0b11101111;
        static const uint8_t key2 = 0b01101111;
        static const uint8_t key3 = 0b10101111;
        static const uint8_t key4 = 0b00101111;

        static const uint8_t MaxBrightness = 7;

        bool m_debugPrint;

        Super_TM1637_Arduino(uint8_t pinClk, uint8_t pinDio, uint8_t digitubeDigits);

        // let digit tube to show a number 1234
        void display(unsigned long number);
        void display_from_end(String str);
        void display(String str);
        void display(float f);
        void switchOnOff(bool on);
        
        uint8_t readKey();

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
        void doTestDisplayAndKeys();

        ~Super_TM1637_Arduino();


};
#endif  // __Super_TM1637_Arduino__



