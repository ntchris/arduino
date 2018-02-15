#include <stdint.h>
#include <arduino.h>

#ifndef __SuperI2CDriver__
#define __SuperI2CDriver__

#ifndef null
#define null 0
#endif
class SuperI2CDriver
{
    //Arduino pins for clk and dio
    uint8_t m_pin_Clk, m_pin_Dio;
    const uint8_t BitsPerByte =8;
    SuperI2CDriver();
    void i2cWriteByte(unsigned char oneByte);
    void i2cStart();
    void i2cStop();
    // true means successful
    bool i2cWaitForAck();
    // send a byte array to i2c, without a new start, also expect the ack from slave. true means successful
    bool i2cWriteByteArray( const unsigned char * array_p, uint8_t arraySize);
    void i2cDelayMicroSecond(unsigned long int microSec = 3 );



  public:
    bool m_debugPrint;

    SuperI2CDriver(uint8_t pinClk, uint8_t pinDio );
    // send a command with start to i2c, wait for ack and stop
    void startCommand(unsigned char command);

    // send a command with start to i2c and a data array, wait for ack and stop
    void startCommandData(unsigned char commandByte, const unsigned char * dataArray_p, uint8_t arraySize);
    uint8_t  startCommandAndReadOneByte(unsigned char commandByte);


    void debugPrint(const char *str);
    void debugPrint(int i);

};


#endif  // __SuperI2CDriver__
