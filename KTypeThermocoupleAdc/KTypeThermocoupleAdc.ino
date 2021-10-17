#include "KTypeThermocoupleAdc.hpp"

#define Debug
#define test
const unsigned long SerialSpeed = 57600;
const int analogReadPin = A0;

void setup() {

  pinMode( analogReadPin, INPUT);

  // put your setup code here, to run once:
  Serial.begin(SerialSpeed);
  //  KTypeThermalCoupleAdc::initTcTable();

#ifdef Debug
  // Serial.println( KTypeThermalCoupleAdc::Max_Tc_Table_Size);
#endif

#ifdef test
  //KTypeThermalCoupleAdc::testTempLookUpAll();
#endif
}


void printDataValue(String data, String value)
{
  Serial.print( data + " ");
  Serial.println( value);

}

unsigned int adcReadAverage( int analogReadPin, int avgCount = 5 )
{
  unsigned int i, opampValInt = 0;

  for (i = 0; i < avgCount; i++)
  {
    int val;
    delayMicroseconds(100);

    val = analogRead(analogReadPin);
    //Serial.println( val);
    opampValInt += val;
  }
  opampValInt = round(opampValInt * 1.0f / (avgCount * 1.0f));
  Serial.println( "adc int average: "   + String(opampValInt ) );

  return opampValInt;
}


float simpleAdcToTemp(unsigned int adcInt)
{

  const float KTypeMvPerC = 0.04083;
  const float JTypeMvPerC = 0.05362;
  const float OpAmpX = 201;
  const float ADCRef = 5.0f;
  const float ADCINTMax = 1023;
  float opampV = 1.0f * ADCRef * adcInt / ADCINTMax ;
  Serial.println("opamp V is " + String(opampV, 2));
  //mV = 1000*V
  float tcOutputMv = opampV * 1000.0 / OpAmpX;
  //Jtype
  //float tctemp = tcOutputMv/JTypeMvPerC;
  //KType
  float tctemp = tcOutputMv / KTypeMvPerC;

  return tctemp;
}


void loop() {

  static unsigned int prev_temp = 0;
  float temp;
  // put your main code here, to run repeatedly:
  const float env_temp = 21.2;
  unsigned int adcInt = adcReadAverage(analogReadPin);

  temp = KTypeThermocoupleAdc::convertAdcToTempMvPerCPerStage(adcInt);
  temp += env_temp;
  if ( ! isFloatEqual( temp, prev_temp ) )
  {
    // only print if different
    Serial.println( String(temp, 1) );
    prev_temp = temp ;
  }
  delay(1500);

}



/*
  void loop()
  {
   unsigned int adcInt = adcReadAverage(analogReadPin);
   float temp = simpleAdcToTemp(adcInt );
   static float prev_temp;
   const float delta = 0.2f ;
   const int env_temp = 20;

   // only when diff is larger enough:
   if( abs( temp - prev_temp ) > delta )
   {   float finaltemp = env_temp + temp;
       Serial.println("TC temp:"+ String(temp,1) + " env temp:" + String(env_temp) + " final temp: "+ String( finaltemp, 1) );
       prev_temp = finaltemp;
   }

   delay(500);

  }
*/
