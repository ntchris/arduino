#include <Keyboard.h>


// Pins connect to arduino
#define pin_CLK 2
#define pin_DIO 3

const bool debug = false;


const uint8_t digitToSegment[] = {
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
  0b11111111,    //8.  with a dot
};




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

  //delay(1600);
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

  // pinMode(pin_DIO, INPUT );

  digitalWrite(pin_CLK, LOW);
  delayCLKDIO();


  unsigned char dio;
  bool on = true;
  do {
    //Serial.print("waiting DIO low\n");

    dio = digitalRead(pin_DIO);
  } while (dio);

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_CLK, LOW);
  //digitalWrite(LED_BUILTIN, HIGH);

  debugPrint("ack end\n");
}


void i2cWriteByte(unsigned char oneByte)
{

  unsigned char i = 0;
  const int BitsPerByte = 8; //always 8, dont' change it

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

void showOneDigit()
{


}

void showNumber(int num)
{
  if ( num >= sizeof(digitToSegment))
  {
    //invalid
    return;
  }



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

    i2cWriteByte( 0);
    i2cAck();
  }
  i2cStop();
  debugPrint("clearAll done");
  delay(1000);

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

  Serial.begin(9600);      // open the serial port at 9600 bps:

  myInit();
  debugPrint("setup done\n");
  clearAll();
  myAPITest();
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop()
{

  delay(1000);

}
