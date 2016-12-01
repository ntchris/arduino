
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

  //===========================================================*/
// Pins connect to arduino
// if use arduino RX, TX (0,1), no pull up resistor is needed, other pins, need pull up resistor for each.
#define pin_CLK 2
#define pin_DIO 12
// if frozen, try to add two 100pf capacitors for each wire

const bool debug = true;

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
};
const uint8_t DotBitmap = 0b10000000;
const uint8_t EmptyBitmap = 0b00000000;




//====================================================================================================================



uint8_t Command_Data_Setting_Write_Data_To_Display_Register = 0b01000000; //0x40
uint8_t Command_Display_And_Control_Command_Setting = 0b10000000; //0x80
uint8_t Command_Address_Command_Setting = 0b11000000; //0xc0
uint8_t TM1637_Starting_Address = 0xc0;



void debugPrint(int i)
{
  if (debug)
  {
    Serial.print(i);
  }

}

void debugPrint(char *str)
{
  if (debug)
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
  debugPrint("i2cStart begin\n");
  digitalWrite(pin_DIO, HIGH);
  delayCLKDIO();

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_DIO, LOW);
  debugPrint("i2cStart done\n");

}

void i2cStop()
{
  debugPrint("stop begin\n");
  digitalWrite(pin_CLK, LOW);
  delayCLKDIO();
  digitalWrite(pin_DIO, LOW);
  delayCLKDIO();

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_DIO, HIGH);
  debugPrint("stop done\n");

}

void i2cAck()
{
  debugPrint("ack begin\n");
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
  debugPrint("ack end\n");
}


void i2cWriteByte(unsigned char oneByte)
{
  const uint8_t BitsPerByte = 8; //always 8, dont' change it
  uint8_t i = 0;

  debugPrint("write byte begin\n");
  for (i = 0; i < BitsPerByte; i++)
  {
    debugPrint("for in writeByte ");
    debugPrint(i); debugPrint("\n");

    digitalWrite(pin_CLK, LOW);
    delayCLKDIO();
    digitalWrite(pin_DIO, oneByte & 0x01 );


    delayCLKDIO();
    oneByte = oneByte >> 1;
    digitalWrite(pin_CLK, HIGH);
    delayCLKDIO();

  }
  debugPrint("write byte done\n");
}




void setDisplayBuffer(uint8_t * indexArray_p, uint8_t digitCount = MaxDigitCount )
{
  debugPrint("my test begin\n");
  if (!indexArray_p)
  {
    debugPrint("null pointer for indexArray\n");
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
  debugPrint("write starting addr 0xC0 done\n");
  i2cAck();
  int i;
  
  //showing digit from the back so the 1st digit is always need to show
  bool isLeadingZero = true;
  for (i = 0 ; i < MaxDigitCount; i++)
  {
    debugPrint("for in apiTest: ");
    debugPrint(i);
    debugPrint(" : ");
    uint8_t digit = indexArray_p[i];
    debugPrint(digit);
    debugPrint("\n");
    if (digit != 0)
    {
      isLeadingZero = false;
      i2cWriteByte( DigitBitmaps[digit]  );

    } else
    {
      if (!showLeadingZero)
      {
        if ( isLeadingZero ) {
          i2cWriteByte( EmptyBitmap );
        }
        else {
          i2cWriteByte( DigitBitmaps[0]  );
        };
      } else
      {
        i2cWriteByte( DigitBitmaps[ 0]  );

      }

    }
    i2cAck();
  }

  i2cStop();
  //digitalWrite(LED_BUILTIN, LOW);

  debugPrint("for is done\n");
  i2cStart();
  i2cWriteByte( 0x8f);
  i2cAck();
  i2cStop();

  debugPrint("my test done\n");

}


// let digit tube to show a number 1234
void showNumber(unsigned long num)
{
  uint8_t i;
  unsigned int divide = 10;
  bool isLeadingZero = true;
  for (i = 0; i < MaxDigitCount; i++)
  {
    uint8_t digit = num % divide;
    bitmapIndexArray[i] = digit;
    num = num / divide;
  }
  setDisplayBuffer(bitmapIndexArray, MaxDigitCount);

}


void myAPITest()
{
  debugPrint("my test begin\n");
  i2cStart();
  i2cWriteByte(Command_Data_Setting_Write_Data_To_Display_Register); //0x40
  i2cAck();
  i2cStop();
  i2cStart();


  i2cWriteByte(TM1637_Starting_Address); //starting address
  debugPrint("write starting addr 0xC0 done\n");
  i2cAck();
  int i;
  for (i = 0; i < 4; i++)
  {
    debugPrint("for in apiTest: ");
    debugPrint(i);
    debugPrint("\n");

    i2cWriteByte( 0b11111111);
    i2cAck();
  }
  i2cStop();
  //digitalWrite(LED_BUILTIN, LOW);

  debugPrint("for is done\n");
  i2cStart();
  i2cWriteByte( 0x8f);
  i2cAck();
  i2cStop();

  debugPrint("my test done\n");

}


void clearAll()
{
  i2cStart();
  i2cWriteByte(Command_Data_Setting_Write_Data_To_Display_Register); //0x40
  i2cAck();
  i2cStop();
  i2cStart();


  i2cWriteByte(TM1637_Starting_Address); //starting address
  debugPrint("write starting addr 0xC0 done\n");
  i2cAck();
  int i;
  for (i = 0; i < 6; i++)
  {
    debugPrint("clearAll for");
    debugPrint(i);
    debugPrint("\n");

    i2cWriteByte(0);
    i2cAck();
  }
  i2cStop();
  debugPrint("clearAll done\n");
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

  if (debug) {
    Serial.begin(9600);      // open the serial port at 9600 bps:
  }

  myInit();
  debugPrint("setup done\n");
  clearAll();

  //setDisplayBuffer(bitmapIndexArray, 4);


  showNumber(9);
  delay(1000);


  /*
  clearAll();

  showNumber(890);
  delay(1000);



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

  delay(1000);

}
