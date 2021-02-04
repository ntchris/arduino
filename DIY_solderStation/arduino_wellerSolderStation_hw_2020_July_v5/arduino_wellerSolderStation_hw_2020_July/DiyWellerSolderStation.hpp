#ifndef DiyWellerSolderStation_h
#define DiyWellerSolderStation_h

#include "Arduino.h"
#include "FastPID.h"

// DiyWellerSolderStation

// 12V 4.963--4.965V
// no 12V , only usb: 4.605V
// 12V and USB 4.967V
// using only 12V 5V value for calculation
const float VCC5 = 4.963; // if you change this, must change the excel file as well, column G calculate
const float ADC1023 = 1023.0;
//#define ADC1023 1023.0
//const float OpAmpR1Black = 0.991;  //1 KOhm
const float OpAmpR2Purple = 0.991; //  1 KOhm
const float OpAmpRopamp2A = 510.0;  //TC Black  // 510K , R 514
//const float OpAmpRopamp2B = 510.0;  //TC Black  // 510K , R 514
//const float OpampGainPurple = OpAmpRopamp2A / OpAmpR2Purple + 1.0; // 515.63
//const float OpampGain = OpampGainPurple;
const float OpampGain = OpAmpRopamp2A / OpAmpR2Purple + 1.0; // 515.63

const byte ReadTcDelayMilliSec = 5;

const float Kp = 7.58, Ki = 0.050, Kd = 0.86, FreqHz = 20;
const bool Sign = false;
const int Bits = 8;

/*
  // copied from weller_driver_v0901.c  http://kair.us/projects/weller/index.html
  // AdcInt List for TC temp:
  // opAmpOutput Voltage = tcMv * opAmpGain /1000
  // AdcInt = (opAmpOutput Voltage )/Vcc X 1023
  static const float ThermalCoupleMvPer10C[] =
  { 0, 0.130, 0.263, 0.398, 0.537, 0.677, 0.820, 0.966, 1.114, 1.263, 1.415,
  1.569, 1.725, 1.882, 2.042, 2.203, 2.365, 2.529, 2.694, 2.860, 3.028,
  3.196, 3.366, 3.536, 3.707, 3.879, 4.052, 4.225, 4.398, 4.572, 4.746,
  4.920, 5.094, 5.268, 5.442, 5.616, 5.790, 5.963, 6.135, 6.307, 6.479,
  6.649, 6.819, 6.988, 7.155, 7.322, 7.487, 7.652, 7.814, 7.975, 8.135
  };
  const int TcTableSize =  sizeof(ThermalCoupleMvPer10C)/sizeof(ThermalCoupleMvPer10C[0]);
*/

const uint8_t TempCPerIndex = 10;

/*
  static int TCAdcIntLookupTable[] =
  {
  0, 14, 28, 43, 57,   72, 88, 103, 119, 135,   151, 168, 184, 201, 218,  235, 253, 270, 288, 305,
  323, 341, 359, 378, 396,   414, 433, 451, 470, 488,   507, 525, 544, 563, 581,  600, 618, 637, 655, 673,
  692, 710, 728, 746, 764,   782, 800, 817, 834, 852,    869
  };
*/
//using 4.963 as Vref
// data from excel file: Weller_WMRP_and_WMRT_thermocouple_voltage_vs_temperature.xlsx

/*static unsigned int TCAdcIntLookupTable[] =
  { 0,14,28,42,57,   72,87,103,118,134,   150,167,183,200,217,   234,251,269,286,304,   322,340,358,376,394,
  412,431,449,467,486,   504,523,541,560,578,  597,615,634,652,670,   689,707,725,743,761,
  778,796,813,831,848,  865
  };
*/



//=======================================================
class WellerSolderControllerStatus
{
  public:
    static const byte OFF = 0, ON = 1, UnknownState = 3;
    int redTemp;
    int brownTemp;
    bool isPurpleConnected;
    bool isBlackConnected;
    bool isConnected;
    bool isOnRest;
    int workStatus;
    byte redPwm;
    byte brownPwm;
    int evnTemp = 18;

};


class DiyWellerSolderStation
{
    static unsigned int TCAdcIntLookupTable[];
    // make sure the table size is small enough!  < 255,  otherwise must use int
    static const unsigned char  TcTableSize;// = sizeof(DiyWellerSolderStation::TCAdcIntLookupTable) / sizeof(DiyWellerSolderStation::TCAdcIntLookupTable[0]);
    // 51;
    int valueToLookupTableIndex(int value, int startindex, int endindex);



    static const bool debug = false;

    byte heaterRedEnablePin = 0;
    byte heaterBrownEnablePin = 0;
    // purple wire( heater red temp thermal couple wire) to oamp and to arduino
    byte purpleOpAmpReadPin = 0;
    // black wire (heater brown temp thermal couple wire) to oamp and to arduino
    byte blackOpAmpReadPin = 0;
    byte _magnetEnablePin = 0;
    byte _magnetDetectPin = 0;
    // don't check red and brown everytime,
    // check one each time, check the other next time.
    bool opampReadIntToIsConnected(int opampValInt);

    void processRedHeaterTemp();
    void processBrownHeaterTemp();

  public:
    float adcIntToVoltage(int adcInt);

    DiyWellerSolderStation(int redEnPin, int brownEnPin,
                           int purpleOpAmOutPin, int blackOpAmOutPin,
                           int magnetEnPin, int magnetDetectPin);
    // should be private
    void initTcAdcIntLookupTable();

    WellerSolderControllerStatus myWellerState;
    //bool checkIsFatalError();
    void disableHeaterRed();
    void disableHeaterBrown();
    void disableAllHeaters();
    void disableHeater(int heaterEnablePin);
    void enableHeater(int heaterEnablePin);
    void enableHeaterRed();
    void enableHeaterBrown();

    bool checkIsHandleDisconnected();
    int getPurpleOpAmpOutputInt();
    int getBlackOpAmpOutputInt();
    //void updateHeaterRedTemp();
    //void updateHeaterBrownTemp();
    //void updateHandleState();
    bool isPenPutOnRest();

    int getOpampOutputIntToTemperature(int analogReadingInt);

    static int getAnalogAvgReadingInt(byte analogReadPin, byte avgCount);
    int convertAdcIntToTemperature(int adcInt);
    bool selfTestConvertAdcIntToTemperature();
    int getEnvTemperature();
    void setHeaterRedPmw(int pwm);
    void setHeaterBrownPmw(int pwm);
    int getHeaterRedTemp();
    int getHeaterBrownTemp();
    void checkHandleTemp();
    // for main loop.
    // do not process red and brown each turn, do it every other time.
    void processWellerHandleTemp(int targettemp);

};


#endif
