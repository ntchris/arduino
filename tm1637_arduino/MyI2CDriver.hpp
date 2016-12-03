#include <stdint.h>
#include <arduino.h>

#ifndef __MyI2CDriver__
#define __MyI2CDriver__

#ifndef null
#define null 0
#endif
class MyI2CDriver
{
    //Arduino pins for clk and dio
    uint8_t m_pin_Clk, m_pin_Dio;
    MyI2CDriver();
    void i2cWriteByte(unsigned char oneByte);
    void i2cStart();
    void i2cStop();
    void i2cWaitForAck();
    // send a byte array to i2c, without a new start, also expect the ack from slave.
    void i2cWriteByteArray( unsigned char * array_p, uint8_t arraySize);
    void i2cDelayMicroSecond(unsigned long int microSec = 5 );

  public:
    bool m_debugPrint;

    MyI2CDriver(uint8_t pinClk, uint8_t pinDio );
    // send a command with start to i2c, wait for ack and stop
    void startCommand(unsigned char command);

    // send a command with start to i2c and a data array, wait for ack and stop
    void startCommandData(unsigned char commandByte, unsigned char * dataArray_p, uint8_t arraySize);


    void debugPrint(const char *str);
    void debugPrint(int i);

};


#endif  // __MyI2CDriver__
