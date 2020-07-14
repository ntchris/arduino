#include <Servo.h> 
 
int servoPin = 11;
 
Servo servo;  
 
int angle = 0;   // servo position in degrees 
 
void setup() 
{ 
  servo.attach(servoPin); 
} 
 

void trigger()
{
    servo.write(90);
    delay(1800);
    servo.write(0);
}


void loop() 
{ 


    
    
       
} 
