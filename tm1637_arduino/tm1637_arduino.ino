
// Pins connect to arduino
#define pin_CLK 2
#define pin_DIO 3


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


void myInit()
{
  pinMode(pin_CLK, OUTPUT);
  pinMode(pin_DIO, OUTPUT);
  digitalWrite(pin_CLK, LOW);
  digitalWrite(pin_DIO, LOW);
  delayCLKDIO();

}





void delayCLKDIO( )
{
  delayMicroseconds(100);
  //delay(1600);
}

void i2cStart()
{
  Serial.print("i2cStart begin\n");
  //pinMode(pin_DIO, OUTPUT);
  digitalWrite(pin_DIO, HIGH);
  delayCLKDIO();

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_DIO, LOW);
  Serial.print("i2cStart done\n");

}

void i2cStop()
{
  Serial.print("stop begin\n");
  digitalWrite(pin_CLK, LOW);
  delayCLKDIO();
  digitalWrite(pin_DIO, LOW);
  delayCLKDIO();

  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_DIO, HIGH);
  Serial.print("stop done\n");

}

void i2cAck()
{
  Serial.print("ack begin\n");
  digitalWrite(pin_CLK, LOW);
  delayCLKDIO();
  unsigned char dio;
  //pinMode(pin_DIO, INPUT);
  do {
    //Serial.print("waiting DIO low\n");
    
    dio = digitalRead(pin_DIO);
  } while (dio);
  
  digitalWrite(pin_CLK, HIGH);
  delayCLKDIO();
  digitalWrite(pin_CLK, LOW);
  Serial.print("ack end\n");
}


void i2cWriteByte(unsigned char oneByte)
{

  unsigned char i = 0;
  const int BitsPerByte = 8; //always 8, dont' change it

  Serial.print("write byte begin\n");
  for (i = 0; i < BitsPerByte; i++)
  {
    Serial.print("for in writeByte ");
    Serial.print(i); Serial.print("\n");

    digitalWrite(pin_CLK, LOW);
    delayCLKDIO();
    digitalWrite(pin_DIO, oneByte & 0x01? HIGH:LOW);
    /*if (oneByte & 0x01)
    {
      digitalWrite(pin_DIO, HIGH);
    } else {
      digitalWrite(pin_DIO, LOW );
    }*/

    delayCLKDIO();
    oneByte = oneByte >> 1;
    digitalWrite(pin_CLK, HIGH);
    delayCLKDIO();

  }
  Serial.print("write byte done\n");
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
  Serial.print("my test begin\n");
  i2cStart();
  i2cWriteByte(Command_Data_Setting_Write_Data_To_Display_Register); //0x40
  i2cAck();
  i2cStop();
  i2cStart();


  i2cWriteByte(TM1637_Starting_Address); //starting address
  Serial.print("write starting addr 0xC0 done\n");
  i2cAck();
  int i;
  for (i = 0; i < 4; i++)
  {
    Serial.print("for in apiTest: ");
    Serial.print(i);
    Serial.print("\n");

    i2cWriteByte( 0b10000001);
    i2cAck();
  }
  i2cStop();

  Serial.print("for is done\n");
  i2cStart();
  i2cWriteByte( 0x8f);
  i2cAck();
  i2cStop();

  Serial.print("my test done\n");

}


void setup()
{
  Serial.begin(9600);      // open the serial port at 9600 bps:
  myInit();
  Serial.print("setup done\n");
  myAPITest();


}

void loop()
{
  
  delay(1000);

}
