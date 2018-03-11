// DIY Weller solder pen controller.
// kty cold junction compensation is not done to reduce the project's complexicity; and I don't think it's important.
// magnet detection is included.
// 

// this is for the PCB  March 10 2018
#include "Super_TM1637_Arduino.hpp" 

#include "superRotaryEncoder.hpp"

// ******************    Pin definition   ******************
const int HeaterEnableABrown = A3;  // Heater2, Brown
const int HeaterEnableARed = A4;    // Heater1, Red
const int OpampOutput1ForPurple = A5;//detect voltage for purple temp after opamp. For Red heater
const int OpampOutput2ForBlack = A2; // detect voltage for black temp after opamp. For brown heater
const int MagnetDetectPin= A1; // check voltage to see if magnet switch is connected , if so, it's put on handle. should shutdown
const int MagnetDetectEnablePin=6;  // D6 Magnet Pullup  , provide a 2.5V to check magnet switch for a short time
const int RotaryButton = 12;
const int RotaryPinA = 10;
const int RotaryPinB = 11;
const int I2C_SDA = 2;  // D2, for TM1637 display
const int I2C_SCL = 3;  // D3, for TM1637 display

const int KtyEnablePin = 5;  // D5 , not useful in this version, aka KtyPullup
const int TcInputBlackKty = A6;  //  not useful in this version, aka Kty input 

// ******************    End of Pin definition   ******************    



//===========================================
const int EnvTempC=19;

const float ADC1024 = 1024.0;
const float VCC5 = 4.97;
const float NotConnectedVoltage = 4.9;
const int NotConnectedVoltageInt = 1015;  // ADC, if 5.0v then it's 1023


const float OpAmpR1Black = 0.991;//1 KOhm
const float OpAmpR2Purple = 0.991; //  1 KOhm
const float OpAmpRopamp2A = 510.0;  //TC Black  // 510K , R 514
const float OpAmpRopamp2B = 510.0;  //TC Black  // 510K , R 514
const float OpampGainPurple = OpAmpRopamp2A * 1.0 / OpAmpR2Purple + 1.0;
const float OpampGain = OpampGainPurple ;

//  const float opampGainBlack = OpAmpRopamp2B * 1.0 / OpAmpR1Black + 1.0;
// For simplicity, assume opamp gain for input purple and input black are the same,


const int TCuVPerC = 11;  //41uV per C  thermal couple type K.

const int MIN_TARGET_TEMP = 20;
const int Default_Target_Temp = 38;


//===============================================================================
// ****************  For display TM1637 and seven segment digitube  *****************
const int InstallDigits = 3;
Super_TM1637_Arduino myTM1637(I2C_SCL, I2C_SDA, InstallDigits);


SuperRotaryEncoder rotEncoder(RotaryPinA, RotaryPinB);



/**
     note: must clean up all flux otherwise ADC reading could be 0 or low while not connected
     (but it should be constantly high or 1023 or 4.9 )
     not connect float 4.9v  int 1023

    R 1.2K   and 220K
    hand temp  0.05V  (opamp pin 1 output)
    XXXXXX  wrong  room temp  float 0.91v  int 187  ????    wrong
    solder not melt temp  float 0.21v  int 42  ????

    both :  room temp =0
    hand:   int =1 , vol =0
    measure 167C,    YiHua 100C (heat gun)  int=38,  vol=0.19
    100C (heat gun)  int=77,  vol=0.38   2:55pm  Feb18 2018



   61C , 0.11 gained,  
   solder melt  int 146 0.7V    95C (wrong ??)

                         63       0.31  (not melt solder)
         150C            98,  0.48


    200C (   )  int=136,  vol=0.66
    250            189        0.92
    300            285        1.39

*/







//=======================================================
 
bool isBlackConnected()
{
  
  
}


bool isHandleConnected()
{
    static bool check_purple = true;
    if ( check_purple )
    {
        check_purple = false;
        return isPurpleConnected();
    }else
    {
       check_purple = true;
       return isBlackConnected();
    }
}

bool isPurpleConnected()
{
    disableHeater( HeaterEnableARed );
    static int oldOpAmpInt = -1;
    int opampValInt = analogRead(OpampOutput1ForPurple);
    Serial.println("opampValInt " + String(opampValInt ));

    if ( opampValInt >= NotConnectedVoltageInt  )
    {
        if (oldOpAmpInt != opampValInt)
        {
            Serial.println("purple not connected. opampVal is " + String(opampValInt ));
        }
        oldOpAmpInt = opampValInt;

        return false;;
    } else
    {
        return true;
    }

}


void pinInit()
{

   pinMode(HeaterEnableARed, OUTPUT );
   digitalWrite(HeaterEnableARed, LOW);


   pinMode(HeaterEnableABrown, OUTPUT );
   digitalWrite(HeaterEnableABrown,LOW);

   // pinMode(MagnetDetectEnablePin, OUTPUT ); // conflict with opamp pin7 output
   //digitalWrite(MagnetDetectEnablePin,LOW);

   pinMode(OpampOutput1ForPurple, INPUT );  // input ?? input_pullup ??
   pinMode(OpampOutput2ForBlack, INPUT );  // input ?? input_pullup ??

 
}

void displayInit()
{
    const int DefaultBrightness = 3;
    
    myTM1637.setBrightness(DefaultBrightness);
    
    myTM1637.display("S.t.b.");
}

void enableHeater(int heaterpin)
{
  
}


void disableHeater(int heaterpin)
{
  
}



void showADCValue(int opampAdcInt)
{
    static int oldOpAmpInt = -1;
    
    if ( abs( oldOpAmpInt - opampAdcInt)>=2  )
    {
        float voltage = VCC5 * opampAdcInt / ADC1024;

        Serial.println("opampValInt:" + String(opampAdcInt) + ", Voltage:" + String(voltage)  );

        float temp = voltageToTempC(voltage, 18 );

        Serial.println("temp: " + String(temp) );

        oldOpAmpInt = opampAdcInt;
    }


}

void setup() {
    // put your setup code here, to run once:
    
    pinInit();
    Serial.begin(9600);
    Serial.println("Starting");
    displayInit();


}



/**

     Thermal Couple K
     41uV/C ,  410uV /10C, 820uV/20C, 2050uV/50C,  4100uV/100C
     8200uV/200C ,    12mv/300C,    16mv/400C,  20.5mv/500C

     Must turn heater OFF before check temp.
*/
bool isTempTooHigh(float opampVoltageLimit)
{
    disableHeater(HeaterEnableARed);
    delay(5);
    int opampValInt = analogRead(OpampOutput1ForPurple);

    showADCValue(opampValInt );

    float voltage = VCC5 * opampValInt / ADC1024;

    if ( voltage >= opampVoltageLimit )
    {

        return true;
    }
    else
    {

        return false;
    }

}


// , float opampVoltage, int envTempC
bool isTempTooHigh_temp(int target_temp)
{
    disableHeater(HeaterEnableARed );
    delay(5);

    const float MAX = 10.0;
    int i;
    float opampAdcInt;
    for(i=0;i<MAX;i++)
    {
       opampAdcInt += analogRead(OpampOutput1ForPurple);
       delay(3);
    }
    opampAdcInt = opampAdcInt /MAX;
    
    showADCValue(opampAdcInt );

    float voltage = VCC5 * opampAdcInt / ADC1024;
    
    float measured_temp = voltageToTempC(voltage, 18);
    if ( measured_temp > target_temp )
    {
        //Serial.println("measured temp: " + String(measured_temp));
        return true;
    } else
    {
        return false;
    }


}


//The opamp voltage to temp
// measured temp  -->
// 41uV/C
float voltageToTempC(float vol, int envTempC)
{
    float thermalJunction_mV = (vol * 1000.0 ) / OpampGain;
    //Serial.println("thermalJunction_mV: " + String(thermalJunction_mV));
    //assume env is always 20C for now
    float envTCVoltage_mv = envTempC * TCuVPerC / 1000; // envTempC voltage ,  uV   18C

    float v1_mv = envTCVoltage_mv + thermalJunction_mV;
    float JunctionTemp = v1_mv * 1000.0 / TCuVPerC ;
    return JunctionTemp;

}


void loop() {
    static int oldval = 0;
    // put your main code here, to run repeatedly:

    bool isconnected = isHandleConnected();
    if( !isconnected )
    {
       myTM1637.display("dsc");

    }

    static float targetTemp = Default_Target_Temp;

    static int old_rotaryValue = 99;
    int rotaryValue = rotEncoder.getEncoderValueSumAccel();
    if ( rotaryValue  != old_rotaryValue )
    {
        const int RotaryToTempDeltaTimes = 5;
        int delta = rotaryValue - old_rotaryValue;
        targetTemp = targetTemp + delta * RotaryToTempDeltaTimes;
        if(targetTemp < MIN_TARGET_TEMP)
        {
           targetTemp = MIN_TARGET_TEMP;  
        }
        Serial.println("setting target temp: " + String(targetTemp));
        old_rotaryValue = rotaryValue;
    }

    if (isconnected )
    {


        //  bool istoohigh = isTempTooHigh(opampVolLimit);
        static bool oldIsTooHigh = false;
        bool istoohigh = isTempTooHigh_temp(targetTemp);
        bool output = false;

        if ( oldIsTooHigh != istoohigh )
        {
            output = true;
            oldIsTooHigh = istoohigh ;
        } else
        {
            output = false;
        }


        if ( istoohigh  )
        {
          
            if (output)
            {   //turn off
                Serial.println("Setting HeaterPin OFF");
                Serial.println("targetTemp: "+String(targetTemp)+ " vs ");
            }
            //disableHeater(HeaterEnableARed);

        } else
        {
            //turn on
            if (output)
            {
                Serial.println("Setting HeaterPin ON");
            }
           // enableHeater(HeaterEnableARed);
        }

    }

    delay(30); 


}
