

const int LedPin = 13;

int outputPin = 5;

// pin 5 6 can use for analog pwm output, 980hz
// 127 output 2.4v ,  255 output 4.8v

void setup() {
    // put your setup code here, to run once:


    pinMode(LedPin, OUTPUT);

    int i;
    for (i = 1; i < 20; i++)
    {
        outputPin = i;
        pinMode(outputPin, OUTPUT);

        analogWrite(outputPin, 127);
    }



}

void loop() {
    // put your main code here, to run repeatedly:



}
