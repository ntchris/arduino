

const bool DEBUG = 0;
const String CMD_ON = "on";
const String CMD_OFF = "off";
const String CMD_POWER_CYCLE = "powercycle";
// power cycle, power off, delay below time in milli second, and power on again.
const int delayTimeMs = 5000;
const unsigned long SerialSpeed = 57600;

// to make it more stable and power cycle proof, use LOW to enable power, HI to disable power.
// so even when the arduino power cycle, USB power is kept enabled.

const int POWER_ENABLE_PIN = A3;

void USB_POWER_OFF()
{
  digitalWrite( POWER_ENABLE_PIN,  HIGH);
}

void USB_POWER_ON()
{
  digitalWrite( POWER_ENABLE_PIN,  LOW);
}

void setup() {
  // initialize serial:
  Serial.begin(SerialSpeed);
  // make the pins outputs:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite( LED_BUILTIN , HIGH);
  
  pinMode(POWER_ENABLE_PIN, OUTPUT);
  
  // default state, USB power is ON
  USB_POWER_ON();
  Serial.println("setup done");

}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    String command = Serial.readString();
    command.trim(); // important, must remove the ending LR/CR char.
    if ( command.length() <= 0 )
    {
      // only hitting enter, no other chars.
      continue;
    }

    if (DEBUG)
    {
      Serial.println("we have " +  command + " " + String( command.length()));
    }
    // look for the newline. That's the end of your sentence:

    command.toLowerCase();
    if (command == CMD_ON) {
      USB_POWER_ON();
      digitalWrite( LED_BUILTIN , HIGH);
      Serial.println("ON!!");

    } else if (command == CMD_OFF ) {
      USB_POWER_OFF();
      digitalWrite( LED_BUILTIN , LOW);
      Serial.println("OFF!!");

    } else if (command == CMD_POWER_CYCLE ) {
      Serial.println("Power Cycle, will do: power off, wait for 5 sec, power on");
      USB_POWER_OFF();
      digitalWrite( LED_BUILTIN , LOW);
      Serial.println("USB OFF and waiting...");

      delay(delayTimeMs);      
      
      USB_POWER_ON();
      digitalWrite( LED_BUILTIN , HIGH);
      Serial.println("USB ON");

    } else {
      Serial.println("Invalid command: " + command);
    }

  }
}
