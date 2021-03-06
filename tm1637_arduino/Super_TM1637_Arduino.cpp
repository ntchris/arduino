#include "Super_TM1637_Arduino.hpp"


// to do ? numbers use one array, letter a-z use another
const uint8_t Super_TM1637_Arduino::DigitBitmapArray[]  = {
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
    
};


                                                // bit: dp,g, f, e, d c, b, a
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLetterL = 0b00111000;  //fed
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLetterS = 0b01101101;
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLetters = 0b01101101;
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLettert = 0b01111000;    // t
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLetterT = 0b00111001;    // t
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLettery = 0b01100110;    // y  same as Y
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLetterY = 0b01100110;    // Y  same as y
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLettero = 0b01011100;
const uint8_t Super_TM1637_Arduino::DigitBitmapArrayLetterO = 0b00111111;    // same as 0

Super_TM1637_Arduino::Super_TM1637_Arduino(uint8_t pinClk, uint8_t pinDio, uint8_t digitubeDigits)
{
    m_pin_Clk = pinClk;
    m_pin_Dio = pinDio;
    m_displayBrightness = 3; //default brightness
    m_debugPrint = false;
    m_currentDisplayOnOffBit = DisplayOnBit;

    m_maxDisplayDigits = digitubeDigits;
    uint8_t displaySetting = Command_SetDisplayBrightness | m_displayBrightness | m_currentDisplayOnOffBit ;
    tmi2cDriver_p = new SuperTMI2CDriver(m_pin_Clk, m_pin_Dio );
    //tmi2cDriver_p->m_debugPrint = m_debugPrint ;

    tmi2cDriver_p->startCommand(displaySetting);
}

Super_TM1637_Arduino:: ~Super_TM1637_Arduino()
{
    delete tmi2cDriver_p;
}



void Super_TM1637_Arduino::debugPrint(int i)
{
    if (m_debugPrint)
    {
        Serial.print(i);
    }

}

void Super_TM1637_Arduino::debugPrint(const char *str)
{
    if (m_debugPrint)
    {
        Serial.print(str);
    }

}


void Super_TM1637_Arduino::debugPrint(const String name, unsigned long int  value)
{
    if (m_debugPrint)
    {
        Serial.print(name);
        Serial.print(value);
        Serial.print("\n");
    }

}

void Super_TM1637_Arduino::debugPrint(const String name, const String str)
{
    if ( m_debugPrint)
    {
        Serial.print(name);
        Serial.print(str);
        Serial.print("\n");
    }

}


void Super_TM1637_Arduino::clearAll()
{
    display("");
}


uint8_t Super_TM1637_Arduino::charToBitMap(char numberChar)
{
    uint8_t bitmap = BitMapEmpty;
    if (numberChar >= '0' && numberChar <= '9' )
    {
        bitmap = DigitBitmapArray[numberChar - '0'];   // char to int
    } else if (numberChar >= 'a' && numberChar <= 'f')
    {
        bitmap = DigitBitmapArray[10 + numberChar -   'a'];   // char to int
    } else if  (numberChar >= 'A' && numberChar <= 'F')
    {
        bitmap = DigitBitmapArray[10 + numberChar -   'A'];   // char to int
    }
    else if ( numberChar == 'u' || numberChar == 'v' || numberChar == 'U' || numberChar == 'V')
    {
        bitmap = BitMapUV;

    } else if (numberChar == '-')
    {
        bitmap = BitMapMinus;

    } else if (numberChar == 'L' || numberChar == 'l')
    {
        bitmap = DigitBitmapArrayLetterL;
    }
    else if (numberChar == 's' || numberChar == 'S')
    {   // the same as 5
        bitmap = DigitBitmapArrayLetters;
    
    } else if (numberChar == 't')
    {    
        bitmap = DigitBitmapArrayLettert;
    }else if (numberChar == 'y' || numberChar == 'Y' )
    {    
        bitmap = DigitBitmapArrayLettery;
    }
    else if (numberChar == 'o')
    {
        bitmap= DigitBitmapArrayLettero;
    }
    else if (numberChar == 'O' )
    {    
        bitmap = DigitBitmapArrayLetterO;
    } 
    
    return bitmap;
}



void Super_TM1637_Arduino::display(unsigned long num)
{


    display(String(num));

}


void Super_TM1637_Arduino:: displayOverflow()
{
    String overflowString = "";
    for (int i = 0; i < m_maxDisplayDigits; i++)
    {
        overflowString = overflowString + "8.";
    }
    display(overflowString);
}

/*
    void TM1637_Arduino_Chris:: display(String str)
    {
    uint8_t bitmapArray[MaxDigitCount];
    // make a copy of the string so we don't trim the original string
    String trimstr = str;
    trimstr.trim();

    //check if string is longer than the installed digitube digits

    memset(bitmapArray, BitMapEmpty, MaxDigitCount);

    //deal with special cases first
    if (str.length() == 0)
    {
    debugPrint("empty string\n");
    } else if ( trimstr.length() == 0)
    {
    debugPrint("trimmed, empty string\n");

    } else if (str[0] == '.' && str.length() == 1)
    {
    //only one dot
    bitmapArray[0] = BitMapDot;
    }
    else
    {
    // find the end of the number string
    int endIndex = str.length() - 1;
    debugPrint("endIndex is str.length() - 1 ", endIndex);

    debugPrint("endIndex is ", endIndex);
    int bitmapArrayIndex = 0;

    for (int strIndex = 0; strIndex <= endIndex ; strIndex++)
    {
      debugPrint("===============================\n");

      debugPrint(" strIndex is ", strIndex);

      //process from the end of number
      char c = str[endIndex - strIndex];
      debugPrint("endIndex - strIndex   = ", endIndex - strIndex   );

      debugPrint("c is ",  (c));
      if (c == 0) break;

      if (c == '.')
      {
        debugPrint("we have a c==.\n");
        if (strIndex == endIndex )
        {
          //so this is the first dot (From left to right);
          bitmapArray[bitmapArrayIndex]  = BitMapDot;
          debugPrint(" c==. , bitmapArrayIndex ", bitmapArrayIndex);
          displayOverflow();
          return;
        }
        if (bitmapArray[bitmapArrayIndex] == BitMapDot )
        {
          //if this is a 2nd dot, bitmap index need +1, otherwise, if has ..  then bitmaparray index never increase ...
          bitmapArrayIndex++;
        }
        debugPrint(" has dot and strIndex is ", strIndex);
        if (bitmapArrayIndex <  MaxDigitCount )
        {
          bitmapArray[bitmapArrayIndex]  = BitMapDot;
        }
      } else
      {
        bitmapArray[bitmapArrayIndex] |= charToBitMap(c) ;
        bitmapArrayIndex++;
      }
      //if (bitmapArrayIndex >= MaxDigitCount) break;
      debugPrint("bitmapArrayIndex is ", bitmapArrayIndex);

      if ((bitmapArrayIndex >=  m_maxDisplayDigits ) && (strIndex < endIndex) )
      {
        debugPrint("overflow");
        displayOverflow();
        return;
      }


    }
    }


    tmi2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
    tmi2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
    tmi2cDriver_p->startCommand(0x8f);



    }


*/


/*
    void TM1637_Arduino_Chris:: display(String str)
    {
    uint8_t bitmapArray[MaxDigitCount];
    // make a copy of the string so we don't trim the original string
    String trimstr = str;
    trimstr.trim();

    //check if string is longer than the installed digitube digits

    memset(bitmapArray, BitMapEmpty, MaxDigitCount);

    //deal with special cases first
    if (str.length() == 0)
    {
    debugPrint("empty string\n");
    } else if ( trimstr.length() == 0)
    {
    debugPrint("trimmed, empty string\n");

    } else if (str[0] == '.' && str.length() == 1)
    {
    //only one dot
    bitmapArray[0] = BitMapDot;
    }
    else
    {
    // find the end of the number string
    int endIndex = str.length() - 1;
    debugPrint("endIndex is str.length() - 1 ", endIndex);

    debugPrint("endIndex is ", endIndex);
    int bitmapArrayIndex = 0;

    for (int strIndex = 0; strIndex <= endIndex ; strIndex++)
    {
      debugPrint("===============================\n");

      debugPrint(" strIndex is ", strIndex);

      //process from the end of number
      char c = str[endIndex - strIndex];
      debugPrint("endIndex - strIndex   = ", endIndex - strIndex   );

      debugPrint("c is ",  (c));
      if (c == 0) break;

      if (c == '.')
      {
        debugPrint("we have a c==.\n");

        debugPrint(" has dot and strIndex is ", strIndex);
        if (bitmapArrayIndex <  MaxDigitCount )
        {
          bitmapArray[bitmapArrayIndex]  = BitMapDot;
          //if next is still a dot then
          if ( (endIndex > strIndex) && str[endIndex - strIndex - 1] == '.')
          {
            bitmapArrayIndex++;
          } else if (endIndex == strIndex )
          {
            bitmapArrayIndex++;

          }
        }
      } else
      {
        bitmapArray[bitmapArrayIndex] |= charToBitMap(c) ;
        bitmapArrayIndex++;
      }
      //if (bitmapArrayIndex >= MaxDigitCount) break;
      debugPrint("bitmapArrayIndex is ", bitmapArrayIndex);
      if (bitmapArrayIndex > m_maxDisplayDigits )
      {
        displayOverflow();
        return;
      }
    }
    }

    tmi2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
    tmi2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
    tmi2cDriver_p->startCommand(0x8f);

    }


*/




uint8_t Super_TM1637_Arduino:: readKey( )
{

    uint8_t key = tmi2cDriver_p->startCommandAndReadOneByte( Command_SetReadKey);
    if (m_debugPrint) 
    {  debugPrint("read key is ", key);
    }
    return key;
}



void Super_TM1637_Arduino:: setDisplayBuffer(uint8_t *bitmapArray, uint8_t arraySize)
{

    bool success = true;
    int retryCount = 0;
    tmi2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);


    tmi2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);

}


void Super_TM1637_Arduino:: display_from_end(String str)
{
    uint8_t bitmapArray[MaxDigitCount];
    // make a copy of the string so we don't trim the original string
    String trimstr = str;
    trimstr.trim();

    //check if string is longer than the installed digitube digits
    debugPrint("display " , str);

    memset(bitmapArray, BitMapEmpty, MaxDigitCount);

    //deal with special cases first
    if (str.length() == 0)
    {
        debugPrint("empty string\n");
    } else if ( trimstr.length() == 0)
    {
        debugPrint("trimmed, empty string\n");

    } else if (str[0] == '.' && str.length() == 1)
    {
        //only one dot
        bitmapArray[0] = BitMapDot;
    }
    else
    {
        if (checkIfOverflow(str) )
        {
            displayOverflow();
            return;
        }

        // find the end of the number string
        int endIndex = str.length() - 1;
        debugPrint("endIndex is str.length() - 1 ", endIndex);

        debugPrint("endIndex is ", endIndex);
        int bitmapArrayIndex = 0;

        for (int strIndex = 0; strIndex <= endIndex ; strIndex++)
        {
            debugPrint("===============================\n");

            debugPrint(" strIndex is ", strIndex);

            //process from the end of number
            char c = str[endIndex - strIndex];
            debugPrint("endIndex - strIndex   = ", endIndex - strIndex   );

            debugPrint("c is ",  (c));
            if (c == 0) break;

            if (c == '.')
            {
                debugPrint("we have a c==.\n");

                debugPrint(" has dot and strIndex is ", strIndex);
                if ( bitmapArray[bitmapArrayIndex] == BitMapDot)
                {
                    // this is the 2nd dot
                    bitmapArrayIndex++;
                    bitmapArray[bitmapArrayIndex] = BitMapDot;

                } else
                {
                    bitmapArray[bitmapArrayIndex] = BitMapDot;
                }

            } else
            {
                bitmapArray[bitmapArrayIndex] |= charToBitMap(c) ;
                bitmapArrayIndex++;
            }
            //safety guard
            if (bitmapArrayIndex > m_maxDisplayDigits) break;
            debugPrint("bitmapArrayIndex is ", bitmapArrayIndex);

        }
    }
    debugPrint("starting i2c\n");
    //tmi2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
    //tmi2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
    setDisplayBuffer(bitmapArray, MaxDigitCount);
    //tmi2cDriver_p->startCommand(m_currentDisplaySetting);

}

bool Super_TM1637_Arduino::checkIfOverflow(String str)
{
    int req = checkRequiredDigitCount(str);
    if (req > m_maxDisplayDigits)
    {
        return true;
    }
    return false;
}

// if string has : , simply change or enable the certain digit with dp.
void Super_TM1637_Arduino:: display(String str)
{
    static String oldstring="";
    if( oldstring.equals(str))
    {
        //since it's already set, no need to do all the same again
        return;
    }
    oldstring = str;
    
    uint8_t bitmapArray[MaxDigitCount];
    // make a copy of the string so we don't trim the original string
    String trimstr = str;
    trimstr.trim();

    //check if string is longer than the installed digitube digits
    debugPrint("display " , str);

    memset(bitmapArray, BitMapEmpty, MaxDigitCount);

    //deal with special cases first
    if (str.length() == 0)
    {
        debugPrint("empty string\n");
    } else if ( trimstr.length() == 0)
    {
        debugPrint("trimmed, empty string\n");

    } else if (str[0] == '.' && str.length() == 1)
    {
        //only one dot
        bitmapArray[0] = BitMapDot;
    }
    else
    {
        int reqdigits = checkRequiredDigitCount(str);
        if (reqdigits > m_maxDisplayDigits )
        {
            //overflow
            displayOverflow();
            return;
        }

        //// find the end of the number string
        //int endIndex = str.length() - 1;
        //debugPrint("endIndex is str.length() - 1 ", endIndex);

        debugPrint("reqdigits is ", reqdigits);
        int bitmapArrayIndex = m_maxDisplayDigits - reqdigits;
        debugPrint("bitmapArrayIndex is ", bitmapArrayIndex );

        int strIndex = 0;
        for (  bitmapArrayIndex ; bitmapArrayIndex < m_maxDisplayDigits; bitmapArrayIndex++)
        {
            //process from the begining of number
            char c = str[strIndex];
            debugPrint("we have c ", c);
            if (c == 0) break;

            //special case, this char is just a . , not 1. or 2.
            if (c == '.' || c == ':')
            {
                //debugPrint("we have a c==.\n");
                //debugPrint(" has dot and strIndex is ", strIndex);
                bitmapArray[bitmapArrayIndex] = BitMapDot;
                strIndex++;
                //done creating this bitmap
                continue;
            } else    //other char 0--9 , abc...
            {
                debugPrint("else, we have a c==.\n");
                bitmapArray[bitmapArrayIndex] = charToBitMap(c) ;

                strIndex++;
                c = str[strIndex];

                if (c == '.' || c == ':') //now need to check if it has a dot or colon :
                {
                    //debugPrint("2nd step, we have a c==.\n");
                    bitmapArray[bitmapArrayIndex] |= BitMapDot;
                    strIndex++;
                }
            }

        } // end of for

    }

    debugPrint("starting i2c\n");
    //tmi2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
    //tmi2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
    setDisplayBuffer(bitmapArray, MaxDigitCount);
    //tmi2cDriver_p->startCommand(m_currentDisplaySetting);

}



void Super_TM1637_Arduino::display(float f)
{
    String floatString = String(f) ;

    display(floatString);
}



//  if m_maxDisplayDigits is 4,  then 8.8.8.8 is not overflow, 12:34 is not overflow.
// so can not simply check string.length()
//  88888 is overflow
int Super_TM1637_Arduino::checkRequiredDigitCount(String str)
{
    uint8_t bitmapCount = 0;

    String trimbackup = str;
    trimbackup.trim();
    if (trimbackup.length() == 0)
    {   
        
        return 0;
    }

    int strlength=str.length();

    int hasDot = str.indexOf('.');
    int hasColon = str.indexOf(':');

    bool noDotAndToolong = false;
    if ( hasDot < 0 && hasColon < 0  && strlength > m_maxDisplayDigits)
    {
        //there is no dot but more chars than maxDisplayDigits, so it has to be overflow
        return strlength;
    }

    
    //it has dot and it it's more then m_maxDisplayDigits, need to do some calculation
    for (int i = 0; i < strlength; i++)
    {
        char c = str[i];
        if (c == '.')
        {
            //if previous char not exist , +1
            if (i == 0)
            {
                bitmapCount++;
            } else if ( str[i - 1] == '.' )
            {
                //if previous char is . +1,
                bitmapCount++;

            } else
            {
                //if prevous char is other, don't +1
            }
        } else if (c == ':')
        {
           // bitmapCount++;

        }
        else
        {
            bitmapCount++;
        }

        if (bitmapCount > m_maxDisplayDigits)
        {
            //no need to count all if it already exceed the installed digit
            break;
        }
    }

    return bitmapCount;


}

void Super_TM1637_Arduino::switchOnOff(bool on)
{
    debugPrint("onOff is ", on);
    uint8_t displaySetting;
    if (on)
    {
        m_currentDisplayOnOffBit = DisplayOnBit;
        displaySetting = Command_SetDisplayBrightness | DisplayOnBit | m_displayBrightness ;
    } else
    {
        m_currentDisplayOnOffBit = DisplayOffBit;
        displaySetting = Command_SetDisplayBrightness | DisplayOffBit | m_displayBrightness  ;
    }
    debugPrint("onOff command is ", displaySetting );
    tmi2cDriver_p->startCommand( displaySetting );
    delay(1000);


}

void Super_TM1637_Arduino::setBrightness(uint8_t level)
{
    if (level > MaxBrightness)
    {
        level = MaxBrightness;
    }

    m_displayBrightness = level;
    uint8_t commd = Command_SetDisplayBrightness | m_currentDisplayOnOffBit | m_displayBrightness;
    debugPrint("brightness   is ", m_displayBrightness);
    tmi2cDriver_p->startCommand( commd );


}

void Super_TM1637_Arduino::doTestBrightnessLevel()
{   static unsigned long i = 0;
    static bool direction = true;
    digitalWrite(LED_BUILTIN,  i % 2);
    if (direction)
    {
        if (i <  MaxBrightness)
        {
            i += 1;
        } else
        {
            direction = !direction;
        }
    } else
    {
        if (i > 0)
        {
            i--;
        } else
        {
            direction = !direction;
        }
    }

    doTest8888();
    setBrightness(i);
    delay(400);


}

void Super_TM1637_Arduino::doTestSwitchOnOff()
{
    switchOnOff( 0);

    delay(500);

    switchOnOff( 1);

}


void Super_TM1637_Arduino::doTest8888()
{
    static unsigned long i = 0;

    display("8.8.8.8.");
    i++;


    //delay(50);



}

void Super_TM1637_Arduino::doTestIntLoop()
{
    display((unsigned long)0);
    delay(1000);
    for (unsigned long i = 0; i < 10000; i++)
    {
        display(i); //delay(200);

    }

}
void Super_TM1637_Arduino::doTest()
{
    //doTestIntLoop();
    /*
        display("..."); delay(1000);
        display("...."); delay(1000);

        display("....."); delay(1000);
    */
    display("12:34"); delay(10000);
    
        display("......"); //delay(300);
        display("1.2.3.4.5."); //delay(300);

        display("1.2.3.4.5.6"); //delay(300);
        display("1.2.3.4.5.6."); //delay(300);
        display("01.2.3.4.5.6."); //delay(300);





        display("1.2.3.4."); //delay(300);
        display("1.2.3.4.5"); //delay(300);

        display("."); //delay(300);
        display(".."); //delay(300);
        display("..."); //delay(300);
        display("...."); //delay(300);
        display("....."); //delay(300);
        display(" "); //delay(300);
        display("......"); //delay(300);
        display((unsigned long)10); //delay(300);
        display("") ; //delay(300);
        display("1234"); //delay(300);

        //display("1.2.3.4."); delay(500);

        display((float)0.12); //delay(300);
        display((float) - 3.14); //delay(300);
        display((unsigned long)0); //delay(300);
        display((unsigned long)10); //delay(300);
        display((unsigned long)100); //delay(300);
        display((unsigned long)1000); //delay(300);
        display((unsigned long)10001); //delay(300);
        display("");
        display(" "); //delay(300);
        display("-1.2.3."); delay(800);
        display("5.13V"); delay(800);
        display("1.0."); delay(800);
        display("0.1.2.3"); //delay(300);
        display("1.2.3.4."); //delay(300);
        display("1.2.3.4.5."); delay(800);
        display("1.2.3.4.5.6.7.8.9."); delay(800);
        display("."); delay(800);
        display(".."); delay(800);
        display("..."); delay(800);
        display("...."); delay(800);
        display("....."); delay(800);
        display((unsigned long )0); delay(800);
        display( (unsigned long )10); delay(800);
        display("abcd");  delay(800);
        display("defg");  delay(800);
        display("vuVU");  delay(800);
        display(String(10));  delay(800);
        display("-1.2v"); delay(800);

        display("1.2:3.4."); delay(800);

        //doTestIntLoop();



        display("0123");
        delay(1000);

       // showNumber( "020");
        delay(1000);
       // showNumber("");
        delay(1000);
   

}




void Super_TM1637_Arduino::doTestDisplayAndKeys()
{
    static int number = 1;
    //myTM1637.doTest();
    //delay(100000);
    display("12:34");
    delay(1000);
    //    myTM1637.switchOnOff(false);
    setBrightness(1);
    display(String(number));

    do {

        uint8_t key = readKey();
        switch (key )
        {
            case nokey:
                // do nothing
                continue;

            case key1:
                number++;
                break;
            case key2:
                number--;
                break;
            case key3:
                number = 0;
                break;
            case key4:
                number += 30;
                break;
            default :
                continue;
        }
        if (m_debugPrint) 
        {
            debugPrint("read key is ", key); 
        }
        if (key != nokey) // number changed
        {
            display(String(number));
        }
    } while (true);

}


