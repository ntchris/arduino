#include <LiquidCrystal.h>

//=====================================================
// LCD definition
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int contrastPin = 3;
// ====================================================

bool debug = 0;

const int IRINPUTPIN = A5;

const byte SampleSecond = 5;

// Maximum is one second has 65535 /2 trigger (rise and fall meaning one round),
// one second about 32000, 32000X60Sec = many many RPM (per minute), so it's good enough for motor measurment.
// if using 8bit 255, then it's 255/2*60sec= 7k rpm, so it's not enough
unsigned int g_sampleQueue[ SampleSecond ];
volatile int instantSpeed = 0;

// this maybe changed by timer interrupt. make it 8bit since we only keep a few seconds sample.
volatile byte g_index = 0;


void init_sampleQueue()
{
  for (byte i = 0; i < SampleSecond; i++)
  {
    g_sampleQueue[i] = 0;
  }
  g_index = 0;
}


void interruptHandlerPortD()
{
  processEncoderInterrupt(PIND);

}

void interruptHandlerPortC()
{
  // get All Pin status using PINC since we are using pin A5

  processEncoderInterrupt(PINC);

}



void interruptHandlerPortB()
{
  // handle pin change interrupt for D8 to D13 here, PINB (PortB)

  processEncoderInterrupt(PINB);

}

// handle pin change interrupt for D8 to D13 here, PINB (PortB)
ISR(PCINT0_vect)
{
  interruptHandlerPortB();
}


// handle pin change interrupt for A0 to A5 here, PINC (PortC)
ISR (PCINT1_vect)
{
  interruptHandlerPortC();
}  // end of PCINT1_vect


// handle pin change interrupt for D0 to D7 here, PIND (PortD)
ISR (PCINT2_vect)
{
  interruptHandlerPortD();
}  // end of PCINT2_vect



void  processEncoderInterrupt(int portValues)
{
  static bool toggle=false;
  if(toggle)
  {
    g_sampleQueue[g_index]++;
  }
  toggle=!toggle;

  if (debug) {
    Serial.print("Port ");
    Serial.println(portValues);
    // 24 0001 1000
    // 32 0010 0000

  }
}



void pciSetup(byte pin)
{
  // enable pin
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));

  // clear any outstanding interrupt
  PCIFR  |= bit (digitalPinToPCICRbit(pin));

  // enable interrupt for the group
  PCICR  |= bit (digitalPinToPCICRbit(pin));
}

void setInputAndPullupMode()
{
  for (int i = A0; i <= A5; i++)
    pinMode(i, INPUT_PULLUP);
}


// timer interrupt ============================================
void setTimeInterrupt()
{

  //set timer1 interrupt at 1Hz, cannot use 8bit timer.
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}


// Timer interrupt 1HZ / 1Sec
ISR(TIMER1_COMPA_vect)
{
  //record the instant speed first
  //to display as round per sec
  instantSpeed = g_sampleQueue[ g_index ];

  //timer1 interrupt 1Hz
  g_index ++;
  // Serial.print("g_index ");
  // Serial.println(g_index);
   
  g_index = g_index % SampleSecond;
  // must reset the counter
  g_sampleQueue[ g_index ] = 0;


  static bool toggle1 = false;
  if (toggle1) {
    digitalWrite(13, HIGH);
    toggle1 = 0;
  }
  else {
    digitalWrite(13, LOW);
    toggle1 = 1;
  }

}


void lcdSetup()
{
  lcd.begin(8, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  lcd.noCursor();
}

// ============================================================

void setup() {
  // pullup all A0 to A5, otherwise should use external pull down resistors for A0 to A5.
  // if not, would have random read value for the whole port, or even random interrupt by other pins.

  setInputAndPullupMode();
  // put your setup code here, to run once:
  pinMode(IRINPUTPIN, INPUT);

  pciSetup(IRINPUTPIN);

  setTimeInterrupt();

  pinMode(contrastPin , OUTPUT);
  analogWrite(contrastPin , 100);
  lcdSetup();
  if (debug)
  {
    Serial.begin(115200);
    Serial.print("Setup \n");
  }
}

//maybe need to measure something faster than 60K ??
unsigned long caculateRPM()
{
  unsigned long sum = 0;
  for (int i = 0; i < SampleSecond; i++)
  {
    // because one round has rise and fall two triggers.
    sum += g_sampleQueue[i];
  }

  // cal count in minute 60sec
  unsigned long rpm = sum * ( 60.0f / SampleSecond);
  return rpm;

}

void loop() 
{
  lcd.clear();
  // put your main code here, to run repeatedly:
  //static unsigned long timer = millis();
  lcd.setCursor(0, 0);
  lcd.print("S: ");
  lcd.print(instantSpeed);
  // must calculate RPM after queue is full, collected enough data.

  unsigned long rpm = caculateRPM();
  Serial.println(rpm);
  lcd.setCursor(0, 1);  
  lcd.print("M: "+String(rpm));

  delay(1000);

}
