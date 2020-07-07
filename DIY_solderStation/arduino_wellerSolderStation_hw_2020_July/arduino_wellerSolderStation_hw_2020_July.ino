// DIY Weller solder pen controller.
// magnet detection is included.
//
// this is for the new hardware version PCB  2020 July 3
// kty cold temp compensation is included.
// DIY_weller_solderStationV4.ms14

#include <LiquidCrystal_I2C.h>
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);
// with 12V power supply: total power 50ma, unplug lcd, 23ma
//

#include "superRotaryEncoder.hpp"
#include "DiyWellerSolderStation.hpp"


const bool debug = false;


// ******************    Pin definition   ******************

const int BuzzerEnablePin = 2;
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
const int Power12VDetect = A3; // detect if 12V power is connected.
// ******************    End of Pin definition   ******************

//===========================================
const int EnvTempC = 18;

const float NotConnectedVoltage = 4.9;

//  const float opampGainBlack = OpAmpRopamp2B * 1.0 / OpAmpR1Black + 1.0;
// For simplicity, assume opamp gain for input purple and input black are the same,


const int MIN_TARGET_TEMP = 54;
//#define  MIN_TARGET_TEMP  54
const int MAX_TARGET_TEMP = 360;
//#define  MAX_TARGET_TEMP  380

const unsigned int MaxPowerOnSec =180;
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
  // input pullup useless for A6 A7
  pinMode(OpampOutput1ForPurplePin, INPUT_PULLUP);
  // input pullup useless for A6 A7
  pinMode(OpampOutput2ForBlackPin, INPUT_PULLUP);

  pinMode(MagnetCheckEnablePin, INPUT);
  pinMode(MagnetDetectPin, INPUT);

  // rotary pins init is in rotary class
}


void displayText(String line1, String line2 = "")
{
  //unsigned long startTimeStamp = millis();
  Serial.println();
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
  displayText("  Super Weller", " Solder Station");
  //displayTextOneLine("  Super Weller   Solder Station");
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
  const unsigned long Delta = 600;
  if ((ts - lastTs) > Delta)
  {
    bool newval = digitalRead(Power12VDetect);
    if(lastvalue != newval )
    {
      lastvalue = newval;
      shortBeep();
    }
    lastTs = ts;
  }
  return lastvalue;
}



int getTargetTemperature()
{
  int rotaryValue = rotEncoder.getValue();
  if (debug )
  {
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
  //Serial.println("displayTargetTemp " + printtext);

  lcd.setCursor(temp_x, temp_y);
  lcd.print(printtext);

}


void displayHandleState()
{
  static bool oldIsConnected = true;

  if ( oldIsConnected == station.myWellerState.isConnected)
  {
     return;
  }
  oldIsConnected = station.myWellerState.isConnected;
  String statusText;
  if( station.myWellerState.isConnected)
  {
     statusText = "             ";
  }else
  {
     statusText = "  handle disc?   ";
  }
  const int x = 0, y = 1;
  lcd.setCursor(x, y);
  lcd.print(statusText);
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

void displayStationState()
{
  String statusText = "";
  Serial.println(statusText);
  if ( station.myWellerState.workStatus == WellerSolderControllerStatus::OFF)
  {
    statusText = "OFF  ";
    Serial.println(statusText);
  } else {

    statusText = "ON   ";
  }
  Serial.println(statusText );
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
  Serial.println(statusText);
  if (power)
  {
    statusText = "12V OK";
  } else
  {
    statusText = "No 12V";
  }
  //Serial.println(statusText );
  static String oldstatusText = "";
  if ( statusText == oldstatusText)
  {
    return;
  }
  const int x = 6, y = 0;
  lcd.setCursor(x, y);
  lcd.print(statusText);
  oldstatusText = statusText;
}


void displayOnRestState(bool isonrest)
{
  static bool oldIsOnRest = false;
  if (isonrest == oldIsOnRest)
  {
    //same value, no need to display again.
    return;
  }
  oldIsOnRest = isonrest;
  String text = "";
  if (station.myWellerState.isOnRest)
  {
    text = "on rest";
  } else
  {
    text = "       ";
  }
  const int x = 0;
  lcd.setCursor(x, 1);
  lcd.print(text);
}



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
  lcd.setCursor(9, 1);
  String tweezerTemps = "";
  String red = String(station.myWellerState.redTemp);
  if (station.myWellerState.redTemp < 100)
  {
    red = " " + red;
  }

  String brown = String(station.myWellerState.brownTemp);
  if (station.myWellerState.brownTemp < 100)
  {
    brown = " " + brown;
  }

  tweezerTemps = brown +  " " + red;
  lcd.print(tweezerTemps);
}



void setup()
{
  pinMode(BuzzerEnablePin, OUTPUT);
  beepStart();
  displayInit();
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting");
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
  const unsigned long delta = 700;
  // if previous state is working/Not on rest, check on rest immediately
  bool oldOnRest = station.myWellerState.isOnRest;
  bool newOnRest = oldOnRest;
  if ( !oldOnRest )
  {
    newOnRest = station.isPenPutOnRest();
    lastCheckOnRestTs = onrest_nowts;
  } else
  {
    // if previous state is on rest, delay checking again too soon
    if (( onrest_nowts - lastCheckOnRestTs) > delta)
    {
      newOnRest = station.isPenPutOnRest();
      lastCheckOnRestTs = onrest_nowts;
      //displayOnRestState();
    }// else, just return the current value

  }
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

  bool power12v = checkIs12VPowerConnected();
  display12VPower(power12v);

  if (debug)
  {
    if (power12v)
    {
      Serial.println("12V is connected");
    } else
    {
      Serial.println("12V disconnected");
    }
  }
  //Serial.println("============ loop starts ===============");

  if (station.checkIsFatalError())
  {
    lcd.setCursor(0, 0);
    lcd.print("Fatal Error");
    //Serial.println("Fatal Error!");
    delay(8000);
    return;
  }

  if ( rotEncoder.getButtonClicked())
  {
    //Serial.println("button clicked");
    station.myWellerState.workStatus = !station.myWellerState.workStatus;
    displayStationState( );
    //it's ON but 12V is off??
    shortBeep();
    if (station.myWellerState.workStatus == WellerSolderControllerStatus::ON && !power12v)
    {
      // power is on by 12V is missing
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
  { // ON
    unsigned int nowTsSec = millis()/1000;
    if( g_heaterStartTimerSec == 0)
    {
      //not started
      g_heaterStartTimerSec = nowTsSec;
    }else
    {
      //already started, check Max timeout
      if (  nowTsSec > ( g_heaterStartTimerSec + MaxPowerOnSec ))
      {
        Serial.println("time out" + String(g_heaterStartTimerSec) + " " +String(nowTsSec) );
        turnSolderStationOff(); 
        longBeep();
       }
    }
    if ( debug )
    { 
      Serial.println("Station is ON");
    }

  }
  displayStationState( );
  int targetTemp = getTargetTemperature();
  displayTargetTemp(targetTemp);
  //station.processWellerHandleTemp(targetTemp);
  // have to check temp first to know if it's connected.
  station.checkHandleTemp();

  if (station.myWellerState.isConnected )
  {
    showTweezerTemp();

    //displayHandleState();
    bool isOnRest = checkIsHandleOnRest();
    if (isOnRest)
    {
      // !!! can be revised later, on rest can be in standby mode, 150C maybe.
      // turnSolderStationOff(); 
      // do not set the state off.
      station.disableAllHeaters();
      g_heaterStartTimerSec = 0;
    } else
    {
      station.processWellerHandleTemp(targetTemp);
    }
    displayOnRestState(isOnRest);
  } else
  { // handle is NOT connected
    //disconnect is always off
    turnSolderStationOff();
    // myTM1637.display("DSC");
    // displayStationState( );
    //displayHandleState();
    // Serial.println("disconnected");

  } // end of if ( station.myWellerState.isConnected)
  displayStationState( );
  displayHandleState();

  /*
    // update display
    if (station.myWellerState.workStatus == WellerSolderControllerStatus::OFF )
    {
      //myTM1637.display("OFF");
    } else if (station.myWellerState.workStatus == WellerSolderControllerStatus::ON)
    {
      //myTM1637.display(String(newTargetTemp ));
    }*/
  //delay();
  //Serial.println("============ loop end ===============\n");


  // calculating sleep time to fit the PID algorithm.
  unsigned long endTimeStamp = millis();
  unsigned long timeUsedMs = endTimeStamp - startTimeStamp ;
  const int ShouldDelayms = 1000 / FreqHz;
  long int delta = ShouldDelayms  - timeUsedMs;

  if (delta > 0)
  {
    Serial.println("timeUsedMs: " + String(timeUsedMs) + " sleep for " + String(delta));
    delay(delta);
  }else
  {
     Serial.println("timeUsedMs: " + String(timeUsedMs) + " " + String(delta)+ " no sleep");
  }

}
