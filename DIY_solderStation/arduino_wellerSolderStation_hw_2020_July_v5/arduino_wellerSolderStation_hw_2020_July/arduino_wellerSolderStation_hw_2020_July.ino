// DIY Weller solder pen controller.
// magnet detection is included.
//
// this is for the new hardware version PCB  2021 Feb 1
// kty cold temp compensation is included.
// DIY_weller_solderStationV5.ms14

#include <LiquidCrystal_I2C.h>
// set the LCD address to 0x27 for a 16 chars and 2 line display
#define LCDADDRESS 0x3F //0x27 //0x3F
static LiquidCrystal_I2C lcd(LCDADDRESS, 16, 2);
// with 12V power supply: total power 50ma, unplug lcd, 23ma (arduino nano itself)
//

#include "superRotaryEncoder.hpp"
#include "DiyWellerSolderStation.hpp"


const bool debug = true;
const bool TestingPwmAlwaysOn = false;

// ******************    Pin definition   ******************

const int BuzzerEnablePin = 6;
const int I2C_SDA = A4;  // A4, for LCD display SDA
const int I2C_SCL = A5;  // A5, for LCD display SCL


const int HeaterEnableBrown = 3;  // Heater2, Brown,
const int HeaterEnableRed = 9;    // Heater1, Red  ,
const int OpampOutput1ForPurplePin = A7; //detect voltage for purple temp after opamp. For Red heater, with magnet reel
const int OpampOutput2ForBlackPin = A6; // detect voltage for black temp after opamp. For brown heater, with KTY
// magnet detection: pen on handle, magnet make pin 1 gnd and pin 6 purple connected/shorted,  not on handle(using), r=2K.
const int MagnetCheckEnablePin = 5; // D6 Magnet Pullup  , provide a 2.5V to check magnet switch for a short time
const int MagnetDetectPin = A1; // check voltage to see if magnet switch is connected , if so, it's put on handle rest. should shutdown

const int RotaryButtonPin = 10; // using pin change interrupt
const int RotaryPinA = 11;  // using pin change interrupt
const int RotaryPinB = 12;  // using pin change interrupt


const int KtyEnablePin = 4;  // D4, KtyPullup
const int TcInputBlackKty = A2;  //    Kty input, black pin
const int Power12VDetect = 2; // detect if 12V power is connected.
// ******************    End of Pin definition   ******************

//===========================================
const int EnvTempC = 18;

const float NotConnectedVoltage = 4.9;

//  const float opampGainBlack = OpAmpRopamp2B * 1.0 / OpAmpR1Black + 1.0;
// For simplicity, assume opamp gain for input purple and input black are the same,


const int MIN_TARGET_TEMP = 54;
//#define  MIN_TARGET_TEMP  54
const int MAX_TARGET_TEMP = 400;
//#define  MAX_TARGET_TEMP  380

const unsigned int MaxPowerOnSec = 180;
unsigned int g_heaterStartTimerSec = 0;

// ****************  For LCD display **********************
// I2C_SDA = A4;  // A4, for LCD display SDA
// I2C_SCL = A5;  // A5, for LCD display SCL


SuperRotaryEncoder rotEncoder(RotaryPinA, RotaryPinB, RotaryButtonPin );


DiyWellerSolderStation station(HeaterEnableRed, HeaterEnableBrown,
                               OpampOutput1ForPurplePin, OpampOutput2ForBlackPin,
                               MagnetCheckEnablePin, MagnetDetectPin);


void pinInit()
{

  pinMode(Power12VDetect, INPUT);

  pinMode(HeaterEnableRed, OUTPUT);
  digitalWrite(HeaterEnableRed, LOW);

  pinMode(HeaterEnableBrown, OUTPUT);
  digitalWrite(HeaterEnableBrown, LOW);
  // A6 and A7 has no internal pullup
  // input pullup useless for A6 A7
  pinMode(OpampOutput1ForPurplePin, INPUT_PULLUP);
  // A6 and A7 has no internal pullup
  // input pullup useless for A6 A7
  pinMode(OpampOutput2ForBlackPin, INPUT_PULLUP);

  pinMode(MagnetCheckEnablePin, INPUT);
  pinMode(MagnetDetectPin, INPUT);

  // rotary pins init is in rotary class
}


void displayTextOnLcd(String line1, String line2 = "")
{
  //unsigned long startTimeStamp = millis();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  if (line2.length() > 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
  //lcd.noBacklight();
  //unsigned long endts = millis();
  // unsigned long delta = endts - startTimeStamp;
  // time spend is 45ms
  //Serial.println("start ts:"+ String(startTimeStamp) + " end ts:" + String(endts) + " delta:"+String(delta) );
}

void showTitle()
{
  lcd.clear();
  displayTextOnLcd("  Super Weller", " Solder Station");
}

void displayInit()
{
  //const int DefaultBrightness = 3;
  //myTM1637.setBrightness(DefaultBrightness);
  //myTM1637.display("S.t.b.");
  lcd.init();
  lcd.backlight();
  lcd.clear();

}

/*
void findI2CAddress()
{
  Wire.begin();
  byte count = 0;
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);  // maybe unneeded?
    } // end of good response
  } // end of for loop
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println(" device(s).");

}
*/

void encoderInit()
{
  rotEncoder.setEncoderValue(MIN_TARGET_TEMP);
  rotEncoder.setEncoderStep(2);
  rotEncoder.setMinMaxValue( MIN_TARGET_TEMP, MAX_TARGET_TEMP );
  rotEncoder.debug = false;
}


void solderStationInit()
{
  pinInit();
  encoderInit();
  // station.initTcAdcIntLookupTable();
  //station.selfTestConvertAdcIntToTemperature();
}

void beepStart()
{
  digitalWrite(BuzzerEnablePin, HIGH);
}

void beepEnd()
{
  digitalWrite(BuzzerEnablePin, LOW);
}


void beepForTimeMs(unsigned ms)
{
  digitalWrite(BuzzerEnablePin, HIGH);
  delay(ms);
  digitalWrite(BuzzerEnablePin, LOW);
}



void shortBeep()
{
  beepForTimeMs(50);
}


void longBeep()
{
  beepForTimeMs(800);
}


bool checkIs12VPowerConnected()
{
  static bool lastvalue = false;
  static unsigned long lastTs = 0;
  unsigned long ts = millis();
  const unsigned long Delta = 500;
  if ((ts - lastTs) > Delta)
  {
    bool newval = digitalRead(Power12VDetect);
     
    if ( newval)
    {

      if (lastvalue != newval )
      {
        if (debug)
        {
          Serial.println ("12V OK");
        }
        lastvalue = newval;
        //shortBeep();
      }
    } else
    {
      //reduce maximum PWM value ?
      if (debug)
      {
        Serial.println ("12V LOW");
      }
    }

    lastTs = ts;
    lastvalue = newval;
  }
  return lastvalue;
}



int getTargetTemperature()
{
  int rotaryValue = rotEncoder.getValue();
  if (debug )
  {
    // 1715 byte ram ok
    Serial.println("targetTemperature: " + String(rotaryValue) );
  }
  return rotaryValue;
}

void displayTargetTemp(int targettemp)
{
  const int temp_x = 13, temp_y = 0;
  static int oldTarget;
  if (oldTarget == targettemp)
  {
    return;
  }

  String printtext(targettemp);
  if (targettemp < 100)
  {
    // pad the 2 digits number to 3 digits
    printtext = " " + printtext;
  }
  
  lcd.setCursor(temp_x, temp_y);
  lcd.print(printtext);

}


void displayHandleConnectState()
{

  static bool oldIsConnected = true;

  if ( oldIsConnected == station.myWellerState.isConnected)
  {
    return;
  }
  if (debug)
  {
    Serial.println("displayHandleConnectState");
  }
  oldIsConnected = station.myWellerState.isConnected;
  String statusText;
  if ( station.myWellerState.isConnected)
  {
    statusText = "             ";
  } else
  {
    statusText = "  handle disc?   ";
    const int x = 0, y = 1;
    lcd.setCursor(x, y);
    lcd.print(statusText);
  }

}


void turnSolderStationOff()
{
  station.disableAllHeaters();
  g_heaterStartTimerSec = 0;
  station.myWellerState.workStatus = WellerSolderControllerStatus::OFF;
  if ( debug )
  {
    Serial.println("Station is OFF");
  }
}

void displayOnOffState()
{
  String statusText = "";

  if ( station.myWellerState.workStatus == WellerSolderControllerStatus::OFF)
  {
    statusText = "OFF  ";

  } else {

    statusText = "ON   ";
  }
  if (debug)
  {
    Serial.println(statusText);
  }
  static String oldstatusText = "";
  if ( statusText == oldstatusText)
  {
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print(statusText);
  oldstatusText = statusText;
}


void display12VPower(bool power)
{
  String statusText = "";
  
  if (power)
  {
    statusText = "12V OK ";
  } else
  {
    statusText = "12V LOW";
  }
  if(debug) 
  {
    Serial.println("display12VPower " + statusText );
  }
  static String oldstatusText = "";
  if ( statusText == oldstatusText)
  {
    return;
  }
  const int x = 6, y = 0;
  lcd.setCursor(x, y);
  lcd.print(statusText);
  oldstatusText = statusText;
  if (!power)
  {
    //no power, long beep warning
    //longBeep();
  }
}


void displayOnRestState(bool isonrest)
{
 
  //oldIsOnRest = isonrest;
  if (!isonrest)
  {
    if(debug)
    {
      Serial.println(" !isonrest");
    }
    return;
  }

  // if it's onRest, don't print the LCD again too soon.
  static bool oldIsOnRest = false;

  /*
     static unsigned long lastTs = 0;
    unsigned long nowTs = millis();
    const unsigned long Delta = 1000;
    if (( lastTs + Delta )> nowTs )
    {
     return;
    }

    lastTs = nowTs;
  */
  String text1 = "  on ";
  String text2 = " rest";
  const int x1 = 0, x2 = 8;
  lcd.setCursor(x1, 1);
  lcd.print(text1);
  lcd.setCursor(x2, 1);
  lcd.print(text2);
  if(debug)
  {
    Serial.println("display on rest !!");
  }

}

// pwm value from 0 to 255
static const String BarString[] = {">    ", ">>   ", ">>>  ", ">>>> ", ">>>>>"};

String getPwmBarFromPwmValue(byte pwm)
{
  // limit by the lcd screen space
  const int MaxBarTypeCount = 5, MaxPWM = 255;
  const String emptyBar = "     ";
  //const String emptyBar = "empty";
  //const int NoShow=5;
  if (pwm == 0)
  {
    return emptyBar;
  }

  //int index = round((1.0*pwm)/MaxPWM * (MaxBarTypeCount-1));
  int index = map(pwm, 0, MaxPWM, 0, MaxBarTypeCount - 1);
  // should not use empty bar as first item, because no matter how small the pwm is, it need to be shown
  
  return BarString[index];

}
void showHeaterPwmBar()
{
  static unsigned long lastTs = 0;
  unsigned long nowTs = millis();
  const unsigned long Delta = 500;
  if (( nowTs - lastTs ) < Delta )
  {
    return;
  }
  
  lastTs = nowTs;
  const int xpwmbrown = 0, xpwmred = 8;
  lcd.setCursor(xpwmbrown, 1);
  String bar;
  bar = getPwmBarFromPwmValue(station.myWellerState.brownPwm);
  // for  test bar = getPwmBarFromPwmValue(255);
  lcd.print(bar);
 
  bar = getPwmBarFromPwmValue(station.myWellerState.redPwm);
  // for test  bar = getPwmBarFromPwmValue(255);
  lcd.setCursor(xpwmred, 1);
  lcd.print(bar);
 
}


// show tweezer temp and pwm ?
void showTweezerTemp()
{
  //only update temp display once every 800ms, no need to refresh like crazy.
  const unsigned Delta = 800;
  static unsigned long last_ts = 0;
  unsigned long nowts = millis();
  if ( (nowts - last_ts) < Delta)
  {
    return;
  }

  last_ts = nowts;
  String tweezerTemps = "";
  String red = String(station.myWellerState.redTemp);

  String brown = String(station.myWellerState.brownTemp);
  if (station.myWellerState.brownTemp < 100)
  {
    brown = " " + brown;
  }

  if (station.myWellerState.redTemp < 100)
  {
    red = " " + red;
  }

  //tweezerTemps = String(station.myWellerState.brownPwm)+"  "+brown +  "   " + String(station.myWellerState.redPwm)+ "  "+red;
  const int xbrown = 5, xred = 13;
  lcd.setCursor(xbrown, 1);
  lcd.print(brown);
  lcd.setCursor(xred, 1);
  lcd.print(red);

}


void measureEnvTemp()
{
  static unsigned int lastTsSec = 0;
  unsigned int nowTsSec = millis() / 1000;
  const unsigned int DeltaSec = 30;
  if (( lastTsSec + DeltaSec ) < nowTsSec )
  {
    return;
  }
  //Serial.println( "getting env temp");
  lastTsSec = nowTsSec;
  station.disableAllHeaters();
  delay(2);

  pinMode(KtyEnablePin, OUTPUT);
  digitalWrite(KtyEnablePin, HIGH);

  delay(2);
  int ktyInt = station.getAnalogAvgReadingInt(TcInputBlackKty, 3);
  // int ktyInt= analogRead(TcInputBlackKty);
  digitalWrite(KtyEnablePin, LOW);
  // very important, must disable pin high before we leave, or will have wrong adc value for heater temp.
  pinMode(KtyEnablePin, INPUT);
  //Serial.println( "kty int " + String(ktyInt));


  // float vkty = map(ktyInt, 0, 1023, 0, VCC5 );
  double vkty = station.adcIntToVoltage(ktyInt);
  // must not use map, because it returns integer.
  // vkty = map(ktyInt, 0, 1023, 0, VCC5 );

  //Serial.println( "vkty  " + String(vkty));
  double r1k = 989.0; // measure the actual value of the pullup 1k resistor
  float rkty;

  // VCC5 /(rkty+r1k ) *rkty = vkty
  // VCC5 *rkty = vkty *(rkty+r1k)
  // VCC5*rkty = vkty * rkty + vkty *r1k
  // VCC5*rkty - vkty*rkty = vkty*r1k
  // (VCC5-vkty)*rkty = vkty*r1k
  rkty = vkty * r1k / ( VCC5 - vkty);
  //Serial.println("rkty is "+ String(rkty));

  // https://www.nxp.com/docs/en/data-sheet/KTY82_SER.pdf
  // Kty82/110,  assuming 80 Ohm per 10C (approximately), so 8 ohm per 1c
  station.myWellerState.evnTemp = getTempFromKty82_110Resist(rkty);

}

int getTempFromKty82_110Resist(float rkty)
{
  const int Rtemp_sub20 = 684, Rtemp_sub10 = 747, Rtemp_0 = 815, Rtemp_10 = 886, Rtemp_20 = 961,
            Rtemp_25 = 1000, Rtemp_30 = 1040, Rtemp_40 = 1122;
  float temp;
  // simple solution, <10 and >10 two range
  if ( rkty < Rtemp_10)
  {
    // 5 Ohm per C
    float deltaOhm = Rtemp_10 - rkty;
    temp = 10.0 - ( deltaOhm / 7.1);

  } else if (rkty < Rtemp_20)
  { // 10>= C < 20,
    float deltaOhm = rkty - Rtemp_10;
    temp = round(10.0 + deltaOhm / 7.5);
  } else if (rkty < Rtemp_30)
  {
    //c>=20
    float deltaOhm = rkty - Rtemp_20;
    temp = round(20.0 + deltaOhm / 7.9);
  } else
  { // c > 30
    float deltaOhm = rkty - Rtemp_30;
    temp = round(30.0 + deltaOhm / 8.2);
  }
  // "better" solution binary search.
  return temp;
}


void setup()
{
  pinMode(BuzzerEnablePin, OUTPUT);
  digitalWrite(BuzzerEnablePin, LOW);

  pinMode( TcInputBlackKty , INPUT);
  if(debug)
  {
    Serial.begin(115200);
  }

  beepStart();
  
  displayInit();
  // put your setup code here, to run once:

  showTitle();
  beepEnd();
  //delay(1000);
  solderStationInit();
  lcd.clear();

}

bool checkIsHandleOnRest()
{
  static unsigned long lastCheckOnRestTs = 0;
  unsigned long onrest_nowts = millis();
  const unsigned long delta = 600;
  // if previous state is working/Not on rest, check on rest immediately
  bool newOnRest, oldOnRest = station.myWellerState.isOnRest;
  

  if ( oldOnRest )
  {
    if(debug)
    {
      Serial.println(" if oldOnRest");
    }
    // if previous state is on rest, delay checking again too soon
    if (( lastCheckOnRestTs + delta) > onrest_nowts)
    {
      // just return the previous value.
      if(debug)
      {
        Serial.println(" quick return");
      }
      return oldOnRest;
    }
  }
  if(debug)
  {
    Serial.println(" finding out magnet");
  }

  newOnRest = station.isPenPutOnRest();
  lastCheckOnRestTs = onrest_nowts;

  if ( oldOnRest != newOnRest)
  {
    shortBeep();
  }
  return newOnRest;
}

void loop()
{
  // if it's ON too long we shut down it.
  unsigned long startTimeStamp = 0;
  // store the last start time stamp

  startTimeStamp = millis();
  measureEnvTemp();
  bool power12v = checkIs12VPowerConnected();
  if(debug)
  {
    Serial.println("12V is: " + String(power12v));
  }

  if (!power12v)
  {
    if(debug)
    {
      Serial.println("Disable all heater");
    }

    //turn off, so maybe we can use a 12V power supply with less wattage output???
    station.disableAllHeaters();
  }

  if (debug)
  {
    if (power12v)
    {
      Serial.println("12V conn");
    } else
    {
      Serial.println("12V disc");
    }
  }
  display12VPower(power12v);

  if(debug)
  { 
    Serial.println("===loop starts===");
  }

  if ( rotEncoder.getButtonClicked())
  {
    //Serial.println("button clicked");
    station.myWellerState.workStatus = !station.myWellerState.workStatus;
    displayOnOffState( );
    //it's ON but 12V is off??
    shortBeep();
    if (station.myWellerState.workStatus == WellerSolderControllerStatus::ON && !power12v)
    {
      // power is on but 12V is missing
      delay(200);
      shortBeep();
      delay(200);
      shortBeep();
    }
  }

  // solder station initial state is OFF
  if ( station.myWellerState.workStatus == WellerSolderControllerStatus::OFF)
  {
    turnSolderStationOff();
  } else
  { // it's ON
    unsigned int nowTsSec = millis() / 1000;
    if ( g_heaterStartTimerSec == 0)
    {
      //not started
      g_heaterStartTimerSec = nowTsSec;
    } else
    {
      //already started, check Max timeout
      if (  nowTsSec > ( g_heaterStartTimerSec + MaxPowerOnSec ))
      {
        if(debug)
        {
          Serial.println("time out" + String(g_heaterStartTimerSec) + " " + String(nowTsSec) );
        }
        turnSolderStationOff();
        longBeep();
      }
    }
    if ( debug )
    {
      Serial.println("Station is ON");
    }

  }
  int targetTemp = getTargetTemperature();
  displayTargetTemp(targetTemp);
   // have to check temp first to know if it's connected.
  station.checkHandleTemp();
  displayHandleConnectState();
  if(TestingPwmAlwaysOn)
  {
    //station.processWellerHandleTemp(targetTemp);
    analogWrite(HeaterEnableRed, targetTemp);
    analogWrite(HeaterEnableBrown, targetTemp);

    lcd.setCursor(0, 1);
    lcd.print("  Always ON!  ");
    
  }else if (station.myWellerState.isConnected )
  {
    // env temp must be ready before tweezer temp, because later need it.
    showTweezerTemp();

    //displayHandleState();
    bool isOnRest = checkIsHandleOnRest();
    if(debug)
    {
      Serial.println("isOnRest " + String( isOnRest));
    }
    if (isOnRest)
    {
      // !!! can be revised later, on rest can be in standby mode, 150C maybe.
      station.disableAllHeaters();
      g_heaterStartTimerSec = 0;
      displayOnRestState(isOnRest);


    } else // not on rest
    {
      if ( power12v)
      {
        // only turn on heat when we have 12V power
        station.processWellerHandleTemp(targetTemp);
      }

      showHeaterPwmBar();
      

    }
  } else
  { // handle is NOT connected
    
    turnSolderStationOff();

  } // end of if ( station.myWellerState.isConnected)
  displayOnOffState( );
  


  // calculating sleep time to fit the PID algorithm.
  unsigned long endTimeStamp = millis();
  unsigned long timeUsedMs = endTimeStamp - startTimeStamp ;
  const int ShouldDelayms = 1000 / FreqHz;
  long int delta = ShouldDelayms  - timeUsedMs;


  if (delta > 0)
  {
    if(debug)
    {
      Serial.println("timeUsedMs: " + String(timeUsedMs) + " sleep for " + String(delta));
    }
    delay(delta);
  } else
  {
    if(debug)
    {
      Serial.println("timeUsedMs: " + String(timeUsedMs) + " " + String(delta) + " no sleep");
    }
  }

}
