/*
 5V ----//////------|------//////---- (Ground)
    NTC thermistor  |     R_low side
                    |
              Analog Input Pin
*/							  

#include <Arduino.h>
// tempCelsius = tempKelvin - 273.15
//#define DEBUG_NTC

class SuperNtcThermistor
{
   const float Temp_Kv_To_C_Delta = 273.15;

   float _ntc_beta ;
   float _ntc_room_temp_resistor ;
   // C
   float _room_temp_kv; 
   float _lowResist  ;
   float _max_adc  ; // arduino is 1023, stm32 is not.
   int _adcpin  ;
   
   private: SuperNtcThermistor()
   {
	   // not allowed to use.
   }
   
   public:
    SuperNtcThermistor(int adcpin, float lowSideResistor, float room_temp_c , float roomtempNtcResist, float beta = 3950, int max_adc =1023  )
                  :_adcpin(adcpin), _lowResist(lowSideResistor), _ntc_room_temp_resistor(roomtempNtcResist),  
				  _ntc_beta( beta), _max_adc(max_adc) 
   {
	  _room_temp_kv = room_temp_c + Temp_Kv_To_C_Delta;
   }
    
   void setNtcThermistor(float beta, float roomTemp, float roomtempNtcResist)
   {
	  _ntc_beta = beta;
	  _ntc_room_temp_resistor = roomtempNtcResist;
   }
   
   void setLowSideResistor(float lowResist)
   {
	  _lowResist = lowResist;
   }
      
   
   float adcReadAverage(int avgtimes=5)
   {
	   int i =0;
	   float sum = 0.0f;
	   for (i=0;i<avgtimes;i++)
	   {
		   int adc = analogRead(_adcpin);
		   sum += adc;
		   delayMicroseconds(100);
	   }
	   return sum/avgtimes;
   }
   
   float getNtcTemp()
   {
  
    //const float ROOM_TEMP_Kv = 298.15;
    //const float BETA = 3974;
    //const float RESISTOR_ROOM_TEMP = 10*1000;
    //const float BALANCE_RESISTOR = 10000;

    float adcAvg = adcReadAverage(_adcpin);
    //Serial.print("ntc temp adc:");
    //Serial.println(adcAvg);
    float rThermistor = _lowResist * ( (_max_adc / adcAvg) - 1.0);
    float tKelvin = (_ntc_beta * _room_temp_kv) / 
           (_ntc_beta + (_room_temp_kv * log(rThermistor / _ntc_room_temp_resistor)));
           
    float ntctemp = tKelvin - Temp_Kv_To_C_Delta;
	
#ifdef DEBUG_NTC
  Serial.print("=============  NTC Debug  ===================");
  Serial.print("ntc avg adc int ");
  Serial.print(adcAvg );
	Serial.print(", voltage is ");
 	Serial.print(". ntc thermistor resistance:");
  Serial.print(rThermistor/1000 );
	Serial.print("k. ");
	Serial.println(ntctemp);
#endif

    return ntctemp;
	
   }
};
