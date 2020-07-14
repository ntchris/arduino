#include <Servo.h>

const int ServoPin =  3; 
const int AlarmPin = 9;
const int MotionDetectPin = A4;

const int servoMid = 90;
const int servoTrigger = 130;


const int CapturedAlarmTimerSec = 120000;

Servo servo;

int angle = 0;   // servo position in degrees


void waitForSensorStable()
{
  //
  int i=0;
  const int MAX=6;
  int det;
  do{       
  for(i=0;i<MAX;i++)
  {
     shortBeep();
     delay(1500);
  }
  det =detectMotion();
  }while(det);
}

void setup()
{
  servo.attach(ServoPin);
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
  delay(800);
  digitalWrite(AlarmPin, LOW);
  delay(800);


}

void shortBeep()
{
  digitalWrite(AlarmPin, HIGH);
  delay(100);
  digitalWrite(AlarmPin, LOW);
  delay(500);
}


void soundAlarm()
{
  
  shortBeep();
  shortBeep();

}


void servoTest()
{
  servo.write(servoMid+8);
  delay(1000);
  servo.write(servoMid);
}

void shutDoorAction()
{
  trigger();
  delay(1200);
  closeAndLock();

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

void loop()
{
  static bool detected = false;
  static bool doorIsShut = false;
  if(!detected )
  {
    detected  = detectMotion();
  }
  
  if (detected && !doorIsShut)
  {
     // allow 300 to trigger ??
    delay(500);
    shutDoorAction();
    doorIsShut = true;
    shortBeep();
    shortBeep();
    shortBeep();
    shortBeep();
    shortBeep();
    delay(5000);
  }
   
   // once detected ,always detected
  if(detected)
  {
    //digitalWrite(LED_BUILTIN, HIGH);
    
    //digitalWrite(LED_BUILTIN, LOW);
    soundAlarm();
    delay(CapturedAlarmTimerSec);
  }
  
  delay(200);



}
