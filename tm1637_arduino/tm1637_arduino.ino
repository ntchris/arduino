#include "TM1637_Arduino_Chris.hpp"
// Pins connect to arduino
// if use arduino RX, TX (0,1), no pull up resistor is needed, other pins, need pull up resistor for each.
#define pin_CLK 2
#define pin_DIO 12
// if frozen, try to add two 100pf capacitors for each wire

const bool debugLowLevel = false;
const bool debugHighLevel = false;

// TM1637 supports 6 digit-seven-segments display, but you will probably just use a 4 digit tube
const uint8_t MaxDigitCount = 6;

// Do we show the leading zero ? so if 12, show 0012 ?
bool showLeadingZero = false;
uint8_t bitmapIndexArray[ MaxDigitCount ];

const uint8_t DigitBitmaps[] = {
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
const uint8_t Index_Dot_Bitmap = 16;
const uint8_t Index_Empty_Bitmap = 17;



//====================================================================================================================



uint8_t Command_Data_Setting_Write_Data_To_Display_Register = 0b01000000; //0x40
uint8_t Command_Display_And_Control_Command_Setting = 0b10000000; //0x80
uint8_t Command_Address_Command_Setting = 0b11000000; //0xc0
uint8_t TM1637_Starting_Address = 0xc0;





void debugHighLevelPrint(int i)
{
  if (debugHighLevel)
  {
    Serial.print(i);
  }
}


void debugHighLevelPrint(char * str)
{
  if (debugHighLevel)
  {
    Serial.print(str);
  }
}
void debugLowLevelPrint(int i)
{
  if (debugLowLevel)
  {
    Serial.print(i);
  }

}

void debugLowLevelPrint(char *str)
{
  if (debugLowLevel)
  {
    Serial.print(str);
  }

}

void delayCLKDIO( )
{
  //delayMicroseconds(30);
  delayMicroseconds(5);

  //delay(1);
}

void i2cStart()
{
  debugLowLevelPrint("i2cStart begin\n");
  digitalWrite(pin_DIO, HIGH);
  delayCLKDIO();

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_DIO, LOW);
  debugLowLevelPrint("i2cStart done\n");

}

void i2cStop()
{
  debugLowLevelPrint("stop begin\n");
  digitalWrite(pin_CLK, LOW);
  delayCLKDIO();
  digitalWrite(pin_DIO, LOW);
  delayCLKDIO();

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_DIO, HIGH);
  debugLowLevelPrint("stop done\n");

}

void i2cAck()
{
  debugLowLevelPrint("ack begin\n");
  //digitalWrite(LED_BUILTIN, LOW);

  digitalWrite(pin_CLK, LOW);
  delayCLKDIO();

  unsigned char dio;
  bool on = true;
  do {
    dio = digitalRead(pin_DIO);
  } while (dio);
  //The 9th clock high
  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  //The 9th clock low
  digitalWrite(pin_CLK, LOW);
  //digitalWrite(LED_BUILTIN, HIGH);
  //now DIO should be released by TM1637
  debugLowLevelPrint("ack end\n");
}


void i2cWriteByte(unsigned char oneByte)
{
  const uint8_t BitsPerByte = 8; //always 8, dont' change it
  uint8_t i = 0;

  debugLowLevelPrint("write byte begin\n");
  for (i = 0; i < BitsPerByte; i++)
  {
    debugLowLevelPrint("for in writeByte ");
    debugLowLevelPrint(i); debugLowLevelPrint("\n");

    digitalWrite(pin_CLK, LOW);
    delayCLKDIO();
    digitalWrite(pin_DIO, oneByte & 0x01 );


    delayCLKDIO();
    oneByte = oneByte >> 1;
    digitalWrite(pin_CLK, HIGH);
    delayCLKDIO();

  }
  debugLowLevelPrint("write byte done\n");
}




void setDisplayBuffer(uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount )
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
  for (i = 0 ; i < MaxDigitCount; i++)
  {
    debugHighLevelPrint("for in setDisplayBuffer: ");
    debugHighLevelPrint(i);
    debugHighLevelPrint(" : ");
    uint8_t digit = indexArray_p[i];
    debugHighLevelPrint(digit);
    debugHighLevelPrint("\n");

    i2cWriteByte(  DigitBitmaps[digit ]  );

    i2cAck();
  }

  i2cStop();
  //digitalWrite(LED_BUILTIN, LOW);

  debugLowLevelPrint("for is done\n");
  i2cStart();
  i2cWriteByte( 0x8f);
  i2cAck();
  i2cStop();

  debugLowLevelPrint("setDisplayBuffer\n");

}


// let digit tube to show a number 1234
void showNumber(unsigned long num)
{
  uint8_t i;
  const unsigned int Ten = 10;
  bool isLeadingZero = true;
  //clear the previous index buffer
  memset(bitmapIndexArray, 0, MaxDigitCount );
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
    debugHighLevelPrint( bitmapIndexArray[i]);
    debugHighLevelPrint(",");
  }
  debugHighLevelPrint("\n");


  if ( !showLeadingZero)
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
      } else
      {
        //stop processing leading zero once we meet the first non-zero number
        break;
      }
    }
  }

  setDisplayBuffer(bitmapIndexArray);

}




void clearAll()
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



void myInit()
{
  pinMode(pin_CLK, OUTPUT);
  pinMode(pin_DIO, OUTPUT );
  digitalWrite(pin_CLK, LOW);
  digitalWrite(pin_DIO, LOW);
  delayCLKDIO();

}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (debugLowLevel || debugHighLevel) {
    Serial.begin(9600);      // open the serial port at 9600 bps:
  }

  myInit();
  debugHighLevelPrint("setup done\n");
  clearAll();

  //setDisplayBuffer(bitmapIndexArray, 4);

  showNumber(7890);
  delay(300);
  showNumber(20);
  delay(300);


  /*
    clearAll();





    showNumber(123456);
    delay(1000);

    clearAll();

    showNumber(1234);
    delay(1000);


    clearAll();
  */

  digitalWrite(LED_BUILTIN, HIGH);

}

void loop()
{
  TM1637_Arduino_Chris myTM1637(pin_CLK, pin_DIO);


  static unsigned long i = 0;
  i++;
  showNumber(i);
  delay(80);

}
