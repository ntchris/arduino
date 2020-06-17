// DIY Weller solder pen controller.
// kty cold junction compensation is not done to reduce the project's complexicity; and I don't think it's important.
// magnet detection is included.
//

// this is for the PCB  March 10 2018
// revisiting in June 2020


#include "Super_TM1637_Arduino.hpp"

#include "superRotaryEncoder.hpp"
#include "DiyWellerSolderStation.hpp"


const bool debug = false;


// ******************    Pin definition   ******************
const int HeaterEnableBrown = 9;  // Heater2, Brown, temp move to D9
const int HeaterEnableRed = 3;    // Heater1, Red  ,  move from A4 to D3 temp
const int OpampOutput1ForPurplePin = A5; //detect voltage for purple temp after opamp. For Red heater, with magnet reel
const int OpampOutput2ForBlackPin = A2; // detect voltage for black temp after opamp. For brown heater, with KTY
// magnet detection: pen on handle, magnet make pin 1 gnd and pin 6 purple connected/shorted,  not on handle(using), r=2K.
const int MagnetCheckEnablePin = 6; // D6 Magnet Pullup  , provide a 2.5V to check magnet switch for a short time
const int MagnetDetectPin = A1; // check voltage to see if magnet switch is connected , if so, it's put on handle rest. should shutdown

const int RotaryButtonPin = 12; // using pin change interrupt
const int RotaryPinA = 11;  // using pin change interrupt
const int RotaryPinB = 10;  // using pin change interrupt
const int I2C_SDA = 2;  // D2, for TM1637 display in final version should use sca scl  pins A4
const int I2C_SCL = 8;  // D8, for TM1637 display, was on D3, change to D8 temporary, in final version should use sca scl  pins A5


const int KtyEnablePin = 5;  // D5 , not useful in this version, aka KtyPullup
const int TcInputBlackKty = A6;  //  not useful in this version, aka Kty input

// ******************    End of Pin definition   ******************

//===========================================
const int EnvTempC = 18;

const float NotConnectedVoltage = 4.9;

//  const float opampGainBlack = OpAmpRopamp2B * 1.0 / OpAmpR1Black + 1.0;
// For simplicity, assume opamp gain for input purple and input black are the same,


const int MIN_TARGET_TEMP = 54;
//#define  MIN_TARGET_TEMP  54
const int MAX_TARGET_TEMP = 380;
//#define  MAX_TARGET_TEMP  380

// ****************  For display TM1637 and seven segment digitube  *****************
const int InstallDigits = 3;
Super_TM1637_Arduino myTM1637(I2C_SCL, I2C_SDA, InstallDigits);

SuperRotaryEncoder rotEncoder(RotaryPinA, RotaryPinB, RotaryButtonPin );


DiyWellerSolderStation station(HeaterEnableRed, HeaterEnableBrown,
                               OpampOutput1ForPurplePin, OpampOutput2ForBlackPin,
                               MagnetCheckEnablePin, MagnetDetectPin);


void pinInit()
{

  pinMode(HeaterEnableRed, OUTPUT);
  digitalWrite(HeaterEnableRed, LOW);

  pinMode(HeaterEnableBrown, OUTPUT);
  digitalWrite(HeaterEnableBrown, LOW);

  pinMode(OpampOutput1ForPurplePin, INPUT);
  pinMode(OpampOutput2ForBlackPin, INPUT);
  pinMode(MagnetCheckEnablePin, INPUT);
  pinMode(MagnetDetectPin, INPUT);

  //!!!! put this inside rotary class
  pinMode(RotaryButtonPin, INPUT_PULLUP);

}

void displayInit()
{
  const int DefaultBrightness = 3;

  myTM1637.setBrightness(DefaultBrightness);

  //myTM1637.display("S.t.b.");
}



void encoderInit()
{
  rotEncoder.setEncoderValue(MIN_TARGET_TEMP);
  rotEncoder.setEncoderStep(2);
  rotEncoder.setMinMaxValue( MIN_TARGET_TEMP, MAX_TARGET_TEMP );
}

void solderStationInit()
{

  pinInit();
  displayInit();
  encoderInit();
  // station.initTcAdcIntLookupTable();
  station.selfTestConvertAdcIntToTemperature();
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Starting");
  solderStationInit();

  myTM1637.display("OFF");

}


int getTargetTemperature()
{
  int rotaryValue = rotEncoder.getValue();
  if (  debug )
  {
    Serial.println("targetTemperature: " + String(rotaryValue) );
  }
  return rotaryValue;
}

void refreshDisplayFlashing(int digit)
{
  static int flashingcounter = 0;
  static bool showDecimal = true;
  const int Max = 2;

  if (flashingcounter > Max)
  {
    flashingcounter = 0;
    showDecimal = !showDecimal;

    if (showDecimal)
    {
      myTM1637.display(String(digit) + ("."));

    }
    else
    {
      myTM1637.display(String(digit));
    }

  }

  flashingcounter++;

}



void loop()
{
  unsigned long startTimeStamp = 0;
  // store the last start time stamp

  startTimeStamp = millis();



  //Serial.println("============ loop starts ===============");

  if (station.checkIsFatalError())
  {
    Serial.println("Fatal Error!");
    delay(8000);
    return;
  }

  // Serial.println("checking button");

  if ( rotEncoder.getButtonClicked())
  {
    Serial.println("button clicked");

    station.myWellerState.workStatus = !station.myWellerState.workStatus;

  }

  // solder station initial state is OFF
  if ( station.myWellerState.workStatus == WellerSolderControllerStatus::OFF)
  {
    if ( debug )
    {
    Serial.println("Station is OFF");
    }
    // myTM1637.display("OFF");
    station.disableAllHeaters();
  } else
  {
    Serial.println("Station is ON");
  }

  static int targetTemp = 0;
  int newTargetTemp = getTargetTemperature();
  bool targetTempChanged = false;
  if ( newTargetTemp != targetTemp)
  {
    // connected, not on rest
    //  station.enableHeaterRedPmw(1);
    if ( station.myWellerState.workStatus == WellerSolderControllerStatus::ON )
    {

      targetTempChanged = true;
      targetTemp = newTargetTemp;
    }
  }

  // should we process pwm yet ?
  station.processWellerHandleTemp(targetTemp);

  if (station.myWellerState.isConnected )
  {
    static int stateBeforeStandby  = WellerSolderControllerStatus::UnknownState;;
    // check if magnet on stand
    station.myWellerState.isOnRest = station.isPenPutOnRest();
    if (station.myWellerState.isOnRest )
    {
      if ( stateBeforeStandby == WellerSolderControllerStatus::UnknownState)
      {
        Serial.println("saving old state: " + String(station.myWellerState.workStatus ));
        stateBeforeStandby = station.myWellerState.workStatus;
      }
      station.myWellerState.workStatus = WellerSolderControllerStatus::STANDBY;
      // deal with standby later
      station.myWellerState.workStatus == WellerSolderControllerStatus::OFF;
      station.disableAllHeaters();

      myTM1637.display("S.b.y");
      return;
    } else  // not on rest
    { // restore the state before entering standby
      Serial.println("connected");

      if (stateBeforeStandby != WellerSolderControllerStatus::UnknownState)
      {
        Serial.println("restore old state: " + String(stateBeforeStandby));
        station.myWellerState.workStatus = stateBeforeStandby;
        stateBeforeStandby = WellerSolderControllerStatus::UnknownState;
      }
    }

  } else
  { // handle is disconnected ??
    //disconnect is always off
    station.myWellerState.workStatus = WellerSolderControllerStatus::OFF;
    myTM1637.display("DSC");
    Serial.println("disconnected");
    return;
  } // end of if ( station.myWellerState.isConnected)

  // update display
  if (station.myWellerState.workStatus == WellerSolderControllerStatus::OFF )
  {
    myTM1637.display("OFF");
  } else if (station.myWellerState.workStatus == WellerSolderControllerStatus::ON)
  {
    myTM1637.display(String(newTargetTemp ));
  }
  //delay();
  //Serial.println("============ loop end ===============\n");



  unsigned long endTimeStamp = millis();
  unsigned long timeUsedMs = endTimeStamp - startTimeStamp ;
  const int ShouldDelayms = 1000 / FreqHz;
  long int delta = ShouldDelayms  - timeUsedMs;
  if (delta > 0)
  {
    Serial.println("timeUsedMs: " + String(timeUsedMs) + " sleep for " + String(delta));
    delay(delta);
  }

}
