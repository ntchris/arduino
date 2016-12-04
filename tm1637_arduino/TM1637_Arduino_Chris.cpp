#include "TM1637_Arduino_Chris.hpp"

const uint8_t TM1637_Arduino_Chris::DigitBitmapArray[]  = {
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

TM1637_Arduino_Chris::TM1637_Arduino_Chris(uint8_t pinClk, uint8_t pinDio, uint8_t digitubeDigits)
{
  m_pin_Clk = pinClk;
  m_pin_Dio = pinDio;
  m_displayBrightness = 4; //default brightness
  m_debugPrint = false;
  m_currentDisplayOnOffBit = DisplayOnBit;

  m_maxDisplayDigits = digitubeDigits;
  uint8_t displaySetting = Command_SetDisplayBrightness | m_displayBrightness | m_currentDisplayOnOffBit ;
  i2cDriver_p = new MyI2CDriver(m_pin_Clk, m_pin_Dio );
  //i2cDriver_p->m_debugPrint = m_debugPrint ;

  i2cDriver_p->startCommand(displaySetting);
}

TM1637_Arduino_Chris:: ~TM1637_Arduino_Chris()
{
  delete i2cDriver_p;
}



void TM1637_Arduino_Chris::debugPrint(int i)
{
  if (m_debugPrint)
  {
    Serial.print(i);
  }

}

void TM1637_Arduino_Chris::debugPrint(const char *str)
{
  if (m_debugPrint)
  {
    Serial.print(str);
  }

}


void TM1637_Arduino_Chris::debugPrint(const String name, unsigned long int  value)
{
  if (m_debugPrint)
  {
    Serial.print(name);
    Serial.print(value);
    Serial.print("\n");
  }

}

void TM1637_Arduino_Chris::debugPrint(const String name, const String str)
{
  if ( m_debugPrint)
  {
    Serial.print(name);
    Serial.print(str);
    Serial.print("\n");
  }

}


void TM1637_Arduino_Chris::clearAll()
{
  display("");
}


uint8_t TM1637_Arduino_Chris::charToBitMap(char numberChar)
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

  }
  return bitmap;
}



void TM1637_Arduino_Chris::display(unsigned long num)
{


  display(String(num));

}


void TM1637_Arduino_Chris:: displayOverflow()
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


  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
  i2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
  i2cDriver_p->startCommand(0x8f);



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

  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
  i2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
  i2cDriver_p->startCommand(0x8f);

  }


*/

void TM1637_Arduino_Chris:: setDisplayBuffer(uint8_t *bitmapArray, uint8_t arraySize)
{

  bool success = true;
  int retryCount=0;
  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);

   
  i2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
   
}


void TM1637_Arduino_Chris:: display(String str)
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
  //i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
  //i2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
  setDisplayBuffer(bitmapArray, MaxDigitCount);
  //i2cDriver_p->startCommand(m_currentDisplaySetting);

}




void TM1637_Arduino_Chris::display(float f)
{
  String floatString = String(f) ;

  display(floatString);
}



//  if m_maxDisplayDigits is 4,  then 8.8.8.8 is not overflow
//  88888 is overflow
bool TM1637_Arduino_Chris::checkIfOverflow(String str)
{
  uint8_t bitmapCount = 0;

  String trimbackup = str;
  trimbackup.trim();
  if (trimbackup.length() == 0)
  {
    return false;
  }

  int strlength = str.length();
  if (strlength <= m_maxDisplayDigits)
  {
    return false;
  }
  int hasDot = str.indexOf('.');
  if (!hasDot &&  strlength > m_maxDisplayDigits)
  {
    //there is no dot but more chars than maxDisplayDigits, so it has to be overflow
    return true;
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
    } else
    {
      bitmapCount++;
    }

    if (bitmapCount > m_maxDisplayDigits)
    {
      return true;
    }
  }

  return false;


}

void TM1637_Arduino_Chris::switchOnOff(bool on)
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
  i2cDriver_p->startCommand( displaySetting );
  delay(1000);


}

void TM1637_Arduino_Chris::setBrightness(uint8_t level)
{
  if (level > MaxBrightness)
  {
    level = MaxBrightness;
  }

  m_displayBrightness = level;
  uint8_t commd = Command_SetDisplayBrightness | m_currentDisplayOnOffBit | m_displayBrightness;
  debugPrint("brightness   is ", m_displayBrightness);
  i2cDriver_p->startCommand( commd );


}

void TM1637_Arduino_Chris::doTestBrightnessLevel()
{

  int i = 4;
  do
  {
    setBrightness(i);
    delay(1000);
    if (i < MaxBrightness)
    {
      i += 1;
    } else
    {
      i = 4;

    }


  } while (true);


}

void TM1637_Arduino_Chris::doTestSwitchOnOff()
{
  switchOnOff( 0);

  delay(500);

  switchOnOff( 1);

}


void TM1637_Arduino_Chris::doTest8888()
{
  static unsigned long i = 0;

  display("8.8.8.8.");
  i++;


  //delay(50);



}

void TM1637_Arduino_Chris::doTestIntLoop()
{
  display((unsigned long)0);
  delay(1000);
  for (unsigned long i = 0; i < 10000; i++)
  {
    display(i); //delay(200);

  }

}
void TM1637_Arduino_Chris::doTest()
{
  //doTestIntLoop();

  display("...."); //delay(300);

  display("....."); //delay(300);

  display("1.2.3.4."); //delay(300);
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
  display("1.2.3.4."); delay(1000);
  display((float)0.12); //delay(300);
  display((float) - 3.14); //delay(300);
  display((unsigned long)0); //delay(300);
  display((unsigned long)10); //delay(300);
  display((unsigned long)100); //delay(300);
  display((unsigned long)1000); //delay(300);
  display((unsigned long)10001); //delay(300);
  display("");
  display(" "); //delay(300);
  display("-1.2.3."); //delay(300);
  display("5.13V"); //delay(300);
  display("1.0."); //delay(300);
  display("0.1.2.3"); //delay(300);
  display("1.2.3.4."); //delay(300);
  display("1.2.3.4.5."); //delay(300);
  display("1.2.3.4.5.6.7.8.9."); //delay(300);
  display("."); //delay(300);
  display(".."); //delay(300);
  display("..."); //delay(300);
  display("...."); //delay(300);
  display("....."); //delay(300);
  display((unsigned long )0); //delay(300);
  display( (unsigned long )10); //delay(300);
  display("abcd");  //delay(300);
  display("defg");  //delay(300);
  display("vuVU");  //delay(300);
  display(String(10));  //delay(300);
  display("-1.2v"); //delay(300);
  //doTestIntLoop();

  /*

    display("0123");
    delay(1000);

    showNumber( "020");
    delay(1000);
    showNumber("");
    delay(1000);
  */

}
