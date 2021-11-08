// for NTC Thermistor
#include "SuperNtcThermistor.h"

// for KType Thermocouple
#include "SuperKTypeThermocoupleAdc.hpp"

#include "TempProfile.hpp"


// for store setting in eeprom


#include <LiquidCrystal_I2C.h>

// for rotary encoder
#include "superRotaryEncoder.hpp"

// for PID
#include "FastPID.h"


const bool debug = true;


// ==========  MCU Pin def  ==============

// for NTC
#define NTC_CHECK_ENABLE_PIN A3
#define NTC_ADC_PIN A2


#define BUZZER_PWM_PIN 5
#define DETECT_20V_PIN A0

#define HeaterPwmPin 3

#define HeatingPlateThermocoupleAdcPin A7

#define I2C_SDA A4;  // A4, for LCD display SDA
#define I2C_SCL A5;  // A5, for LCD display SCL


// ==========  End of MCU Pin def  ==============



const float BALANCE_RESISTOR = 9.97 * 1000;
const float NTC_VALUE_Room_Temp_C = 18;// the room temp when measure the NTC resistance
const float RoomtempNtcResist  = 12.94 * 1000; //the NTC resist at above temp;
const float NTC_BETA = 3950;
SuperNtcThermistor superNTC(NTC_ADC_PIN, BALANCE_RESISTOR, NTC_VALUE_Room_Temp_C, RoomtempNtcResist, NTC_BETA);

#define LCDADDRESS 0x27 //0x3F
static LiquidCrystal_I2C lcd(LCDADDRESS, 16, 2);

const int cH = 523;

const int gSH = 830;
const int aH = 880;



#define CheckRoomTempIntervalSec 5
float g_room_temp = 20;

#define OpAmpGain 243.2f
#define OpAmpMinOutputMv 135
#define Vref 5.024
SuperKTypeThermocoupleAdc kTypeThermocoupleAdc(OpAmpGain, OpAmpMinOutputMv, Vref );

#define RotaryPinA 9
#define RotaryPinB 8
#define RotaryButtonPin 7
SuperRotaryEncoder rotEncoder(RotaryPinA, RotaryPinB, RotaryButtonPin );

SmtTempProfile g_smtprofile1("P1");
SmtTempProfile g_smtprofile2("P2");
SmtTempProfile *g_activeProfile;


// pwm value from 0 to 255
static const String BarString[] = {">    ", ">>   ", ">>>  ", ">>>> ", ">>>>>"};


int g_heaterPwm = 0;

int g_workStateOn = false;

unsigned long g_heaterStartTimeStampSec = 0;

unsigned long g_ReachTempStageTimeStampSec = 0;

const int MIN_TARGET_TEMP = 18;

const int MAX_TARGET_TEMP = 350;




// safety feature
const unsigned int MaxPowerOnSec = 300;


//const float Kp = 7.58, Ki = 0.050, Kd = 0.86, FreqHz = 20;
const float Kp = 20, Ki = 1, Kd = 1, FreqHz = 20;

const bool Sign = false;
const int Bits = 8;
FastPID tempPid(Kp, Ki, Kd, FreqHz, Bits, Sign);


int g_thermoTempC = 0;
int g_targetTemp = 0;
int g_stageTimeLengthSec = 0;



int getEncoderReading()
{
  int rotaryValue = rotEncoder.getValue();
  if (debug )
  {

    Serial.println("getEncoderReading:" + String(rotaryValue) );
  }
  return rotaryValue;
}



class Gui_Options
{
    int _gui_option_index = Gui_Loc_Temp_Profile ;
    bool isChangeSetting = false;

  public:
    static const int Gui_Loc_Temp_Profile = 0;
    static const int Gui_Loc_OnOff = 1;
    static const int Gui_Loc_Stage = 2;
    static const int Gui_Loc_Temp = 3;
    static const int Gui_Loc_TimeSec = 4;
    static const int Gui_Loc_Max = 4;

    int getGuiIndex()
    {
      return _gui_option_index;
    }

    bool getIsChangeSetting()
    {
      return isChangeSetting;
    }
    void toggleIsChangeSetting()
    {
      Serial.println("isChangeSetting " + String(isChangeSetting));
      isChangeSetting = !isChangeSetting;
    }


    void displayGuiIndicator(  )
    {
      const int X_Profile = 0, Y1 = 0;
      const int X_OnOff = 4, Y2 = 0;
      const int X_Stage = 0, Y3 = 1;
      const int X_temp = 4 ;
      const int X_time = 10, Y4 = 1;
      lcd.setCursor(X_Profile, Y1);
      lcd.print(" ");
      lcd.setCursor(X_OnOff, Y2);
      lcd.print(" ");
      lcd.setCursor(X_Stage, Y3);
      lcd.print(" ");
      lcd.setCursor(X_temp, Y4);
      lcd.print(" ");
      lcd.setCursor(X_time, Y4);
      lcd.print(" ");

      if ( _gui_option_index == Gui_Loc_Temp_Profile)
      {

        lcd.setCursor(X_Profile, Y1);


      } else if ( _gui_option_index  == Gui_Loc_OnOff )
      {

        lcd.setCursor(X_OnOff, Y2);
      } else if ( _gui_option_index == Gui_Loc_Stage)
      {
        lcd.setCursor(X_Stage, Y3);
      }
      else if (  _gui_option_index  == Gui_Loc_Temp )
      {

        lcd.setCursor(X_temp, Y3);

      } else if ( _gui_option_index  == Gui_Loc_TimeSec  )
      {

        lcd.setCursor(X_time, Y4);
      }

      Serial.println(">!!!" + String(_gui_option_index  ) + " " + String(getIsChangeSetting()));
      if (  ( ( _gui_option_index  == Gui_Loc_TimeSec)  || (_gui_option_index  == Gui_Loc_Temp)) && getIsChangeSetting())
      {
        // if we are changing temp /time setting
        lcd.print(">");
      } else
      {

        lcd.print("*");
      }
    }

    void updateGuiPointer()
    {
      static int prev_index = -1;
      if ( !getIsChangeSetting() )
      {
        _gui_option_index = getEncoderReading();
      }
      if ( _gui_option_index != prev_index )
      {
        prev_index = _gui_option_index;
        //when encode is rotated, not clicked.
        displayGuiIndicator();

      }


    }
};

Gui_Options gui_options;

void buzzerPwmOn( int pwm)
{
  analogWrite( BUZZER_PWM_PIN, pwm);

}

void buzzerPwmOff( )
{
  analogWrite( BUZZER_PWM_PIN, 0);
}



float detectNTCTemp()
{
  digitalWrite(NTC_CHECK_ENABLE_PIN, HIGH);
  delayMicroseconds(100);
  float ntctemp = superNTC.getNtcTemp();

  // ntc is mounted on PCB, so temp is a bit higher than room temp.
  // adjust it a little bit
  const float NTCRoomTempAdj = 1;
  ntctemp -= NTCRoomTempAdj;
  digitalWrite(NTC_CHECK_ENABLE_PIN, LOW);
  return ntctemp;
}




float adcReadAverage( int analogReadPin, int avgCount = 5 )
{
  unsigned int i;
  float opampVal = 0;

  for (i = 0; i < avgCount; i++)
  {
    int adc;
    delayMicroseconds(50);
    adc = analogRead(analogReadPin);
    opampVal += adc;
  }
  opampVal =  opampVal  / (avgCount * 1.0f);

  return opampVal;
}


bool checkIs20VOK()
{

  int v20_OK = digitalRead(DETECT_20V_PIN);
  // with Zener, if Rup/Rdown is too large value, voltage drop below zener.
  //int adc = analogRead(Detect20V);
  //const int AdcMax=1023;
  //float v = 5.0*adc/AdcMax;
  //Serial.println(v);
  if (v20_OK)
  {
    //Serial.println("20V is OK");
  } else
  {
    //Serial.println("20V OFF!");
  }

  return v20_OK;
}


void setHeaterPwm(int pwm)
{
  analogWrite(HeaterPwmPin, pwm);
}

void disableHeater()
{
  digitalWrite(HeaterPwmPin, LOW);
}


// must be short tone
// because we are using delay to make it simple.
void shortBeep(int delayMs = 50, int pwm = aH)
{
  analogWrite(BUZZER_PWM_PIN, pwm);
  delay(delayMs);
  analogWrite(BUZZER_PWM_PIN, 0);
}

void twoShortBeep()
{
  shortBeep(20);
  buzzerPwmOff( );
  delay(200);
  shortBeep(20);
  buzzerPwmOff( );
}

void longBeep(int pwm = cH)
{
  const int LongBeepDelayMs = 800;
  analogWrite(BUZZER_PWM_PIN, pwm);
  delay(LongBeepDelayMs);
  analogWrite(BUZZER_PWM_PIN, 0);
}


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


int getThermocoupleTemp()
{
  // must disable heater to get opAmp reading
  // why?
  disableHeater();
  // must delay more than 60microseconds to get an accurate reading from the opamp
  delayMicroseconds(100);

  float adc = adcReadAverage(HeatingPlateThermocoupleAdcPin);
  setHeaterPwm(g_heaterPwm );

  float tctemp =  kTypeThermocoupleAdc.convertAdcToTempMvPerCPerStage( adc);

  float finaltemp = tctemp + g_room_temp;

  int thermoC = round(finaltemp);
  return thermoC;
}



void displayTextOnLcd(String line1, String line2 = "")
{

  lcd.setCursor(0, 0);
  lcd.print(line1);
  if (line2.length() > 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }

}

void showTitle()
{
  lcd.clear();
  displayTextOnLcd("   Super SMT ", " Heating Plate ");
}

void displayInit()
{
  //const int DefaultBrightness = 3;

  lcd.init();
  lcd.backlight();
  lcd.clear();

}



void showHeaterTemp(int temp)
{
  static int pre_temp = 0;
  if (pre_temp == temp)
  {
    return;
  }
  pre_temp = temp;

  //only update temp display once every 800ms, no need to refresh like crazy.
  const unsigned Delta = 800;
  static unsigned long last_ts = 0;
  unsigned long nowts = millis();
  if ( (nowts - last_ts) < Delta)
  {
    return;
  }

  last_ts = nowts;
  String tempString(temp);

  if (temp < 100)
  {
    tempString = " " + tempString;
  }

  const int xtemp = 13;
  lcd.setCursor(xtemp, 0);
  lcd.print(tempString);

}



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


void showHeaterWorkStatePwmBar( )
{

  static unsigned long lastTs = 0;
  unsigned long nowTs = millis();
  //only refresh every 500ms, don't refresh too fast, no use.
  const unsigned long Delta = 300;
  if (( nowTs - lastTs ) < Delta )
  {
    return;
  }


  lastTs = nowTs;

  const int xoff = 5 ;
  if ( !g_workStateOn )
  {
    //OFF

    lcd.setCursor(xoff, 0);
    lcd.print("OFF      ");
  }
  else
  {
    lcd.setCursor(xoff, 0);
    // show ON or Stage time left
    if ( g_ReachTempStageTimeStampSec == 0)
    {
      //stage temp has not reached, just show ON
      lcd.print(" ON");
    } else
    {
      // stage timer has not reached yet
      // show timer counting
      unsigned long counting = millis() / 1000ul - g_ReachTempStageTimeStampSec;
      String countingStr = intToStringPadding(counting);
      lcd.print(countingStr);
    }

    const int xpwm = 8 ;
    lcd.setCursor(xpwm, 0);
    String bar;
    bar = getPwmBarFromPwmValue(g_heaterPwm );
    //test full bar   bar = getPwmBarFromPwmValue(255);
    lcd.print(bar);
  }
}


void turnHeaterOn()
{
  Serial.println("turnHeaterOn()");
  if ( g_heaterStartTimeStampSec == 0  )
  {
     // if it was OFF, now we are turning it ON
 
    g_heaterStartTimeStampSec = millis() / 1000ul;

    Serial.println("g_heaterStartTimeStampSec " + String(g_heaterStartTimeStampSec ));
  }
  g_workStateOn = true;


  if ( debug )
  {
    Serial.println("Heater is ON");
  }
}



void turnHeaterOff()
{
  disableHeater();
  g_heaterPwm = 0;
  g_heaterStartTimeStampSec = 0;
  g_workStateOn = false;
  g_heaterStartTimeStampSec = 0;
  g_ReachTempStageTimeStampSec = 0;
  g_activeProfile->changeActiveStage(0);
  if ( debug )
  {
    Serial.println("turnHeaterOff");
  }
}


void encoderInitForMainMenu( int value = 0)
{
  Serial.print("encoderInitForMainMenu");
  Serial.println(value);
  rotEncoder.setEncoderValue(  gui_options.getGuiIndex());
  rotEncoder.setEncoderStep(1);
  rotEncoder.setMinMaxValue( Gui_Options::Gui_Loc_Temp_Profile , Gui_Options::Gui_Loc_Max  );
  //rotEncoder.debug = false;
}

void encoderInitForTempChange( int value = 0)
{
  Serial.print("encoderInitForTempChange");
  Serial.println(value);
  rotEncoder.setEncoderValue( g_activeProfile->getCurrentStageTemp());
  rotEncoder.setEncoderStep(2);
  rotEncoder.setMinMaxValue( MIN_TARGET_TEMP  , MAX_TARGET_TEMP   );
  //rotEncoder.debug = false;
}

void encoderInitForTimeChange( int value = 0 )
{
  const int StageMaxTimeSec = 999;// max 10 min ?
  Serial.print("encoderInitForTimeChange" + String(g_activeProfile->getCurrentStageTimeSec()));
  Serial.println(value);
  rotEncoder.setEncoderValue( g_activeProfile->getCurrentStageTimeSec());
  rotEncoder.setEncoderStep(2);
  rotEncoder.setMinMaxValue( 0  , StageMaxTimeSec   );
  //rotEncoder.debug = false;

}
 
// for main loop.
void processHeaterTemp(int targettemp, int currentTemp)
{

  if (debug )
  {
    Serial.println("g_workStateOn:" + String(g_workStateOn) + ", targettemp: " + String(targettemp) + ", heater temp: " + String(currentTemp)   );
  }

  if ( g_workStateOn )
  {
    g_heaterPwm  = tempPid.step(targettemp, currentTemp );
    setHeaterPwm(g_heaterPwm  );

    if (  debug )
    {
      Serial.println("heater pwm: " + String(g_heaterPwm ) );
    }
  }
}

void checkSafetyTimer(unsigned long nowTsSec )
{

  if ( !g_workStateOn )
  {
    Serial.println("g_workStateOn is off");
    return;
  }


  //already started, check Max timeout
  if (  nowTsSec > ( g_heaterStartTimeStampSec + MaxPowerOnSec ))
  {
    if (debug)
    {
      Serial.println("nowTsSec:" + String(nowTsSec) + ",  g_heaterStartTimeStampSec:"
                     + String(g_heaterStartTimeStampSec) );
    }
    Serial.println("checkSafetyTimer turning off");

    turnHeaterOff();
    //showHeaterPwmBar();
    longBeep();
  }


}


void initTempProfile()
{

  g_smtprofile1.loadProfile();
  String s_name = g_smtprofile1.getCurrentStage()->getName() ;
  Serial.println("end getName");

  if ( s_name != "S1")
  {
    //Serial.println("g_smtprofile  _stageName" + g_smtprofile1.getCurrentStage()->getName());

    //eeprom is not inited.
    // this is for smt, 2 stages temp, ramp up and soak, spike, and cool
    g_smtprofile1.setTempStage(0, 24, 5); // 24C, soak for 5 seconds
    Serial.println("end setTempStage 0");

    g_smtprofile1.setTempStage(1, 26, 3); // 26C, spike for 3 seconds
    Serial.println("end setTempStage 1");

  }
  Serial.println("end g_smtprofile1");

  g_smtprofile2.loadProfile();
  s_name = g_smtprofile2.getCurrentStage()->getName();
  if ( s_name != "S1")
  {
    //eeprom is not inited.
    //Serial.println("g_smtprofile2 ->_stageName" + g_smtprofile2.getCurrentStage()->getName());

    // this is for one stage long time, for example desolder multiple pcb
    g_smtprofile2.setTempStage(0, 28, 20 );
    g_smtprofile2.setTempStage(1, 0, 0 );
  }
  g_activeProfile = &g_smtprofile1;

  Serial.println("end initTempProfile()");

}



void displayTempProfile()
{
  OneTempStage *stage = g_activeProfile->getCurrentStage();

  const int stageNameX = 1, stageNameY = 1;
  const int stage1tempx = 5, stage1timex = 11 , stagey = 1;

  lcd.setCursor(stageNameX, stageNameY);
  //Serial.println("stg name " +stage->getName());

  lcd.print(stage->getName());

  //Serial.println("temptext is " + String(temptext));

  lcd.setCursor(stage1tempx, stagey);
  String temptext = g_activeProfile->getStageTempPaddedText( );

  //Serial.println("temptext is " + String(temptext));

  lcd.print(temptext);

  String timesecText = g_activeProfile->getStageTimeSecPaddedText( );
  //Serial.println("timesecText is " + String(timesecText));
  lcd.setCursor(stage1timex, stagey);

  lcd.print(timesecText);

  //tempStageText = g_activeProfile->getTempStageText(1);



}


void displayTempProfileName()
{
  int p1x = 1, p1y = 0, p2x = 4;
  lcd.setCursor(p1x, p1y);
  //Serial.println("displayTempProfileName" + g_activeProfile->getName());

  lcd.print(g_activeProfile->getName());

}

void processEncoderButtonClick(int guiIndex)
{
  if ( guiIndex == Gui_Options::Gui_Loc_Temp_Profile )
  {
    // * pointing to profile , so clicking button means change another profile

    //Serial.println("Gui_Loc_Temp_Profile");

    // SmtTempProfile g_smtprofile1;
    //SmtTempProfile g_smtprofile2;
    if ( g_activeProfile == &g_smtprofile1 )
    {
      //Serial.println("==g_smtprofile1");

      g_activeProfile = &g_smtprofile2;
    } else
    {
      //Serial.println("else!");
      g_activeProfile = &g_smtprofile1;
    }
    displayTempProfileName();
    displayTempProfile();
    //g_targetTemp = g_activeProfile->getCurrentStageTemp();
    //g_stageTimeLengthSec = g_activeProfile->getCurrentStageTimeSec();

  } else  if ( guiIndex == Gui_Options::Gui_Loc_OnOff )
  {
    // * pointing to ON/OFF click button to switch on/off work mode
    g_workStateOn = !g_workStateOn;
    if( g_workStateOn )
    {
      turnHeaterOn();
    }

  } else if (guiIndex == Gui_Options::Gui_Loc_Stage)
  {
    g_activeProfile->changeActiveStage();

    displayTempProfile();
    //g_targetTemp = g_activeProfile->getCurrentStageTemp();
  }
  else if ( guiIndex == Gui_Options::Gui_Loc_Temp )
  {
    // * pointing to temp and click
    //Serial.println("guiIndex == Gui_Options::Gui_Loc_Temp, toggle!");
    gui_options.toggleIsChangeSetting();

    //* now pointing to temperature, click to enable changing temp.
    // click again to exit back to main menu

    if ( gui_options.getIsChangeSetting())
    {
      encoderInitForTempChange();
    }
    else
    {
      encoderInitForMainMenu();
      g_activeProfile->saveProfile();
    }

  } else if ( guiIndex == Gui_Options::Gui_Loc_TimeSec )
  {
    //* pointing to time, click to enable changing time sec.
    //Serial.println("guiIndex == Gui_Options::Gui_Loc_TimeSec, toggle!");
    gui_options.toggleIsChangeSetting();
    if ( gui_options.getIsChangeSetting())
    {
      encoderInitForTimeChange();
    }
    else
    {
      encoderInitForMainMenu();
      g_activeProfile->saveProfile();
    }

  }
  g_targetTemp = g_activeProfile->getCurrentStageTemp();
  g_stageTimeLengthSec = g_activeProfile->getCurrentStageTimeSec();

  gui_options.displayGuiIndicator();

}

void processEncoderRotation(int guiIndex)
{
  // button not clicked.
  // but rotary could have been rotated.
  // process encoder rotation input

  if ( (gui_options.getIsChangeSetting() ) && ( guiIndex == Gui_Options::Gui_Loc_Temp ))
  {
    // changing temperature
    int encoderValue = getEncoderReading();
    //display new temp and time?
    OneTempStage * currentStage = g_activeProfile->getCurrentStage();
    currentStage->setTemp( encoderValue ) ;

    displayTempProfile();
  } else if (  gui_options.getIsChangeSetting()  && (guiIndex == Gui_Options::Gui_Loc_TimeSec))
  {
    int encoderValue = getEncoderReading();
    //display new temp and time?
    OneTempStage * currentStage = g_activeProfile->getCurrentStage();
    currentStage->setTimeSec( encoderValue ) ;
    displayTempProfile();
  }

}

bool processEncoderRotationInput()
{

  // in the main menu, so should update main menu gui index
  // otherwise, don't update main menu gui index, because the reading could be temperature ie. 100C
  // do not update gui index to 100C
  gui_options.updateGuiPointer();


  int guiIndex = gui_options.getGuiIndex();
  //Serial.println("processEncoderRotationInput " + String(guiIndex) );

  if ( !rotEncoder.getButtonClicked( ) )
  {
    // not clicked
    processEncoderRotation(guiIndex);
    return false;

  } else
  {
    // button clicked
    // =========================================
    //Serial.println("processEncoderButtonClick " + String(guiIndex) );
    processEncoderButtonClick(guiIndex);
    gui_options.displayGuiIndicator();
    showHeaterWorkStatePwmBar();
    shortBeep();
    return true;
  }
}


void checkStageTimer(unsigned long nowSec)
{
  Serial.println( "checkStageTimer()");
  Serial.println("g_stageTimeLengthSec: " + String(g_stageTimeLengthSec) + ", g_ReachTempStageTimeStampSec: " + String(g_ReachTempStageTimeStampSec));
  Serial.println("now " + String(nowSec));

  // check stage time
  if ( nowSec > (g_stageTimeLengthSec + g_ReachTempStageTimeStampSec) )
  {
    // has reached stage time length.
    shortBeep();
    Serial.println( " < nowSec");

    bool hasNextStage = g_activeProfile->enterNextStage();
    if ( hasNextStage )
    {
      g_targetTemp = g_activeProfile->getCurrentStageTemp();
      g_stageTimeLengthSec = g_activeProfile->getCurrentStageTimeSec();
      // must reset the reach temp timestamp since it's a new stage 
      g_ReachTempStageTimeStampSec = 0;
      // must display the new stage,otherwise still the old stage.
    } else
    {
      // no next stage , turn off
      Serial.println("no next stage, all off");
      g_workStateOn = false;
      turnHeaterOff();
      longBeep();
    }
    displayTempProfile();

  }
  // end of check stage time

}

void setup() {

  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(BUZZER_PWM_PIN, OUTPUT);
  digitalWrite(BUZZER_PWM_PIN, LOW);

  pinMode(DETECT_20V_PIN, INPUT);

  pinMode(NTC_ADC_PIN, INPUT);
  pinMode(NTC_CHECK_ENABLE_PIN, OUTPUT);
  digitalWrite(NTC_CHECK_ENABLE_PIN, LOW);

  pinMode(HeaterPwmPin, OUTPUT);
  digitalWrite(HeaterPwmPin, LOW);

  pinMode(HeatingPlateThermocoupleAdcPin, INPUT);

  displayInit();
  showTitle();
  twoShortBeep();
  delay(1000);
  lcd.clear();
  encoderInitForMainMenu( );

  initTempProfile();
  turnHeaterOff();

  g_activeProfile = &g_smtprofile1;
  g_targetTemp = g_activeProfile->getCurrentStageTemp();
  g_stageTimeLengthSec = g_activeProfile->getCurrentStageTimeSec();


  displayTempProfileName();
  //Serial.print("displayTempProfile!");
  displayTempProfile();
  gui_options.displayGuiIndicator();
  Serial.println("end of setup");
}





void loop() {

  unsigned long startTimeStamp = 0;
  // store the last start time stamp

  startTimeStamp = millis();

  bool v20_OK = checkIs20VOK();

  bool buttonClicked = processEncoderRotationInput();

  if (!v20_OK)
  {
    //Serial.println("Warning!! 20V disconnected, MCU VREF is not 5V, thermocouple reading is OFF!!");
    //Serial.println("20V disconnected, disable heater.");
    turnHeaterOff();
  }
/*
  if ( g_workStateOn )
  {
    turnHeaterOn();
  }
*/
  showHeaterWorkStatePwmBar();
  //int guiIndex = gui_options.getGuiIndex();

  //displayTargetTemp(targetTemp);

  // must disable heater first to get thermo reading
  g_thermoTempC = getThermocoupleTemp();


  Serial.println("g_thermoTempC:" + String(g_thermoTempC) + ", g_targetTemp:" + String(g_targetTemp) +
                 ", g_ReachTempStageTimeStampSec:" + String(g_ReachTempStageTimeStampSec) );
  // check reaching stage temp time
  if (g_workStateOn && (g_thermoTempC >= g_targetTemp) && (g_ReachTempStageTimeStampSec == 0 ))
  {
    Serial.println("reaching target temp ");

    //now we are entering stage temp, save the timestamp now.
    g_ReachTempStageTimeStampSec = millis() / 1000ul;
    //g_targetTemp
    //g_stageTimerSec
    //g_thermoTempC
    //g_ReachTempStageTimeStampSec
  }


  processHeaterTemp(g_targetTemp , g_thermoTempC);

  showHeaterTemp(g_thermoTempC);


  static unsigned long prev_checkRoomTempTimeStamp = 0;
  unsigned long now = millis();

  if ( (( now  - prev_checkRoomTempTimeStamp ) / 1000)  > CheckRoomTempIntervalSec )
  {
    Serial.print("checking room temp...");
    g_room_temp = detectNTCTemp();
    Serial.println(String(g_room_temp) + "C");
    prev_checkRoomTempTimeStamp = now;
  }
  //Serial.println("Room temp:" + String(room_temp)+"C");

  if ( !g_workStateOn )
  {
    turnHeaterOff();

  } else
  {
    unsigned long nowsec = millis() / 1000ul;
    // it's ON
    checkSafetyTimer(nowsec);
    
    if( g_ReachTempStageTimeStampSec >0 )
    {  
      // dont do this when temp has not reached target temp.
      // check stage timer only after it reaches target temp. 
      checkStageTimer(nowsec);
    }

    unsigned long endTimeStamp = millis();
    unsigned long timeUsedMs = endTimeStamp - startTimeStamp ;
    const int ShouldDelayms = 1000 / FreqHz;
    long int delta = ShouldDelayms  - timeUsedMs;

    Serial.println("g_workStateOn " + String(g_workStateOn) + " thermo:" + String(g_thermoTempC) +
                   " heater pwm:" + String(g_heaterPwm ));


    if (delta > 0)
    {
      if (debug)
      {
        Serial.println("timeUsedMs: " + String(timeUsedMs) + " sleep for " + String(delta));
      }
      delay(delta);
    } else
    {
      if (debug)
      {
        Serial.println("timeUsedMs: " + String(timeUsedMs) + " " + String(delta) + " no sleep");
      }
    }

  }
}
