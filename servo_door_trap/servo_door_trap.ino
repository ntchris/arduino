#include <Servo.h>
#include "LowPower.h"

const int ServoPin =  3;
const int AlarmPin = 9;
const int MotionDetectPin = A4;

const int servoMid = 90;
const int servoTrigger = 130;

const unsigned long CapturedAlarmTimerSec = 30 * 60 * 1000l;

Servo servo;

int angle = 0;   // servo position in degrees


void waitForSensorStable()
{
  //
  int i = 0;
  const int MAX = 6;
  int det;
  do {
    for (i = 0; i < MAX; i++)
    {
      shortBeep();
      delay(1500);
    }
    det = detectMotion();
  } while (det);
}

void setup()
{
  pinMode(MotionDetectPin, INPUT);
  pinMode(AlarmPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  longBeep();
  servoTest();

  //let motin sensor be stable
  waitForSensorStable();
  // meaning we are starting:
  longBeep();
  longBeep();
  longBeep();
}

void longBeep()
{
  digitalWrite(AlarmPin, HIGH);
  delay(600);
  digitalWrite(AlarmPin, LOW);
  delay(600);


}

void shortBeep()
{
  digitalWrite(AlarmPin, HIGH);
  delay(50);
  digitalWrite(AlarmPin, LOW);

}


void soundAlarm()
{

  shortBeep();
  delay(2000);
  shortBeep();


}


void servoTest()
{
  servo.attach(ServoPin);
  delay(200);
  servo.write(servoMid + 8);
  delay(1500);
  servo.write(servoMid);
  delay(2000);
  disableServo();
}


void disableServo()
{
  delay(1000);
  servo.detach();
  digitalWrite(ServoPin, LOW);
}

void shutDoorAction()
{

  servo.attach(ServoPin);
  delay(200);
  trigger();
  delay(1000);
  closeAndLock();
  delay(500);
  disableServo();


}

void trigger()
{
  servo.write(servoTrigger);
}

void closeAndLock()
{
  servo.write(servoMid);
}


bool detectMotion()
{
  //int detected = analogRead(MotionDetectPin);
  int detected = digitalRead(MotionDetectPin);
  return detected;
}


void deepSleep60sec()
{
  for (int i = 0; i < 7; i++)
  {
    LowPower.powerDown( SLEEP_8S, ADC_OFF, BOD_OFF);
  }

  LowPower.powerDown( SLEEP_4S, ADC_OFF, BOD_OFF);


}

void loop()
{
  static bool detected = false;
  static bool doorIsShut = false;
  if (!detected )
  {
    // consider pin change interrupt
    detected  = detectMotion();
  }

  if (detected && !doorIsShut)
  {
    // allow 300 to trigger ??
    
    shutDoorAction();
    doorIsShut = true;
    shortBeep();
    delay(1000);
    shortBeep();
    delay(1000);
    shortBeep();
    delay(1000);
    shortBeep();
    delay(1000);
    longBeep();
    delay(1000);
    shortBeep();
    delay(1000);
    shortBeep();
    
    //delay(5000);
    disableServo();
  }

  // once detected ,always detected
  if (doorIsShut)
  {
    // don't sound alarm too many times, only per 30min
    static int capturedNotification = 0;
    if ( capturedNotification > 30)
    {
      soundAlarm();
      capturedNotification = 0;

    }
    capturedNotification++;
    deepSleep60sec();
  }
  else
  {
     LowPower.powerDown( SLEEP_1S, ADC_OFF, BOD_OFF);
  }



}
