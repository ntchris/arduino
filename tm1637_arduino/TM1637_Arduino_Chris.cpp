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
  0b10000000,    // dot
  0b00000000     // empty bitmap
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


 



void TM1637_Arduino_Chris::setDisplayBuffer(uint8_t * dataArray_p, uint8_t digitCount)
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

    bitmapArray[i]= DigitBitmapArray[digit] ;
 
  }
  setDisplayBuffer(bitmapArray);

}

 

void TM1637_Arduino_Chris::clearAll()
{
  unsigned char clearAllDataArray[MaxDigitCount];
  memset(clearAllDataArray, 0, MaxDigitCount);

  i2cDriver_p->startCommand(Command_Data_Setting_Write_Data_To_Display_Register);
  i2cDriver_p->startCommandData( TM1637_Starting_Address, clearAllDataArray, MaxDigitCount);
}

