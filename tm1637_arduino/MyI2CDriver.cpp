

#include "MyI2CDriver.hpp"


MyI2CDriver::MyI2CDriver(uint8_t pinClk, uint8_t pinDio )
{
  m_pin_Clk = pinClk;
  m_pin_Dio = pinDio;
  m_debugPrint = true;
  pinMode(m_pin_Clk, OUTPUT);
  pinMode(m_pin_Dio, OUTPUT);
  digitalWrite(m_pin_Clk, LOW);
  digitalWrite(m_pin_Dio, LOW);

}


void MyI2CDriver::debugPrint(const char * str)
{
  if (m_debugPrint)
  {
    Serial.print(str);
  }
}
void MyI2CDriver::debugPrint(int i)
{
  if (m_debugPrint)
  {
    Serial.print(i);
  }

}
void MyI2CDriver::i2cStart()
{
  //debugPrint("i2cStart begin\n");
  digitalWrite(m_pin_Dio, HIGH);
  i2cDelayMicroSecond();

  digitalWrite(m_pin_Clk, HIGH);
  i2cDelayMicroSecond();
  digitalWrite(m_pin_Dio, LOW);
  //debugPrint("i2cStart done\n");

}

void MyI2CDriver::i2cStop()
{
  //debugPrint("stop begin\n");
  digitalWrite(m_pin_Clk, LOW);
  i2cDelayMicroSecond();
  digitalWrite(m_pin_Dio, LOW);
  i2cDelayMicroSecond();

  digitalWrite(m_pin_Clk, HIGH);
  i2cDelayMicroSecond();
  digitalWrite(m_pin_Dio, HIGH);
  //debugPrint("stop done\n");

}

bool MyI2CDriver::i2cWaitForAck()
{
  //debugPrint("ack begin\n");
  //digitalWrite(LED_BUILTIN, LOW);

  digitalWrite(m_pin_Clk, LOW);
  i2cDelayMicroSecond(5);

  unsigned char dio;
  int waitingCount = 0;
  const int MaxRetry = 3;
  do {

    dio = digitalRead(m_pin_Dio);
    waitingCount++;
    if (waitingCount >= MaxRetry)
    {
      //digitalWrite(m_pin_Clk, LOW);

      debugPrint("ack waited too long\n");
      return false;
    }
    //Serial.print(waitingCount );
    //Serial.print("\n");
  } while (dio);
  //The 9th clock high
  digitalWrite(m_pin_Clk, HIGH);
  i2cDelayMicroSecond();
  //The 9th clock low
  digitalWrite(m_pin_Clk, LOW);
  //now DIO should be released
  //debugPrint("ack end\n");
  return true;
}


void MyI2CDriver::i2cWriteByte(unsigned char oneByte)
{
  const uint8_t BitsPerByte = 8; //always 8, dont' change it
  uint8_t i = 0;

  //debugPrint("write byte begin\n");
  for (i = 0; i < BitsPerByte; i++)
  {
    //debugPrint("for in writeByte ");
    //debugPrint(i); debugPrint("\n");

    digitalWrite(m_pin_Clk, LOW);
    i2cDelayMicroSecond();
    digitalWrite(m_pin_Dio, oneByte & 0x01);


    i2cDelayMicroSecond();
    oneByte = oneByte >> 1;
    digitalWrite(m_pin_Clk, HIGH);
    i2cDelayMicroSecond();

  }
  //debugPrint("write byte done\n");
}

void MyI2CDriver::i2cDelayMicroSecond(unsigned long int microSec  )
{
  delayMicroseconds(microSec);
}

bool MyI2CDriver:: i2cWriteByteArray(const unsigned char * array_p, uint8_t arraySize)
{
  if (arraySize <= 0) {
    debugPrint("error: i2cWriteByteArray 0 array size\n");
    return ;
  }
  if (array_p == null) {
    debugPrint("error: i2cWriteByteArray null pionter\n");
    return ;
  }

  bool success = true;

  for (int i = 0; i < arraySize; i++)
  {
    i2cWriteByte(array_p[i]);
    success = i2cWaitForAck();
    if (!success) break;
  }
  return success;
}
void MyI2CDriver:: startCommand(unsigned char commandByte)
{
  pinMode(m_pin_Dio, OUTPUT);

  bool success = true;
  do {
    i2cStart();
    i2cWriteByte(commandByte);
    success = i2cWaitForAck();
    if (!success)
    {
      i2cStop();
    }

  } while (!success);
  i2cStop();
}

void MyI2CDriver::startCommandData(unsigned char commandByte, const unsigned char * dataArray_p, uint8_t arraySize)
{
  bool success = true;
  do
  {
    i2cStart();
    i2cWriteByte(commandByte);
    success = i2cWaitForAck();
    if (!success)
    {
      i2cDelayMicroSecond(3000);
      debugPrint("i2cWriteByte failed, retry\n");
      i2cStop();
      continue;
    }

    success = i2cWriteByteArray(dataArray_p, arraySize );
    if (!success)
    {
      i2cDelayMicroSecond(3000);

      debugPrint("i2cWriteByteArray failed, retry\n");
      i2cStop();
      continue;
    }
  } while (!success);
  i2cStop();
}


uint8_t MyI2CDriver:: startCommandAndReadOneByte(unsigned char commandByte)
{ 
  bool success = true;
  do {
    i2cStart();
    i2cWriteByte(commandByte);
    success = i2cWaitForAck();
    if (!success)
    {
      debugPrint("fail to send read key command");
      i2cStop();
    }else
    {
        debugPrint("read key command success\n");
     }
  } while (!success);

   // very important
   digitalWrite(m_pin_Dio, HIGH);

   uint8_t readkey = 0;
   debugPrint("value is ");

  for (int i = 0; i < BitsPerByte ; i++)
  {
    digitalWrite(m_pin_Clk, LOW);
    
    i2cDelayMicroSecond();
    digitalWrite(m_pin_Clk, HIGH);
    i2cDelayMicroSecond();

    int value = digitalRead(m_pin_Dio);

    readkey<<=1;
    
    debugPrint(value);
    debugPrint(" ");
    
    readkey = readkey | value ;
    
    i2cDelayMicroSecond();

  }
  success = i2cWaitForAck();
  if (!success)
  {
    debugPrint("readkey i2c waitForAck() failed");
   }
  i2cStop();

  return readkey;
}
