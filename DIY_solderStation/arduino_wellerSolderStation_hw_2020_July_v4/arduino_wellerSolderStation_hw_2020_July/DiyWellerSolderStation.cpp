
#include "DiyWellerSolderStation.hpp"


//===============================================================================
// https://github.com/mike-matera/FastPID

FastPID tempPidRed(Kp, Ki, Kd, FreqHz, Bits, Sign);
FastPID tempPidBrown(Kp, Ki, Kd, FreqHz, Bits, Sign);
//===============================================================================


DiyWellerSolderStation::DiyWellerSolderStation(int redEnPin, int brownEnPin,
    int purpleOpAmOutPin, int blackOpAmOutPin,
    int magnetEn, int magnetDetect):
  heaterRedEnablePin(redEnPin), heaterBrownEnablePin(brownEnPin),
  purpleOpAmpReadPin(purpleOpAmOutPin), blackOpAmpReadPin(blackOpAmOutPin),
  magnetEnablePin(magnetEn), magnetDetectPin(magnetDetect)
{
  // constructor

  myWellerState.workStatus = WellerSolderControllerStatus::OFF;
  disableAllHeaters();
}

/*
  void DiyWellerSolderStation::initTcAdcIntLookupTable()
  {
   Serial.println("initialize thermal couple lookup table");
  // AdcInt List for TC temp:
  // opAmpOutput Voltage = tcMv * opAmpGain /1000
  // AdcInt = (opAmpOutput Voltage )/Vcc X 1023

  // mv table ThermalCoupleMvPer10C
  // TcTableSize
  int i = 0;
  for (i = 0; i < TcTableSize; i++)
  {
    double opAmpOutputVoltage = ThermalCoupleMvPer10C[i] * OpampGain / 1000.0;
    double adcD = opAmpOutputVoltage * ADC1024 / VCC5;
    int adcInt = round(adcD);
    TCAdcIntLookupTable[i] = adcInt ;
    //Serial.println("table " + String(adcInt));
  }
  }
*/


void DiyWellerSolderStation::disableAllHeaters()
{
  // Serial.println("Disable all heaters " );
  DiyWellerSolderStation::disableHeaterRed();
  DiyWellerSolderStation::disableHeaterBrown();
  

}


void DiyWellerSolderStation::enableHeater(int heaterEnablePin)
{
  digitalWrite(heaterEnablePin, HIGH);
}

void DiyWellerSolderStation::disableHeater(int heaterEnablePin)
{
  digitalWrite(heaterEnablePin, LOW);
}

void DiyWellerSolderStation::disableHeaterRed()
{
  if (debug)
  {
    Serial.println("Dis RED ht" );
  }
  disableHeater(heaterRedEnablePin);
  myWellerState.redPwm = 0;
  //myWellerState.isHeaterRedEnabled = false;

}


void DiyWellerSolderStation::disableHeaterBrown()
{
  if (debug)
  { Serial.println("Dis BR ht" );
  }
  disableHeater(heaterBrownEnablePin);
  myWellerState.brownPwm = 0;
  //myWellerState.isHeaterBrownEnabled = false;

}





bool DiyWellerSolderStation::checkIsHandleDisconnected()
{

  if ( myWellerState.isPurpleConnected
       &&  myWellerState.isBlackConnected)
  {
    if ( DiyWellerSolderStation::debug )
    {
      Serial.println("is connected");
    }
    return true;
  }
  else
  {
    if ( DiyWellerSolderStation:: debug )
    {
      Serial.println("not connected");
    }
    return false;
  }
}



int DiyWellerSolderStation::getAnalogAvgReadingInt(int analogReadPin, int avgCount = 5)
{

  int i, opampValInt = 0;
  for (i = 0; i < avgCount; i++)
  {
    int val;
    delayMicroseconds(200);

    val = analogRead(analogReadPin);
    //Serial.println( val);
    opampValInt += val;
  }
  opampValInt = round(opampValInt / (avgCount*1.0));
  return opampValInt;
}


int DiyWellerSolderStation::getPurpleOpAmpOutputInt()
{

  int purpleInt =  getAnalogAvgReadingInt(purpleOpAmpReadPin);

  if ( DiyWellerSolderStation::debug )
  {
    Serial.println( "Red getPurpleOpAmpOutputInt(adjusted): " + String(purpleInt));
  }
  return purpleInt;
}


int DiyWellerSolderStation::getBlackOpAmpOutputInt()
{
  int blackInt = getAnalogAvgReadingInt(blackOpAmpReadPin );

  if ( DiyWellerSolderStation:: debug )
  {
    Serial.println( "Brown getBlackOpAmpOutputInt: " + String(blackInt));
  }
  return blackInt;
}


// only for reading int value from purple and black pin's value
bool DiyWellerSolderStation::opampReadIntToIsConnected(int opampValInt)
{

  // Opamp reading if > this, meaning handle is disconnected.
  // ADC, if 5.0v then it's 1023
  const int NotConnectedVoltageInt = 850;
  //if not connected, the opamp output is high 5.0V
  if (opampValInt >= NotConnectedVoltageInt)
  {
    return false;;
  }
  else
  {
    return true;
  }
}

int DiyWellerSolderStation::getHeaterRedTemp()
{
  int readInt = getPurpleOpAmpOutputInt();
  if (DiyWellerSolderStation::debug)
  { Serial.println("readInt: " + String(readInt));
  }
  
  myWellerState.isPurpleConnected = opampReadIntToIsConnected(readInt);
  int redtemp = 0;
  if (myWellerState.isPurpleConnected)
  {
    myWellerState.purpleInt = readInt;

    redtemp = getOpampOutputIntToTemperature(
                              myWellerState.purpleInt) ;
  }
  return redtemp;
}


int DiyWellerSolderStation::getHeaterBrownTemp()
{
  myWellerState.blackInt = getBlackOpAmpOutputInt();
  // Serial.println("update checking blackInt " + String(myWellerSolderController.blackInt  ));

  myWellerState.isBlackConnected = opampReadIntToIsConnected(
                                     myWellerState.blackInt);
  int brownTemp = 0;
  if (myWellerState.isBlackConnected)
  {
    brownTemp = getOpampOutputIntToTemperature(myWellerState.blackInt);
    if ( DiyWellerSolderStation::debug )
    {
      Serial.println("brownInt: " + String(myWellerState.blackInt) + " brownTemp(compened):" + String(myWellerState.brownTemp  ));
    }

  } 
  return brownTemp;
}



float DiyWellerSolderStation::adcIntToVoltage(int adcInt)
{
  float adcIntF = adcInt*1.0;
  float vol = VCC5 * adcIntF / ADC1023;
  // Serial.println("my cal " + String(vol));
  /*
    const int  ADCMAPMAX1023 = 1023;

    seems always the same result.
    const int MakeFloatInt = 10000;
    // must make VCC5 larger because it's 4.94 so it will become 4 as input
    float vol2 = map(adcInt, 0, ADCMAPMAX1023, 0, VCC5*MakeFloatInt );
    Serial.print("map " );
    Serial.println(vol2/10000);
  */
  return vol;
}

// must disable heater first and delay!!
int DiyWellerSolderStation::getOpampOutputIntToTemperature(int analogReadingInt)
{
  int measured_temp = 0;
  //double adcVoltage = adcIntToVoltage(analogReadingInt);//VCC5 * analogReadingInt * 1.0 / ADC1024;
  //Serial.println("opAmp votage: " + String(adcVoltage));
  measured_temp = convertAdcIntToTemperature(analogReadingInt);
  if ( DiyWellerSolderStation::debug )
  {
    Serial.println("temp " + String(measured_temp ));
  }
  int adjustedTemp = 0;
  adjustedTemp = measured_temp + getEnvTemperature();
  if ( DiyWellerSolderStation::debug )
  {
    Serial.println("tc + env temp " + String( adjustedTemp ));
  }
  return adjustedTemp;
}
int DiyWellerSolderStation::getEnvTemperature()
{
  // maybe in feature, use KTY reading.
  //Serial.println("getEnvTemperature temp " + String(myWellerState.evnTemp ));
  return myWellerState.evnTemp;
}

bool DiyWellerSolderStation::checkIsFatalError()
{
  return myWellerState.fatalError;
}

bool DiyWellerSolderStation::isPenPutOnRest()
{
  const int MagnetDetectValue = 100;
  //  when on rest, magnet connect to ground, reading should be low , very close to 0.
  //  when on use, magnet connect to pull up 10K and with 2K then to ground, reading should be about
  //  5V(4.95) / (10K+2K) * 2K= 0.8V , adc int = 170, we use 165.

  disableAllHeaters();
  delay(1);
  pinMode(magnetEnablePin, OUTPUT); // when output conflict with opamp pin7 output

  digitalWrite(magnetEnablePin, HIGH);
  delayMicroseconds(1000);
  int magnetReadInt = analogRead(magnetDetectPin);
  myWellerState.isOnRest = (magnetReadInt < MagnetDetectValue);

  if ( DiyWellerSolderStation:: debug )
  {
    float vol = adcIntToVoltage(magnetReadInt);
    Serial.println("magnet pin int: " + String(magnetReadInt) + " " + vol + " onRest:" + String(myWellerState.isOnRest));
  }
  //when magnet switch is close, magnet detect pin connects to gnd, so it's low


  // done, cancel the magnet enable pin pullup
  digitalWrite(magnetEnablePin, LOW);

  pinMode(magnetEnablePin, INPUT);

  return myWellerState.isOnRest;
}

// using TCAdcIntLookupTable, recursive binary search
// return the index that table[index] < value < table[index+1]
// table values must be in small to large order
// if value is too large, return last index.
// if value is too small, return first index.
int valueToLookupTableIndex(int value, int startindex, int endindex)
{
  int index;
  if ( ( endindex - startindex ) <= 1)
  {
    //end of search
    return startindex;
  }

  if (value <= TCAdcIntLookupTable[startindex])
  {
    return startindex;
  } else if ( value >= TCAdcIntLookupTable[endindex] )
  {
    return endindex;
  }

  int midindex = (startindex + endindex) / 2;
  int midValue = TCAdcIntLookupTable[midindex];
  if ( value <= midValue )
  {
    index = valueToLookupTableIndex(value, startindex, midindex);
  } else // value > midValue
  {
    index = valueToLookupTableIndex(value,  midindex, endindex);
  }

  return index;
}

bool DiyWellerSolderStation::selfTestConvertAdcIntToTemperature()
{
  const int adcInt[] =           {0,  72, 110, 150,  195, 235, 270, 370, 414,   488, 550, 581, 600, 650,  690, 728, 764, 800, 867, 900};
  const int correctResultTempC[] = {0,  50, 74, 99,  126, 150, 170, 225, 250,   290, 323, 340, 350, 377,  398, 420, 440, 460, 498, 500};
  int MAX = sizeof(adcInt) / sizeof(adcInt[0]);
  for (int i = 0; i < MAX; i++)
  {
    int adc = adcInt[i];
    int temp = convertAdcIntToTemperature(adc);
    if ( DiyWellerSolderStation::debug)
    {
      Serial.println(String(adc) + " : " + String(temp));

    }

    if (temp != correctResultTempC[i])
    {
      Serial.println("wrong temperature conversion: " + String(adc) + " : actual: " + String(temp) + " vs exp " + String(correctResultTempC[i]));
      myWellerState.fatalError  = true;
    }
  }
  if ( DiyWellerSolderStation::debug)
  { if ( !myWellerState.fatalError )
    {
      Serial.println("adc to temperature lookup and search algorithm self test passed");
    }
  }
  return myWellerState.fatalError;
}

// using lookup table TCAdcIntLookupTable
int DiyWellerSolderStation::convertAdcIntToTemperature(int adcInt)
{
  int temp = 0;

  // first index = 0,  last index = tableSize -1
  int resultIndex = valueToLookupTableIndex(adcInt, 0, TcTableSize - 1);

  // must check for last index, or may have array index out of range error
  if (resultIndex  == (TcTableSize - 1))
  {
    return resultIndex  * TempCPerIndex;
  } else
  {
    // for map
    int mapInt0 = TCAdcIntLookupTable[resultIndex ];
    int mapInt1 = TCAdcIntLookupTable[resultIndex  + 1];
    int maptemp0 = resultIndex  * TempCPerIndex, maptemp1 = (resultIndex  + 1) * TempCPerIndex;
    temp = map(adcInt, mapInt0, mapInt1, maptemp0, maptemp1 );
  }
  return temp;
}


void DiyWellerSolderStation::setHeaterRedPmw(int pwm)
{
  analogWrite(heaterRedEnablePin, pwm);
}

void DiyWellerSolderStation::setHeaterBrownPmw(int pwm)
{

  analogWrite( heaterBrownEnablePin, pwm);

}
 

 


void DiyWellerSolderStation::checkHandleTemp()
{
  disableAllHeaters();
  delay(ReadTcDelayMilliSec);
  int redTemp = getHeaterRedTemp();
  int brownTemp = getHeaterBrownTemp();
  myWellerState.isConnected = checkIsHandleDisconnected();
  if(myWellerState.isConnected )
  {
    //
    myWellerState.redTemp = redTemp;
    myWellerState.brownTemp = brownTemp;
  }

}

// for main loop.
// do not process red and brown each turn, do it every other time.
void DiyWellerSolderStation::processWellerHandleTemp(int targettemp)
{ //static bool checkRedTemp = true;
  // have to disable all heaters (both) to get a stable read value

  if ( DiyWellerSolderStation::debug )
  {
    Serial.println("targettemp: " + String(targettemp) +
                   ", redTemp: " + String(myWellerState.redTemp) + ", brown: " + String(myWellerState.brownTemp ) );
  }

  if ( myWellerState.workStatus == WellerSolderControllerStatus::ON)
  {
    myWellerState.redPwm = tempPidRed.step(targettemp, myWellerState.redTemp);
    setHeaterRedPmw(myWellerState.redPwm);

    myWellerState.brownPwm = tempPidBrown.step(targettemp, myWellerState.brownTemp);
    setHeaterBrownPmw(myWellerState.brownPwm);
    //enableHeaterBrownPmw(1);
    if ( DiyWellerSolderStation:: debug )
    {
      Serial.println("red pwm: " + String(myWellerState.redPwm ) + " brown pwm: "+ String(myWellerState.brownPwm));
    }
  }
}
