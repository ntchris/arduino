#ifndef __KTypeThermocoupleAdc__
#define __KTypeThermocoupleAdc__

// not always want to use lookup table, if not, save the code space
// #define __USE_LOOKUP_TABLE__

#define test
#define Debug

//adc step = 5/1023 = 0.004886
// arduino ADC, minimum ADC input is 20.6mV, 20.6mV : ADC= 1 , 25.1mv : ADC = 2
const float ArduiniAdcMinMv = 20.6;

#include "Arduino.h"


// 240K and 1K opamp , it's 241X gain
const float OpAmpGain = 239.4 + 1;
const float AdcRef = 5.0f;
const float AdcIntMax = 1023;


const float Tc51CMv = 2.064 , Tc101CMv = 4.138, Tc151CMv = 6179, Tc201CMv = 8178, Tc251CMv = 10.194, Tc301CMv = 12.250, Tc351CMv = 14.335;
const float MVPerC_0_50 = 0.04004, MVPerC_50_100 = 0.04079, MVPerC_100_150 = 0.04098, MVPerC_150_200 = 0.04079,
            MVPerC_200_250 = 0.04063, MVPerC_250_300 = 0.04065,  MVPerC_300_350 = 0.04077, MVPerC_350_400 = 0.04092  ;
// meaning TC is not connected
const float MaxTcVoltageMv = 16;


bool isFloatEqual(float a, float b, float delta = 0.1);

class KTypeThermocoupleAdc
{
#ifdef __USE_LOOKUP_TABLE__
    static   float KType_TC_Temperature_Mv_Lookup_Table[];
    static int valueToLookupTableIndex(int value, int startindex, int endindex);
    static void convertTCMvLookupData();
#endif


  public:
    const static unsigned int Max_Tc_Table_Size;
    static const uint8_t TempCPerIndex = 5;
#ifdef __USE_LOOKUP_TABLE__
    static float getTemperatureFromLookupTableAdcInt(unsigned int adcInt);
#endif
    static void testTempLookUp(unsigned int adcInt, unsigned int expected_temp);
    static void testTempLookUpAll();

    static void initTcTable();
    static float convertAdcToTempMvPerC (unsigned int adcInt);
    static float convertAdcIntToVoltage( unsigned int adcInt);
    static float convertAdcToTempMvPerCPerStage(unsigned int adcInt);
};


#endif
