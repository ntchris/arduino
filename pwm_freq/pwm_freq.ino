// Prescaled, 8-bit Fast PWM mode.
// #define SPINDLE_TCCRB_INIT_MASK   (1<<CS20)               // Disable prescaler -> 62.5kHz
// #define SPINDLE_TCCRB_INIT_MASK   (1<<CS21)               // 1/8 prescaler -> 7.8kHz (Used in v0.9)
// #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
// #define SPINDLE_TCCRB_INIT_MASK   ((1<<CS21) | (1<<CS20)) // 1/32 prescaler -> 1.96kHz
#define SPINDLE_TCCRB_INIT_MASK      (1<<CS22)               // 1/64 prescaler -> 0.98kHz (J-tech laser)
#define SPINDLE_TCCRB_INIT_MASK_488HZ  ((1<<CS22)|(1<<CS20))  // 1/128 prescaler -> 488kHz
#define SPINDLE_TCCRB_INIT_MASK_244HZ  ((1<<CS22)|(1<<CS21))  // 1/256 prescaler -> 244kHz
#define SPINDLE_TCCRB_INIT_MASK_60HZ  ((1<<CS22)|(1<<CS20)|(1<<CS21))  // 1/1024    prescaler -> 60HZ
// https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
#define SPINDLE_TCCRA_INIT_MASK   ( (1 << COM2A1) |  (1<<WGM20) | (1<<WGM21))  // Configures fast PWM mode.


#define SPINDLE_TCCRA_REGISTER = SPINDLE_TCCRA_INIT_MASK; // Configure PWM output compare timer

#define SPINDLE_TCCRA_REGISTER    TCCR2A
#define SPINDLE_TCCRB_REGISTER    TCCR2B
#define SPINDLE_OCR_REGISTER      OCR2A
#define SPINDLE_COMB_BIT          COM2A1
void pinInit()
{
  SPINDLE_TCCRA_REGISTER = SPINDLE_TCCRA_INIT_MASK; // Configure PWM output compare timer
  SPINDLE_TCCRB_REGISTER = SPINDLE_TCCRB_INIT_MASK_60HZ;
  //SPINDLE_TCCRB_REGISTER = (TCCR2B & 0b11111000 )|SPINDLE_TCCRB_INIT_MASK_jjalling; // this is 60hz


}

// For Spindle controled by ESC, brushed or brushless motor/esc.
#define SpindleUsingESC true
 // make it float so not lose precision
#define ESC_SPINDLE_PWM_ONTIMEUS_MIN_US 1500.0
// make it float so not lose precision
#define ESC_SPINDLE_PWM_ONTIMEUS_MAX_US 2050.0
// 1500/16667*255=23
#define ESC_PWM_CYCLE_TIME_US (1000.0/60 * 1000)

#define ESC_SPINDLE_PWM_MIN ( ESC_SPINDLE_PWM_ONTIMEUS_MIN_US * 255.0 / ESC_PWM_CYCLE_TIME_US ) 
// 31
#define ESC_SPINDLE_PWM_MAX ( ESC_SPINDLE_PWM_ONTIMEUS_MAX_US * 255.0 / ESC_PWM_CYCLE_TIME_US )



// min pwm on time :1000us = 1 ms   max pwm on time: 2000us = 2 ms
uint8_t convertNormalPwmToEscPwm(uint8_t pwm)
{
  
    // make it float so later we won't cause int calculation problem.
    float MAX_PWM = 255.0f;

    const unsigned int OneSec1000Ms = 1000; //1sec =1000ms
    const unsigned int Freq = 60;
    float powerPercent = (pwm * 1.0f / MAX_PWM);    
    Serial.println("inputPwmPercent " + String(powerPercent));
    // must use US not to loss data
    int onTimeUs = round((ESC_SPINDLE_PWM_ONTIMEUS_MIN_US + ((ESC_SPINDLE_PWM_ONTIMEUS_MAX_US - ESC_SPINDLE_PWM_ONTIMEUS_MIN_US) * powerPercent)) );
    Serial.println("PWM: " + String(pwm) + " to: " + String(onTimeUs) + " On time US");
    const unsigned int PwmCycleTimeUs = round( OneSec1000Ms*1000.0 / Freq ); //one pwm cycle for 60hz is 1sec/60hz =16.6ms=16666us 
    Serial.println("PwmCycleTimeUs " + String(PwmCycleTimeUs)); 

    int escPWM = round((onTimeUs * 1.0f) / PwmCycleTimeUs * MAX_PWM);
    return escPWM;
}


void armEsc(void)
{
   //analogWrite(11, 50); // using min pwm signal to arm the esc
   //delay(1000);

   analogWrite(11, ESC_SPINDLE_PWM_MIN ); // using min pwm signal to arm the esc
   //delay_ms(200);
   // must wait for this time to make esc connected /armed.
   delay(1200);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting");
  pinInit();
  int escPwm;
  //escPwm = convertNormalPwmToEscPwm(255);
  //escPwm = convertNormalPwmToEscPwm(128);
  //escPwm = convertNormalPwmToEscPwm(0);
  Serial.println("ESC_PWM_CYCLE_TIME_US "+ String(ESC_PWM_CYCLE_TIME_US));
  Serial.println("ESC_SPINDLE_PWM_MIN " + String(ESC_SPINDLE_PWM_MIN));
  Serial.println("ESC_SPINDLE_PWM_MAX " + String(ESC_SPINDLE_PWM_MAX));
  armEsc();
  //analogWrite(11, 25 );
  delay(100);
  //analogWrite(11, maxpwm );

  delay(2000);

  //analogWrite(11, 25 );
  
  //analogWrite(11, ESC_SPINDLE_PWM_MIN );
  
  //int escPwm;
  escPwm = convertNormalPwmToEscPwm(0);
  Serial.println("setting " + String(escPwm));

  analogWrite(11, escPwm  );


//  escPwm = convertNormalPwmToEscPwm(50);
//  Serial.println("setting " + String(escPwm));
//  analogWrite(11, escPwm  );
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static int pwm = 90;
  //int escPwm = convertNormalPwmToEscPwm(pwm);
  //analogWrite(11, escPwm  );
  //pwm -=2;
  analogWrite(11, ESC_SPINDLE_PWM_MIN );
  delay(1000);
  
}
