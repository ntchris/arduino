// for NTC Thermistor
#include "SuperNtcThermistor.h"


#include "TempProfile.hpp"


// for store setting in eeprom


#include <LiquidCrystal_I2C.h>

// for rotary encoder
#include "superRotaryEncoder.hpp"

// for PID
#include <PID_v1.h>

#include <Adafruit_NeoPixel.h>





const bool debug = true;
const bool DebugTimer = false;
const bool DebugPwm  = true;
const bool DebugSleep = false;


// if use PTC resistor plate/module, must reduce power when in low temp and provide full power when it's high temp
#define USE_PTC 1


// ==========  MCU Pin def  ==============

// for NTC
#define NTC_CHECK_ENABLE_PIN A2
#define NTC_ADC_PIN A3


#define BUZZER_ENABLE_PIN 3

#define HeaterRelayPin 2

//#define HeatingPlateThermocoupleAdcPin A7

#define I2C_SDA A4;  // A4, for LCD display SDA
#define I2C_SCL A5;  // A5, for LCD display SCL

#define RGB_DATA_PIN  13

// ==========  End of MCU Pin def  ==============



const float BALANCE_RESISTOR = 100.0 * 1000.0  ;
const float NTC_VALUE_Room_Temp_C = 18;// the room temp when measure the NTC resistance
const float RoomtempNtcResist  = 133.0 * 1000.0  ; //the NTC resist at above temp;
const float NTC_BETA = 3950;
SuperNtcThermistor superNTC(NTC_ADC_PIN, BALANCE_RESISTOR, NTC_VALUE_Room_Temp_C, RoomtempNtcResist, NTC_BETA, 1023 );

//#define LCDADDRESS 0x27 // 2004: 0x3F
//static LiquidCrystal_I2C lcd(LCDADDRESS, 16, 2);
#define LCDADDRESS 0x3F
static LiquidCrystal_I2C lcd(LCDADDRESS, 20, 4);

// beep buzzer
const int cH = 523;
const int aH = 880;



#define CheckNTCTempIntervalMs 200



#define RotaryPinA 8
#define RotaryPinB 9
#define RotaryButtonPin 7
SuperRotaryEncoder rotEncoder(RotaryPinA, RotaryPinB, RotaryButtonPin );

SmtTempProfile g_smtprofile1("Pf1");
SmtTempProfile g_smtprofile2("Pf2");
SmtTempProfile *g_activeProfile;


// pwm value from 0 to 255
#define MaxBarTypeCount 8
static const String BarString[MaxBarTypeCount] = {"       >", "      >>", "     >>>", "    >>>>", "   >>>>>", "  >>>>>>", " >>>>>>>", ">>>>>>>>"};


int g_workStateOn = false;

unsigned long g_heaterStartTimeStampSec = 0;

unsigned long g_ReachTempStageTimeStampSec = 0;

const int MIN_TARGET_TEMP = 15;

const int MAX_TARGET_TEMP = 350;




// safety feature
const unsigned int MaxPowerOnSec = 30 * 60;


#define RGB_LED_COUNT 8
Adafruit_NeoPixel pixels(RGB_LED_COUNT, RGB_DATA_PIN, NEO_GRB + NEO_KHZ800);



/*

  //const float Kp = 7.58, Ki = 0.050, Kd = 0.86, FreqHz = 20;  // for weller tweezers  , a bit weak.
  //const float Kp = 3, Ki = 2, Kd = 2, FreqHz = 20;
  // const float Kp = 0.2, Ki = 1, Kd = 2, FreqHz = 20;  set 160 rush to 180
  //const float Kp = 0.6, Ki = 0.6, Kd = 2, FreqHz = 20;
  //const float Kp = 0.2, Ki = 0.2, Kd = 0.6, FreqHz = 20;    // set 100, jump to 160
  //const float Kp =0.2, Ki = 0.2, Kd =3, FreqHz = 20;   184-->215

  // const float Kp =0.8, Ki = 0.08, Kd =0.8, FreqHz = 20;   // if Ki = 0.06, temp up is too slow, 25W, pwm=40

  //const float Kp = 1.05, Ki = 0.07, Kd = 0.8, FreqHz = 20; // if Ki = 0.06, temp up is too slow, 25W, pwm=40

  //const float Kp = 1.05, Ki = 0.1, Kd = 0.2, FreqHz = 20; // bascially OK , very slow
  //const float Kp = 1.05, Ki = 0.1, Kd = 1, FreqHz = 20; // bascially OK, a little slow
  //const float Kp = 1.05, Ki = 0.1, Kd = 5, FreqHz = 20; //  faster response
  //const float Kp = 1.00, Ki = 0.1, Kd = 5, FreqHz = 20; //
  //const float Kp = 1.00, Ki = 0.1, Kd = 10, FreqHz = 20; // fast

  const float Kp = 0.8, Ki = 0.1, Kd = 12.70; // fast  // Kd must < 255/Hz = 12.75

  const bool Sign = false;
  const int Bits = 8;
  FastPID g_tempPid(Kp, Ki, Kd, FreqHz, Bits, Sign);
*/
const float FreqHz = 20;
// double aggKp=4, aggKi=0.2, aggKd=1;
double Kp = 0.6, Ki = 0.05, Kd = 2;

const int MAXPWM = 255;

double g_hotplateTempC = 0;
int g_targetTemp = 0;
double g_pid_targetTemp = 0;

//byte g_heaterPwm = 0;
double g_heaterPwm = 0;

PID g_tempPid(&g_hotplateTempC, &g_heaterPwm, &g_pid_targetTemp, Kp, Ki, Kd, DIRECT);

byte pid_compute(  )
{

  //   g_tempPid.SetOutputLimits(0, MAXPWM );
  g_tempPid.Compute();
  //Serial.println(String(g_thermoTempC) + " " + String(g_heaterPwm) + " "+ String(g_pid_targetTemp));



  uint8_t ptc_pwm = 0;
  if ( USE_PTC )
  {
    ptc_pwm = g_heaterPwm;
  }
  g_heaterPwm = ptc_pwm;
  return g_heaterPwm;
}

// end of PID
//=============================================================================================


int g_stageTimeLengthSec = 0;


int getEncoderReading()
{
  int rotaryValue = rotEncoder.getValue();

  return rotaryValue;
}



const uint8_t lcd_chardata_bedTemp[8] = {
  B00000,
  B11111,
  B10101,
  B10001,
  B10101,
  B11111,
  B00000,
  B00000
};


const uint8_t lcd_chardata_degree[8] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};

const uint8_t lcd_chardata_temp[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110
};


void init_lcd_special_chars()
{
  lcd.createChar(0, lcd_chardata_temp);
}


class Gui_Options
{

    const int X_Profile = 0, Y_Profile = 0  ;
    const int X_OnOff = 7, Y_OnOff = 0 , X_Reached_Timer = 15;
    const int X_Stage_Name = 2, X_Stage_Temp = 9, X_Stage_Time = 15 ;
    const int Y_Stage1 = 1, Y_Stage2 = 2;
    const int X_Plate_Temp = 11, Y_Plate_Temp = 3;
    const int X_MESSAGE = 0, Y_MESSAGE = 3 ;
    const int X_PWM = 0, Y_PWM = 3 ;


    int _gui_option_index = Gui_Loc_Temp_Profile ;
    bool _isChangeSetting = false;

  public:
    static const int Gui_Loc_Temp_Profile = 0;
    static const int Gui_Loc_OnOff = 1;

    static const int Gui_Loc_Stage1_Temp = 2;
    static const int Gui_Loc_Stage1_Time = 3;
    static const int Gui_Loc_Stage2_Temp = 4;
    static const int Gui_Loc_Stage2_Time = 5;

    static const int Gui_Loc_Max = 5;

    int getGuiIndex()
    {
      return _gui_option_index;
    }

    bool getIsChangeSetting()
    {
      return _isChangeSetting;
    }
    void toggleIsChangeSetting()
    {
      // Serial.println("was:_isChangeSetting " + String(_isChangeSetting));
      _isChangeSetting = !_isChangeSetting;
    }


    void displayGuiIndicator(  )
    {

      lcd.setCursor(X_Profile, Y_Profile);
      lcd.print(" ");
      lcd.setCursor(X_OnOff - 1, Y_OnOff);
      lcd.print(" ");
      //lcd.setCursor(X_Stage1, Y_Stage1);
      //lcd.print(" ");
      //lcd.setCursor(X_Stage2, Y_Stage2);
      //lcd.print(" ");

      lcd.setCursor(X_Stage_Temp - 1, Y_Stage1);
      lcd.print(" ");
      lcd.setCursor(X_Stage_Temp - 1, Y_Stage2);
      lcd.print(" ");
      lcd.setCursor(X_Stage_Time - 1, Y_Stage1);
      lcd.print(" ");
      lcd.setCursor(X_Stage_Time - 1, Y_Stage2);
      lcd.print(" ");

      lcd.setCursor(X_Reached_Timer, Y_OnOff);
      lcd.print("    ");

      if ( _gui_option_index == Gui_Loc_Temp_Profile)
      {
        lcd.setCursor(X_Profile, Y_Profile);
        lcd.print("*");
      } else if ( _gui_option_index  == Gui_Loc_OnOff )
      {
        lcd.setCursor(X_OnOff - 1, Y_OnOff);
        lcd.print("*");
      } else if ( _gui_option_index == Gui_Loc_Stage1_Temp)
      {
        lcd.setCursor(X_Stage_Temp - 1, Y_Stage1);
        if ( getIsChangeSetting())
        {
          lcd.print(">");


        } else
        {
          lcd.print("*");

        }

      }
      else if ( _gui_option_index == Gui_Loc_Stage1_Time)
      {
        lcd.setCursor(X_Stage_Time - 1, Y_Stage1);
        if ( getIsChangeSetting())
        {
          lcd.print(">");

        } else
        {
          lcd.print("*");

        }


      }
      else if (  _gui_option_index  == Gui_Loc_Stage2_Temp )
      {

        lcd.setCursor(X_Stage_Temp - 1, Y_Stage2);
        if ( getIsChangeSetting())
        {
          lcd.print(">");
        } else
        {
          lcd.print("*");
        }


      } else if ( _gui_option_index  == Gui_Loc_Stage2_Time )
      {

        lcd.setCursor(X_Stage_Time - 1, Y_Stage2);
        if ( getIsChangeSetting())
        {
          lcd.print(">");
        } else
        {
          lcd.print("*");
        }
      }

    }

    void updateGuiPointer()
    {
      // Serial.println("updateGuiPointer " + String(_gui_option_index));

      static int prev_index = -1;
      if ( !getIsChangeSetting() )
      {
        _gui_option_index = getEncoderReading();
      }


      if ( _gui_option_index != prev_index )
      {
        //  Serial.println("updateGuiPointer " + String(_gui_option_index));

        prev_index = _gui_option_index;
        //when encode is rotated, not clicked.
        displayGuiIndicator();
      }

    }



    void displayTempStage(OneTempStage *stage, int y)
    {
      //Serial.println(String("displayTempStage ") + stage->getName() + " " + String(stage->getTemp()) + " " + String(stage->getTimeSec()));

      lcd.setCursor(X_Stage_Name - 1, y);

      if (g_workStateOn && stage == g_activeProfile->getCurrentStage() )
      {
        lcd.print("#");
        // show active stage sign when it's ON
      } else
      {
        lcd.print(" ");
      }

      //lcd.setCursor(X_Stage_Name, y);

      lcd.print(stage->getName());


      lcd.setCursor(X_Stage_Temp, y);
      String temptext = stage->getStageTempPaddedText( );
      lcd.print(temptext);

      String timesecText = stage->getStageTimeSecPaddedText( );
      lcd.setCursor(X_Stage_Time, y);
      //Serial.println("timesecText " + timesecText);

      lcd.print(timesecText);

    }

    void displayTempProfileStages(SmtTempProfile *profile)
    {
      OneTempStage *stage1 = profile->getStage(0);
      OneTempStage *stage2 = profile->getStage(1);
      displayTempStage(stage1, Y_Stage1);
      displayTempStage(stage2, Y_Stage2);
    }


    void show_message(String message)
    {
      static int old_message_len = 0;

      lcd.setCursor(X_MESSAGE, Y_MESSAGE);
      lcd.print("          ");
      lcd.setCursor(X_MESSAGE, Y_MESSAGE);
      lcd.print(message);
      Serial.println(message);
      /*
            if (old_message_len>message.length())
            {
              int delta = old_message_len>message.length();
              String empty="";
              int i = 0;
              for (i=0;i<delta;i++)
              {
                empty=empty+" ";
              }
              lcd.print(empty);
            }
      */
      old_message_len = message.length() ;
    }

    void showHeaterWorkStatePwmBar( bool force = false)
    {

      static unsigned long lastTs = 0;
      unsigned long nowTs = millis();
      //only refresh every 500ms, don't refresh too fast, no use.
      const unsigned long Delta = 300;
      if (force)
      { Serial.println("force update pwm bar");
      }
      if ( force && (( nowTs - lastTs ) < Delta ))
      {

        return;
      }


      lastTs = nowTs;


      if ( !g_workStateOn )
      {
        //OFF
        lcd.setCursor(X_OnOff, Y_OnOff);
        lcd.print("OFF");

      }
      else
      {
        lcd.setCursor(X_OnOff , Y_OnOff);
        lcd.print(" ON");


        // show ON or Stage time left
        if ( g_ReachTempStageTimeStampSec > 0)
        {
          lcd.setCursor(X_Reached_Timer, Y_OnOff);

          // stage timer has not reached yet
          // show timer counting
          unsigned long counting = millis() / 1000ul - g_ReachTempStageTimeStampSec;
          String countingStr = intToStringPadding(counting);
          lcd.print(countingStr);
        }


      }
      lcd.setCursor(X_PWM, Y_PWM);


      String bar = getPwmBarFromPwmValue(g_heaterPwm );
      //Serial.println("new bar " + bar);

      lcd.print(bar);


    }

    void show_heat_plate_temp(int temp, int target_temp = 0)
    {
      lcd.setCursor(X_Plate_Temp, Y_Plate_Temp);
      // temp icon
      lcd.write(byte(0));
      String temp_str(temp);
      if (temp_str.length() == 1)
      {
        temp_str = "  " + temp_str;
      } else if (temp_str.length() == 2)
      {
        temp_str = " " + temp_str;
      }

      lcd.print(temp_str);
      if (target_temp)
      {
        lcd.print(String("/") + target_temp);
      }
      lcd.print('C');
    }




};

Gui_Options gui_options;

void buzzerOn( int pwm)
{
  digitalWrite( BUZZER_ENABLE_PIN, HIGH);

}

void buzzerOff( )
{
  digitalWrite( BUZZER_ENABLE_PIN, LOW);
}



float read_ntc_temp()
{
  digitalWrite(NTC_CHECK_ENABLE_PIN, HIGH);
  delayMicroseconds(200);
  float ntctemp = superNTC.getNtcTemp();

  // ntc is mounted on PCB, so temp is a bit higher than room temp.
  // adjust it a little bit

  //digitalWrite(NTC_CHECK_ENABLE_PIN, LOW);
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


void setHeaterPwm(byte pwm)
{
  //analogWrite(HeaterRelayPin, pwm);
  apply_pwm_to_relay(pwm);

}

void turn_relay_on()
{
  digitalWrite(HeaterRelayPin, LOW);

}

void turn_relay_off()
{
  digitalWrite(HeaterRelayPin, HIGH);
}

void apply_pwm_to_relay(byte pwm)
{
  static unsigned long last_apply = 0;
  unsigned long now = millis();
  const unsigned long MIN_APPLY_TIME_MS = 800;
  if ((now - last_apply) < MIN_APPLY_TIME_MS )
  {
    // do not apply relay too often
    return;
  }
  last_apply = now;

  const unsigned long WindowSizeMs = 10000; // 5sec
  static unsigned long windowStartTime = 0;

  unsigned long timer_counter = now - windowStartTime;
  if ( timer_counter > WindowSizeMs)
  { //time to shift the Relay Window
    windowStartTime = now;
    timer_counter = 0;
    Serial.println(String("new window start time now ") + String(now)   );
  }


  int on_period_ms = map(pwm, 0, MAXPWM, 0, WindowSizeMs );


  Serial.println(String("pwm ") + String(pwm) + String(" on ms ") + String(on_period_ms) + String(", timer_counter ") + String(timer_counter)  );


  if ( timer_counter < on_period_ms )
  {
    Serial.println("relay on!");
    turn_relay_on();
  }
  else {
    Serial.println("relay off!");
    turn_relay_off();
  }

}


void disableHeater()
{
  digitalWrite(HeaterRelayPin, HIGH);
}


// must be short tone
// because we are using delay to make it simple.
void shortBeep(int delayMs = 50, int pwm = aH)
{
  digitalWrite(BUZZER_ENABLE_PIN, HIGH);
  delay(delayMs);
  digitalWrite(BUZZER_ENABLE_PIN, 0);
}

void twoShortBeep()
{
  shortBeep(20);
  buzzerOff( );
  delay(200);
  shortBeep(20);
  buzzerOff( );
}

void longBeep( )
{
  const int LongBeepDelayMs = 800;
  analogWrite(BUZZER_ENABLE_PIN, HIGH);
  delay(LongBeepDelayMs);
  analogWrite(BUZZER_ENABLE_PIN, 0);
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


int getHotPlateTemp()
{
  return read_ntc_temp();

}



void displayTextOnLcd(String line1, String line2 = "", String line3 = "", String line4 = "")
{

  lcd.setCursor(0, 0);
  lcd.print(line1);
  if (line2.length() > 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(line2);
  }
  lcd.setCursor(0, 2);
  lcd.print(line3);
  lcd.setCursor(0, 3);
  lcd.print(line4);

}

void showTitle()
{
  lcd.clear();
  displayTextOnLcd("   Super SMT ", " Heating Plate ", "   LCD2004", "   I2C");
}

void displayInit()
{
  //const int DefaultBrightness = 3;

  lcd.init();
  lcd.backlight();
  lcd.clear();

}



void showHeaterTemp(int temp, int target_temp)
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


  gui_options.show_heat_plate_temp(temp, target_temp);


}



String getPwmBarFromPwmValue(byte pwm)
{
  // limit by the lcd screen space

  const String emptyBar = "         ";
  //const String emptyBar = "empty";
  //const int NoShow=5;

  //Serial.println("pwm is " + String(pwm));

  if (pwm < 1)
  {
    return emptyBar;
  }

  //int index = round((1.0*pwm)/MaxPWM * (MaxBarTypeCount-1));
  int index = map(pwm, 0, MAXPWM, 0, MaxBarTypeCount - 1);
  // should not use empty bar as first item, because no matter how small the pwm is, it need to be shown

  return BarString[index];

}


void turnHeaterOn()
{


  if ( g_heaterStartTimeStampSec == 0  )
  {
    // if it was OFF, now we are turning it ON

    g_heaterStartTimeStampSec = millis() / 1000ul;

    //Serial.println("g_heaterStartTimeStampSec " + String(g_heaterStartTimeStampSec ));
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
  // g_activeProfile->changeActiveStage(0);
  // cannot do this in here,
  if ( debug )
  {
    // Serial.println("turnHeaterOff");
  }


}


void encoderInitForMainMenu( int value = 0)
{

  rotEncoder.setEncoderValue(  gui_options.getGuiIndex());
  rotEncoder.setEncoderStep(1);
  rotEncoder.setMinMaxValue( Gui_Options::Gui_Loc_Temp_Profile , Gui_Options::Gui_Loc_Max  );
  //rotEncoder.debug = false;
}


void encoderInitForTempChange(OneTempStage *stage, int value = 0)
{


  rotEncoder.setEncoderValue( stage->getTemp());
  rotEncoder.setEncoderStep(2);
  rotEncoder.setMinMaxValue( MIN_TARGET_TEMP  , MAX_TARGET_TEMP   );

}


void encoderInitForTimeChange(OneTempStage *stage  )
{
  const int StageMaxTimeSec = 999;// max 10 min ?


  rotEncoder.setEncoderValue( stage->getTimeSec());
  rotEncoder.setEncoderStep(2);
  rotEncoder.setMinMaxValue( 0  , StageMaxTimeSec   );
  //rotEncoder.debug = false;

}

// for main loop.
void processHeaterTemp(int targettemp, int currentTemp)
{
  bool printPWM = false;
  if (DebugPwm )
  { static unsigned long last_printms = 0;
    unsigned long now = millis();
    if ((now - last_printms) > 1000)
    {
      printPWM = true;
      //Serial.println("g_workStateOn:" + String(g_workStateOn) + ", targettemp: " + String(targettemp) + ", heater temp: " + String(currentTemp));
      last_printms = now;
    }
  }

  if ( g_workStateOn )
  {
    //g_heaterPwm  = g_tempPid.step(targettemp, currentTemp );
    byte heaterPwm = pid_compute();
    //Serial.println("after compute, pwm is now " + String(heaterPwm));

    // sometimes even when currentTemp > target, but the output pwm is stil large like 255...filter it
    const byte FilterMinPwm = 20;
    const int TooHigh = 2;
    if ( currentTemp >= (targettemp + TooHigh ))
    {
      if (heaterPwm )
      {
        Serial.println("Tooo high, set pwm 0, was " + String(heaterPwm));
        heaterPwm = 0;
        g_heaterPwm = 0;
      }
      // tooo tooo high


    }/* else if ( currentTemp > targettemp )
    {
      // just a little bit too high
      if ( heaterPwm > FilterMinPwm )
      {

        Serial.print("filtered wrong pwm " + String(g_heaterPwm) );
        g_heaterPwm = FilterMinPwm;
        Serial.println(", now is " + String(g_heaterPwm));
      }

    }
*/
    //g_heaterPwm  = (targettemp, currentTemp );

    setHeaterPwm(heaterPwm);

    if ( printPWM)
    {
      Serial.println("heater pwm: " + String(heaterPwm ) );
    }
  } else
  {
    turn_relay_off();
  }
}

void checkSafetyTimer(unsigned long nowTsSec )
{

  if ( !g_workStateOn )
  {
    //Serial.println("g_workStateOn is off");
    return;
  }


  //already started, check Max timeout
  if (  nowTsSec > ( g_heaterStartTimeStampSec + MaxPowerOnSec ))
  {

    if (DebugTimer)
    {
      Serial.println("nowTsSec:" + String(nowTsSec) + ",  g_heaterStartTimeStampSec:"
                     + String(g_heaterStartTimeStampSec) );
    }
    //Serial.println("checkSafetyTimer turning off");

    turnHeaterOff();
    gui_options.showHeaterWorkStatePwmBar(true);
    longBeep();
  }


}


void initTempProfile()
{

  g_smtprofile1.loadProfile();
  String s_name = g_smtprofile1.getStage(0)->getName() ;
  //Serial.println("end getName");

  if ( !s_name.startsWith("Stg"))
  {
    //Serial.println("g_smtprofile  _stageName" + g_smtprofile1.getCurrentStage()->getName());

    //eeprom is not inited.
    // this is for smt, 2 stages temp, ramp up and soak, spike, and cool
    g_smtprofile1.setTempStage(0, 25, 21); // 24C, soak for 20 seconds
    //Serial.println("end setTempStage 0");

    g_smtprofile1.setTempStage(1, 30, 12); // 28C, spike for 10 seconds
    //Serial.println("end setTempStage 1");

  }
  //Serial.println("end g_smtprofile1");

  g_smtprofile2.loadProfile();
  s_name = g_smtprofile2.getStage(0)->getName();
  if (! s_name.startsWith("Stg"))
  {
    //eeprom is not inited.
    //Serial.println("g_smtprofile2 ->_stageName" + g_smtprofile2.getCurrentStage()->getName());

    // this is for one stage long time, for example desolder multiple pcb
    g_smtprofile2.setTempStage(0, 28, 20 );
    g_smtprofile2.setTempStage(1, 0, 0 );
  }
  g_activeProfile = &g_smtprofile1;

  //Serial.println("end initTempProfile()");

}




void displayTempProfileStages()
{

  gui_options.displayTempProfileStages(g_activeProfile);



}


void displayTempProfileName()
{
  int p1x = 1, p1y = 0, p2x = 4;
  lcd.setCursor(p1x, p1y);
  //Serial.println("displayTempProfileName" + g_activeProfile->getName());

  lcd.print(g_activeProfile->getName());

}


void update_stage_setting()
{
  g_targetTemp = g_activeProfile->getCurrentStageTemp();
  g_stageTimeLengthSec = g_activeProfile->getCurrentStageTimeSec();
  if ( g_targetTemp > g_hotplateTempC)
  {
    g_ReachTempStageTimeStampSec = 0;
  }
  g_pid_targetTemp = g_targetTemp;

  Serial.println("update_stage_setting g_pid_targetTemp: " + String(g_pid_targetTemp));
  gui_options.show_heat_plate_temp( g_hotplateTempC, g_targetTemp);



}

void processEncoderButtonClick(int guiIndex)
{
  OneTempStage *changing_stage = NULL;
  if ( guiIndex == Gui_Options::Gui_Loc_Temp_Profile )
  {
    // * pointing to profile , so clicking button means change another profile

    Serial.println("Gui_Loc_Temp_Profile");

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
    displayTempProfileStages();
    update_stage_setting();

  } else if ( guiIndex == Gui_Options::Gui_Loc_OnOff )
  {
    // * pointing to ON/OFF click button to switch on/off work mode
    g_workStateOn = !g_workStateOn;
    if ( g_workStateOn )
    {
      turnHeaterOn();
    } else
    {
      turnHeaterOff();
      Serial.println("button turn off");
      gui_options.showHeaterWorkStatePwmBar(true);

    }
    // to show active stage
    update_stage_setting();

    displayTempProfileStages();

  }
  else if ( guiIndex == Gui_Options::Gui_Loc_Stage1_Temp || guiIndex == Gui_Options::Gui_Loc_Stage1_Time ||
            guiIndex == Gui_Options::Gui_Loc_Stage2_Temp || guiIndex == Gui_Options::Gui_Loc_Stage2_Time )
  {
    Serial.println("here guiIndex = " + String(guiIndex));

    gui_options.toggleIsChangeSetting();

    //* now pointing to temperature, click to enable changing temp.
    // click again to exit back to main menu


    if ( gui_options.getIsChangeSetting() )
    {
      if (guiIndex == Gui_Options::Gui_Loc_Stage1_Temp || guiIndex == Gui_Options::Gui_Loc_Stage1_Time )
      {
        changing_stage = g_activeProfile->getStage(0);
      }
      else if (guiIndex == Gui_Options::Gui_Loc_Stage2_Temp || guiIndex == Gui_Options::Gui_Loc_Stage2_Time )
      {
        changing_stage = g_activeProfile->getStage(1);
        //Serial.println("btn changing stage " + changing_stage->getName()  );

      }
      if (!changing_stage)
      {
        //Serial.println("btn changing stage is NULL");
        return;
      }

      if (    guiIndex == Gui_Options::Gui_Loc_Stage1_Temp || guiIndex == Gui_Options::Gui_Loc_Stage2_Temp)
      {
        //Serial.println("btn changing stage encoderInitForTempChange");
        encoderInitForTempChange( changing_stage );

      } else if (  guiIndex == Gui_Options::Gui_Loc_Stage1_Time || guiIndex == Gui_Options::Gui_Loc_Stage2_Time)
      {
        encoderInitForTimeChange(changing_stage);
      }

    } else
    {
      // exiting temp/time setting
      encoderInitForMainMenu();
      g_activeProfile->saveProfile();
      update_stage_setting();
    }
  }
  else
  {

    /* encoderInitForMainMenu();

      // user just changed target temp, reset the target temp timer if new target temp is higher than current temp
      g_targetTemp = changing_stage->getTemp();
      if ( g_targetTemp > g_thermoTempC)
      {
       g_ReachTempStageTimeStampSec = 0;
      }*/
  }



  gui_options.displayGuiIndicator();

}

void processEncoderRotation(int guiIndex)
{
  // button not clicked.
  // but rotary could have been rotated.
  // process encoder rotation input

  if ( !gui_options.getIsChangeSetting() )
  {

    return;
  }

  int encoderValue = getEncoderReading();
  OneTempStage *changing_stage = NULL;
  if ( guiIndex == Gui_Options::Gui_Loc_Stage1_Temp || guiIndex == Gui_Options::Gui_Loc_Stage1_Time )
  {
    //Serial.println("processEncoderRotation guiIndex "+ String(guiIndex));
    changing_stage = g_activeProfile->getStage(0);
    //Serial.println("here changing_stage = g_activeProfile->getStage(0) ");
    //Serial.println( changing_stage->getName());


  } else if ( guiIndex == Gui_Options::Gui_Loc_Stage2_Temp || guiIndex == Gui_Options::Gui_Loc_Stage2_Time  )
  {
    changing_stage = g_activeProfile->getStage(1);
    //Serial.println("here changing_stage = g_activeProfile->getStage(1) ");
    //Serial.println( changing_stage->getName());
  }
  //Serial.println( "here111 " + changing_stage->getName());

  if ( guiIndex == Gui_Options::Gui_Loc_Stage1_Temp || guiIndex == Gui_Options::Gui_Loc_Stage2_Temp)
  {
    //Serial.println("here222 changing stage temp "+changing_stage->getName());
    changing_stage->setTemp( encoderValue ) ;

  } else if (guiIndex == Gui_Options::Gui_Loc_Stage1_Time || guiIndex == Gui_Options::Gui_Loc_Stage2_Time)
  {
    //Serial.println("here333 changing stage time "+changing_stage->getName());
    changing_stage->setTimeSec( encoderValue ) ;
  }
  displayTempProfileStages();




}

bool processEncoderInput()
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
    // rotation to change setting only, not including main menu selection
    processEncoderRotation(guiIndex);
    return false;

  } else
  {
    // button clicked
    // =========================================
    processEncoderButtonClick(guiIndex);
    gui_options.displayGuiIndicator();
    gui_options.showHeaterWorkStatePwmBar(true);
    shortBeep();
    return true;
  }
}


void checkStageTimer(unsigned long nowSec)
{
  //Serial.println( "checkStageTimer()");
  //Serial.println("g_stageTimeLengthSec: " + String(g_stageTimeLengthSec) + ", g_ReachTempStageTimeStampSec: " + String(g_ReachTempStageTimeStampSec));
  //Serial.println("now " + String(nowSec));

  // check stage time
  if ( nowSec > (g_stageTimeLengthSec + g_ReachTempStageTimeStampSec) )
  {
    // has reached stage time length.
    shortBeep();
    Serial.println( " < nowSec");

    bool hasNextStage = g_activeProfile->enterNextStage();
    if ( hasNextStage )
    {
      update_stage_setting();
      // must reset the reach temp timestamp since it's a new stage
      g_ReachTempStageTimeStampSec = 0;
      // must display the new stage,otherwise still the old stage.
    } else
    {
      // no next stage , turn off
      Serial.println("no next stage, all off");
      g_workStateOn = false;
      turnHeaterOff();
      g_activeProfile->changeActiveStage(0);
      longBeep();
    }
    displayTempProfileStages();

  }
  // end of check stage time

}

void set_rgb_color(byte r, byte g, byte b)
{
  int i = 0;
  for ( i = 0; i < RGB_LED_COUNT; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }

}


// all RGB brightest (all 255), 8 RGB LEDs, 340ma )
// added relay 407A
void set_rgb_color_by_temp_simple(int temp)
{

  byte r = 0, g = 0, b = 0;
  const int HIGH_TEMP = 160, MID_HIGH = 100 , MID_LOW = 50;
  if ( temp > HIGH_TEMP )
  {
    r = 128, g = 0, b = 0;
  } else if ( temp > MID_HIGH)
  {
    r = 100, g = 64, b = 0;
  } else if ( temp > MID_LOW)
  {
    r = 64, g = 64, b = 0;
  } else
  {
    r = 0, g = 0, b = 80;
  }

  Serial.println(String("temp is ") + String(temp) + " , " + r + " " + g + " " + b);
  set_rgb_color(r, g, b);
  pixels.show();

}



// all RGB brightest (all 255), 8 RGB LEDs, 340ma )
// added relay 407A
void set_rgb_color_by_temp(int temp)
{
  const int MAX = 148;
  const int B_MAX = 45;

  byte r = 0, g = 0, b = 0;
  const int LOW_TEMP = 35, HIGH_TEMP = 170;
  if ( temp < LOW_TEMP)
  {
    b = B_MAX;
    r = 0;
    g = 0;
  } else if ( temp > HIGH_TEMP )
  {
    r = MAX;
    g = 0;
    b = 0;
  } else
  {

    r = map(temp,  LOW_TEMP, HIGH_TEMP, 0, MAX);
    if ( MAX > (r + 1) )
    {
      g = MAX - r - 1;
    }
    if ( g > r )
    {
      g = r;
    }
    if (B_MAX > r )
    {
      b = B_MAX - r;
    }
    
    
  }
  set_rgb_color(r, g, b);
  Serial.println(String("temp is ") + String(temp) + " , " + r + " " + g + " " + b);

  pixels.show();

}


void test_rgb_color()
{
  int i = 0;
  for (i = 0; i < 300; i += 5)
  {
    set_rgb_color_by_temp(i);
    delay(500);
  }

}



void setup() {

  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(BUZZER_ENABLE_PIN, OUTPUT);
  digitalWrite(BUZZER_ENABLE_PIN, LOW);


  pinMode(NTC_ADC_PIN, INPUT);
  pinMode(NTC_CHECK_ENABLE_PIN, OUTPUT);
  digitalWrite(NTC_CHECK_ENABLE_PIN, LOW);

  pinMode(HeaterRelayPin, OUTPUT);
  turn_relay_off();
  pinMode(RGB_DATA_PIN, OUTPUT);
  digitalWrite(RGB_DATA_PIN, LOW);


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

  // g_tempPid.setOutputRange(0, MAXPWM );
  g_tempPid.SetOutputLimits(0, MAXPWM );
  g_tempPid.SetSampleTime(1000.0 / FreqHz);
  //turn the PID on
  g_tempPid.SetMode(AUTOMATIC);


  displayTempProfileName();
  //Serial.print("displayTempProfile!");
  displayTempProfileStages();
  gui_options.displayGuiIndicator();
  //Serial.println("end of setup");


  init_lcd_special_chars();
  // test_special_char();

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  test_rgb_color();

}





void loop() {


  unsigned long startTimeStamp = 0;
  // store the last start time stamp

  startTimeStamp = millis();


  bool buttonClicked = processEncoderInput();



  gui_options.showHeaterWorkStatePwmBar();

  //int guiIndex = gui_options.getGuiIndex();

  //displayTargetTemp(targetTemp);

  // must disable heater first to get thermo reading
  g_hotplateTempC = getHotPlateTemp();



  // check reaching stage temp time
  if (g_workStateOn && (g_hotplateTempC >= g_targetTemp) && (g_ReachTempStageTimeStampSec == 0 ))
  {
    //Serial.println("reaching target temp ");

    //now we are entering stage temp, save the timestamp now.
    g_ReachTempStageTimeStampSec = millis() / 1000ul;
    //g_targetTemp
    //g_stageTimerSec
    //g_thermoTempC
    //g_ReachTempStageTimeStampSec
  }


  processHeaterTemp(g_targetTemp , g_hotplateTempC);
  showHeaterTemp(g_hotplateTempC, g_targetTemp );
  set_rgb_color_by_temp(g_hotplateTempC);

  static unsigned long prev_checkRoomTempTimeStamp = 0;
  unsigned long now = millis();

  if ( !g_workStateOn )
  {

    turnHeaterOff();

  } else
  {
    // it's ON
    unsigned long nowsec = millis() / 1000ul;
    // it's ON
    checkSafetyTimer(nowsec);

    if ( g_ReachTempStageTimeStampSec > 0 )
    {
      // dont do this when temp has not reached target temp.
      // check stage timer only after it reaches target temp.
      checkStageTimer(nowsec);
    }

  } // end of ON

  // checking for sleep for how long ?
  unsigned long endTimeStamp = millis();
  unsigned long timeUsedMs = endTimeStamp - startTimeStamp ;
  const unsigned long ShouldDelayms = round(1000.0 / FreqHz);


  //Serial.println("g_workStateOn " + String(g_workStateOn) + " thermo:" + String(g_thermoTempC) +
  //               " heater pwm:" + String(g_heaterPwm ));


  if (ShouldDelayms > timeUsedMs)
  {
    unsigned long deltaMs = ShouldDelayms  - timeUsedMs;

    if (DebugSleep)
    {
      Serial.println("timeUsedMs: " + String(timeUsedMs) + " sleep for " + String(deltaMs));
    }
    delay(deltaMs);
  } else
  {
    if (DebugSleep)
    {
      //Serial.println("timeUsedMs: " + String(timeUsedMs) + " " + String(delta) + " no sleep");
    }
  }


}
