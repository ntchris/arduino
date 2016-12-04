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

TM1637_Arduino_Chris::TM1637_Arduino_Chris(uint8_t pinClk, uint8_t pinDio)
{
  m_pin_Clk = pinClk;
  m_pin_Dio = pinDio;

  m_debugPrint = false;


  i2cDriver_p = new MyI2CDriver(m_pin_Clk, m_pin_Dio );

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


void TM1637_Arduino_Chris::debugPrint(const char *name, unsigned long int  value)
{
  if (m_debugPrint)
  {
    Serial.print(name);
    Serial.print(value);
    Serial.print("\n");
  }

}

void TM1637_Arduino_Chris::debugPrint(const char *name, const char * str)
{
  if ( m_debugPrint)
  {
    Serial.print(name);
    Serial.print(str);
    Serial.print("\n");
  }

}

/*
  void TM1637_Arduino_Chris::setDisplayBuffer(const uint8_t * dataArray_p, uint8_t digitCount)
  {

  if (!dataArray_p)
  {
    debugPrint("null pointer for indexArray\n");
    return;
  }
  if (digitCount >= MaxDigitCount)
  {
    digitCount = MaxDigitCount;
  }
  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);


  i2cDriver_p-> startCommandData(TM1637_Starting_Address, dataArray_p, MaxDigitCount );
  i2cDriver_p->startCommand(0x8f);

  debugPrint("setDisplayBuffer\n");

  }
*/
/* sorry I don't like it
  // let digit tube to show a number 1234
  void TM1637_Arduino_Chris::showNumber(unsigned long num)
  {
  uint8_t i;
  const unsigned int Ten = 10;
  bool isLeadingZero = true;
  uint8_t bitmapIndexArray[MaxDigitCount];
  uint8_t bitmapArray[MaxDigitCount];

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
    debugPrint(bitmapIndexArray[i]);
    debugPrint(",");
  }
  debugPrint("\n");


  if (!showLeadingZero)
  { //now deal with leading zero
    for (i = MaxDigitCount - 1; i >= 0; i--)
    {
      debugPrint("bitmapIndexArray[i]: ");
      debugPrint(bitmapIndexArray[i]);
      debugPrint("\n");
      if (bitmapIndexArray[i] == 0)
      {
        //this is a leading zero, so use empty bitmap index instead of 0 bitmap index
        debugPrint("replacing 0 to empty\n");
        bitmapIndexArray[i] = Index_Empty_Bitmap;
      }
      else
      {
        //stop processing leading zero once we meet the first non-zero number
        break;
      }
    }
  }

  // can not just use the index array, should use the bitmap array
  for (i = 0; i < MaxDigitCount; i++)
  {

    uint8_t digit = bitmapIndexArray[i];
    debugPrint(digit);
    debugPrint("\n");

    bitmapArray[i] = DigitBitmapArray[digit] ;

  }
  setDisplayBuffer(bitmapArray);

  }


*/


/*
  void TM1637_Arduino_Chris::clearAll()
  {
  const unsigned char clearAllDataArray[MaxDigitCount];
  memset(clearAllDataArray, 0, MaxDigitCount);

  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
  i2cDriver_p->startCommandData( TM1637_Starting_Address, clearAllDataArray, MaxDigitCount);
  }
*/

void TM1637_Arduino_Chris::clearAll()
{
  display("");
}

/*
  uint8_t TM1637_Arduino_Chris::charToInt(char numberChar)
  {
  uint8_t intValue = 255;
  if (numberChar >= '0' && numberChar <= '9' )
  {
    intValue = numberChar -   '0';   // char to int
  } else if (numberChar >= 'a' && numberChar <= 'f')
  {
    intValue = 10 + numberChar -   'a';   // char to int
  } else if  (numberChar >= 'A' && numberChar <= 'F')
  {
    intValue = 10 + numberChar -   'A';   // char to int
  }
  return intValue;
  }
*/

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
  char string[MaxDigitCount+1]; // don't forget the ending 0 for string
  const unsigned int Ten = 10;
  debugPrint("showNumber(num) ", num);

  memset(string, 0, MaxDigitCount+1);

  // special condition for 0
  //zero is special, no matter what it need to show
  if (num == 0)
  {
    display("0");
    return;
  }

  // making a string [] to represent the int number
  for (int i = 0; i < MaxDigitCount; i++)
  {
    uint8_t digit = num % Ten;
    string[MaxDigitCount - i - 1] = digit + '0';
    num = num / Ten;

  }

  debugPrint("number string =  ", string );

  //Now we have a string to represent the integer number.
  //Replacing leading zero with space
  //however if we don't want to show all the leading zero we need to replace them with space
  if (!showLeadingZero)
  {
    for (int i = 0; i < MaxDigitCount; i++)
    {
      debugPrint("string[i]=  ", string[i]);
      //if it's a leading zero then replace it with empty
      if ( string[i] == '0')
      {
        string[i] = ' ';
      } else
      { //we have something else, so from now on it can't be leading zero
        break;
      }

    }
  }

  debugPrint(string);

  display(string);
  debugPrint("end showNumber(num) end\n\n\n");

}


/*

void TM1637_Arduino_Chris::display(unsigned long num)
{
  char string[MaxDigitCount+1]; // don't forget the ending 0 for string
  const unsigned int Ten = 10;
  debugPrint("showNumber(num) ", num);

  memset(string, 0, MaxDigitCount+1);

  // special condition for 0
  //zero is special, no matter what it need to show
  if (num == 0)
  {
    display("0");
    return;
  }

  // making a string [] to represent the int number
  for (int i = 0; i < MaxDigitCount; i++)
  {
    uint8_t digit = num % Ten;
    string[MaxDigitCount - i - 1] = digit + '0';
    num = num / Ten;

  }

  debugPrint("number string =  ", string );

  //Now we have a string to represent the integer number.
  //Replacing leading zero with space
  //however if we don't want to show all the leading zero we need to replace them with space
  if (!showLeadingZero)
  {
    for (int i = 0; i < MaxDigitCount; i++)
    {
      debugPrint("string[i]=  ", string[i]);
      //if it's a leading zero then replace it with empty
      if ( string[i] == '0')
      {
        string[i] = ' ';
      } else
      { //we have something else, so from now on it can't be leading zero
        break;
      }

    }
  }

  debugPrint(string);

  display(string);
  debugPrint("end showNumber(num) end\n\n\n");

}

*/
void TM1637_Arduino_Chris:: display(String str)
{
  uint8_t bitmapArray[MaxDigitCount];
  // make a copy of the string so we don't trim the original string
  String trimstr = str;
  trimstr.trim();
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
      debugPrint("strIndex =", strIndex);
      //process from the end of number
      char c = str[endIndex - strIndex];
      debugPrint("endIndex - strIndex   = ", endIndex - strIndex   );

      debugPrint("c is ",  (c));
      if (c == 0) break;

      if (c == '.')
      {
        if (bitmapArray[bitmapArrayIndex] == BitMapDot )
        {
          //if this is a 2nd dot, bitmap index need +1, otherwise, if has ..  then bitmaparray index never increase ...
          bitmapArrayIndex++;
        }
        debugPrint(" has dot and strIndex is ", strIndex);
        bitmapArray[bitmapArrayIndex]  = BitMapDot;
      } else
      {
        bitmapArray[bitmapArrayIndex] |= charToBitMap(c) ;
        bitmapArrayIndex++;
      }
      if (bitmapArrayIndex >= MaxDigitCount) break;
    }
  }


  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
  i2cDriver_p->startCommandData( TM1637_Starting_Address, bitmapArray, MaxDigitCount);
  i2cDriver_p->startCommand(0x8f);



}


void TM1637_Arduino_Chris::doTest()
{
 
    showLeadingZero=true;
     display(10);delay(1000);
     display(100);delay(1000);
    /*  display(1000);delay(1000);
 
    showLeadingZero=false;
    display(0);delay(1000);
    
    display(10);delay(1000);
    display(100);delay(1000);
     display(1000);delay(1000);

    
     display("");
    display(" "); delay(1000);
    display("-1.2.3."); delay(1000);

  display("5.13V"); delay(1000);

  display("1.0."); delay(1000);

  display("0.1.2.3"); delay(1000);
  display("1.2.3.4."); delay(1000);
  display("1.2.3.4.5."); delay(1000);
  display("1.2.3.4.5.6.7.8.9."); delay(1000);
  display("."); delay(1000);
  display(".."); delay(1000);
  display("..."); delay(1000);
  display("...."); delay(1000);
  display("....."); delay(1000);
  display(0);delay(1000);
  display(10);delay(1000);
    display("abcd");  delay(1000);
    display("defg");  delay(1000);
    display("vuVU");  delay(1000);
    display(String(10));  delay(1000);
    display("-10v"); delay(1000);
 */
  /*

    display("0123");
    delay(1000);

    showNumber( "020");
    delay(1000);
    showNumber("");
    delay(1000);
  */

}
