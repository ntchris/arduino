#include "SuperKTypeThermocoupleAdc.hpp"

/*
  // tc temperature -> tc mV -> opAmpX201 ->opAmp Output Voltage -> Adc Integer
  static float KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[] =
  {
  // 0
  0 ,
  //  5       10       15         20         25        30       35        40        45        50
  0.198 , 0.397   , 0.597   , 0.798   , 1.000   , 1.203   , 1.407   , 1.612   , 1.817   , 2.023 ,

  //  55     60         65        70         75          80      85         90        95      100
  2.230   , 2.436   , 2.644   , 2.851   , 3.059   , 3.267   , 3.474   , 3.682   , 3.889   , 4.096 ,

  // 105 to 150
  4.303   , 4.509   , 4.715   , 4.920   , 5.124   , 5.328   , 5.532   , 5.735   , 5.937   , 6.138 ,

  // 155 to 200
  6.339   , 6.540   , 6.741   , 6.941   , 7.140   , 7.340   , 7.540   , 7.739   , 7.939   , 8.138     ,

  // 205 to 250
  8.338   , 8.539   , 8.739   , 8.940   , 9.141   , 9.343   , 9.545   , 9.747   , 9.950   , 10.153    ,


  //255 to 300
  10.357  , 10.561  , 10.766  , 10.971  , 11.176  , 11.382  , 11.588  , 11.795  , 12.001  , 12.209  ,

  //305 to 350
  12.416  , 12.624  , 12.831  , 13.040  , 13.248  , 13.457  , 13.665  , 13.874  , 14.084  , 14.293  ,

  //355 360   365   370   375  380
  14.503  , 14.713  , 14.923  , 15.133  , 15.343  , 15.554  ,

  };


  const static unsigned int KTypeThermalCoupleAdc::Max_Tc_Table_Size =
  sizeof( KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table) /
  sizeof(KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[0]);

*/

/*
  void KTypeThermalCoupleAdc::initTcTable()
  {
    Serial.println("!!!!!!!!!!!!!");
   // step1 convert mv to opamp output voltage

   unsigned int i;
   for(i=0;i<Max_Tc_Table_Size;i++)
   {
     KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[i] = OpAmpGain * KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[i] / 1000;
     // from mv to voltage / 1000
     // now it's voltage
     Serial.println(String(KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[i], 4));
   }

   // step2 convert opamp output to int
   for(i=0;i<Max_Tc_Table_Size;i++)
   {
     KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[i] = AdcIntMax *
              KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[i] / AdcRef ;

     Serial.println(  String(  round(KTypeThermalCoupleAdc::KType_TC_Temperature_Mv_Lookup_Table[i])  ));

   }

  }
*/



static bool isFloatEqual(float a, float b, float delta = 0.1)
{
  if ( abs( a - b) > delta )
  {
    return false;
  }
  else {
    return true;
  }
}




float SuperKTypeThermocoupleAdc::convertAdcIntToVoltage( unsigned int adcInt)
{
 
  if ( adcInt == 0 )
  {
    return 0;
  }
  float adcVoltage = 1.0f * _adcRef * (adcInt - 1)  / _adcIntMax + _adcMinMv / 1000.0;

  float adcVoltageAdj = adcVoltage - (OpAmpMinOutputMv / 1000.0f);
#ifdef DebugThermo
  Serial.println( "adcVoltage V before adjust for min opamp output: "   + String(adcVoltage, 3) );
  Serial.println( "adcVoltage V after adjust min output: "   + String(adcVoltage, 3) );
#endif
  if (adcVoltageAdj < 0)
  {
    adcVoltageAdj = 0;
  }
  return adcVoltageAdj;
}


#ifdef __USE_LOOKUP_TABLE__



/*
  float convertOpAmpOutputVoltageToInputVoltage(float voltage)
  {
   const float OpAmpX = 201;
   float input = voltage / OpAmpX;
   return input;
  }
*/


// using lookup table
float SuperKTypeThermocoupleAdc::getTemperatureFromLookupTableAdcInt(unsigned int adcInt)
{
  float temp = 0;

  float  adjsted = 1;//0.9917;
  float adjustAdcInt = adcInt / adjsted ;
  adcInt = round( adjustAdcInt );
  unsigned int index;
  int resultIndex = KTypeThermocoupleAdc::valueToLookupTableIndex(adcInt, 0,  Max_Tc_Table_Size - 1);

  if (resultIndex  == (Max_Tc_Table_Size - 1))
  {
    // last index
    return resultIndex  * TempCPerIndex;
  } else
  {
    // for map
    float mapAdcInt0 = KType_TC_Temperature_Mv_Lookup_Table[resultIndex ];
    float mapAdcInt1 = KType_TC_Temperature_Mv_Lookup_Table[resultIndex  + 1];
    float maptemp0 = resultIndex  * KTypeThermocoupleAdc::TempCPerIndex;
    float maptemp1 = (resultIndex  + 1) * KTypeThermocoupleAdc::TempCPerIndex;

#ifdef DebugThermo
    Serial.println(" adcInt " + String(adcInt) + ", mapAdcInt0 " + String(mapAdcInt0) + ", mapAdcInt1 " + String(mapAdcInt1) +
                   ", maptemp0 " + String(maptemp0) + ", maptemp1 " + String(maptemp1) );
#endif

    //temp = map(adcInt, mapAdcInt0, mapAdcInt1, maptemp0, maptemp1 );
    // map doesn't work well on 514 adcint, 231C, map gives 230C
    // this is better than map
    double double1 = 1.0;
    temp =  ( maptemp0 + ( (adcInt - mapAdcInt0) * double1 /
                           (double1 * (mapAdcInt1 - mapAdcInt0)) * (maptemp1 - maptemp0) ) );
  }
  return temp;

}

// using TCAdcIntLookupTable, recursive binary search
// return the index that which table[index] < value < table[index+1]
// table values must be in small to large order
// if value is too large, return last index.
// if value is too small, return first index.
int SuperKTypeThermocoupleAdc::valueToLookupTableIndex(int value, int startindex, int endindex)
{
  int index;
  if ( ( endindex - startindex ) <= 1)
  {
    //end of search
    return startindex;
  }

  if (value <=  KType_TC_Temperature_Mv_Lookup_Table[startindex])
  {
    return startindex;
  } else if ( value >= KType_TC_Temperature_Mv_Lookup_Table[endindex] )
  {
    return endindex;
  }

  int midindex = (startindex + endindex) / 2;
  int midValue =  KType_TC_Temperature_Mv_Lookup_Table[midindex];
  if ( value <= midValue )
  {
    index = valueToLookupTableIndex(value, startindex, midindex);
  } else // value > midValue
  {
    index = valueToLookupTableIndex(value,  midindex, endindex);
  }

  return index;
}

#endif


/*
  void KTypeThermalCoupleAdc::testTempLookUp(unsigned int adcInt, unsigned int expected_temp)
  {
  unsigned int temp;
  temp = KTypeThermalCoupleAdc::getTemperatureFromAdcInt(adcInt);
  // printDataValue()
  if (temp == expected_temp)
  {
    Serial.println(String(adcInt) + " pass");
  } else
  {
    Serial.print(String(adcInt));
    Serial.print("Failed!!!! actual " + String(temp) + " but expect " + String(expected_temp));
  }

  }

  void KTypeThermalCoupleAdc::testTempLookUpAll()
  {
  #ifdef test
  testTempLookUp(0, 0);
  testTempLookUp(10, 5);
  testTempLookUp(202, 100);
  testTempLookUp(401, 200);
  testTempLookUp(501, 250);
  testTempLookUp(853, 380);
  //exceed max value
  testTempLookUp(880, 380);

  testTempLookUp(6, 3);
  testTempLookUp(47, 24);

  testTempLookUp(2, 1);
  testTempLookUp(463, 231);



  #endif
  }
*/



float SuperKTypeThermocoupleAdc::convertAdcToTempMvPerC(unsigned int adcInt)
{

  const float KTypeMvPerC = 0.04068;
  const float JTypeMvPerC = 0.05362;

  float adcVoltage = convertAdcIntToVoltage(adcInt);
#ifdef DebugThermo
  
  Serial.println("opamp V is " + String(adcVoltage, 3));
  
#endif
  //mV = 1000*V


  float tcOutputMv = adcVoltage * 1000.0 / _opampGain;
  //Jtype
  //float tctemp = tcOutputMv/JTypeMvPerC;
  //KType
  float tctemp = tcOutputMv / KTypeMvPerC;

  return tctemp;
}


float SuperKTypeThermocoupleAdc::convertAdcToTempMvPerCPerStage(unsigned int adcInt)
{

  double adcVoltage = SuperKTypeThermocoupleAdc::convertAdcIntToVoltage(adcInt);

  //mV = 1000*V


  double tcOutputMv = adcVoltage * 1000.0 / _opampGain;
#ifdef DebugThermo
  Serial.println("Tc mV is " + String(tcOutputMv, 3));
#endif
  
  if ( isFloatEqual( tcOutputMv, 0, 0.1))
  {  
    return 0;
  }


  //KType

  // const float Tc51CMv = 2.064 , Tc101CMv = 4.138, Tc151CMv = 6179, Tc201CMv = 8178, Tc251CMv=10.194, Tc301CMv=12.250, Tc351CMv=14.335;
  //const float MVPerC_0_50 = 0.04004, MVPerC_50_100 = 0.04079, MVPerC_100_150 = 0.04098, MVPerC_150_200 = 0.04079,
  //            MVPerC_200_250 = 0.04063, MVPerC_250_300 = 0.04065,  MVPerC_300_350 = 0.04077, MVPerC_350_400 = 0.04092  ;


  double tctemp ;
  if ( tcOutputMv < Tc51CMv )
  {
    tctemp = tcOutputMv / MVPerC_0_50 ;

  } else if ( tcOutputMv < Tc101CMv )
  {
    tctemp = tcOutputMv / MVPerC_50_100 ;

  } else if ( tcOutputMv < Tc151CMv )
  {
    tctemp = tcOutputMv / MVPerC_100_150 ;

  } else if (tcOutputMv < Tc201CMv )
  {
    tctemp = tcOutputMv / MVPerC_150_200 ;

  } else if (tcOutputMv < Tc251CMv )
  {
    tctemp = tcOutputMv / MVPerC_200_250  ;

  } else if (tcOutputMv < Tc301CMv )
  {
    tctemp = tcOutputMv / MVPerC_250_300  ;

  } else if (tcOutputMv < Tc351CMv )
  {
    tctemp = tcOutputMv / MVPerC_300_350  ;

  } else
  {
    tctemp = tcOutputMv / MVPerC_350_400;
  }

  return tctemp;

}
