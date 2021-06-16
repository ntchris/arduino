#include <Servo.h> 
 
int servoPin = 10;
 
Servo servo;  
 
int servoAngleMid = 90;   // servo position in degrees 
 
void setup() 
{ 
    pinMode(servoPin, OUTPUT);
    trigger();
    //servo.attach(servoPin); 
    //servo.write(servoAngleMid);
} 
 

void trigger()
{   
    servo.attach(servoPin); 
    servo.write(servoAngleMid);
    delay(2000);
    servo.write(servoAngleMid - 45);
    delay(2000);
    servo.detach();
    digitalWrite(servoPin, LOW);

}


void loop() 
{ 
   
   
   delay(3000);
    
    
       
} 
