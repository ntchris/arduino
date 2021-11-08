
#include "TempProfile.hpp"


static int SmtTempProfile::_eeprom_save_address = 0;

static const int OneTempStage::_stage_size = sizeof(OneTempStage);

void OneTempStage::fixName()
{
  //char *p = _stageName.c_str();
  //p[NameSize - 1] = 0;
}

OneTempStage::OneTempStage()
{
  //memset(_stageName.c_str(), 0, NameSize);
  _stageName = 0;
}

void OneTempStage::setName(char stgname)
{
  _stageName = stgname;
  if (DebugProfile)
  {
    Serial.println("_stageName " + String(_stageName ));
  }

}

String OneTempStage::getName()
{
  String strname = "S ";
  strname[1] = _stageName;
  return strname;
}

int OneTempStage::getTemp()
{
  return _temp;
}
int OneTempStage::getTimeSec()
{
  return _timeSec;
}

void OneTempStage::setTemp(int temp )
{
  _temp = temp;
}

void OneTempStage::setTimeSec(int sec)
{
  _timeSec = sec;
}





SmtTempProfile::SmtTempProfile(String profile_name)
{
  _name = profile_name;
  _profileEepromSaveAddress = _eeprom_save_address;
  if (DebugProfile)
  {
    Serial.println("stage size is" + String(OneTempStage::_stage_size));
  }

  int profileSaveSize = MaxStageCount * OneTempStage::_stage_size;
  if (DebugProfile)
  {
    Serial.println("profile size is" + String(profileSaveSize));
  }
  _eeprom_save_address += profileSaveSize;

}


void SmtTempProfile::setTempStage(int index,  int temp, int timeSec )
{
  OneTempStage *stage = &_stages[index ];

  stage->setName('31' + index);
  stage->_temp = temp;
  stage->_timeSec = timeSec;

}

OneTempStage * SmtTempProfile::getCurrentStage()
{
  OneTempStage *stage = this->getStage(_index);
  return stage;
}

int SmtTempProfile::getCurrentStageTemp()
{
  OneTempStage *stage = this->getCurrentStage();
  return stage->getTemp();
}


int SmtTempProfile::getCurrentStageTimeSec()
{
  OneTempStage *stage = this->getCurrentStage();
  return stage->getTimeSec();
}


OneTempStage * SmtTempProfile::getStage(int index)
{

  if (DebugProfile)
  {
    Serial.println("getStageTemp() invalid index:" + String(index));
  }

  OneTempStage *stage = &_stages[index ];

  return stage  ;
}


String intToStringPadding(int i)
{
  String  text = String(i);

  if ( i < 10)
  {
    text =  "  " + String(i) ;

  } else if ( i < 100 )
  {
    text =  " " + String(i) ;

  } else if ( i >= 100 )
  {
    text =  String(i);
  }
  return text;

}
/*
  String SmtTempProfile::getTempStageText(int index)
  {
  OneTempStage *stage = this->getStage(index);
  int temp = stage->getTemp();
  int timesec = stage->getTimeSec();
  String temptext = intToString(temp);
  String sectext =  intToString(timesec);

  String text = temptext + "C " + sectext + "s";
  return text;
  }
*/

// return string but pad to has 3 chars
String SmtTempProfile::getStageTempPaddedText(  )
{
  int temp =  getCurrentStageTemp();
  String temptext = intToStringPadding(temp);
  return temptext + "C";

}

String SmtTempProfile::getStageTimeSecPaddedText(   )
{

  int timesec = getCurrentStageTimeSec();
  String sectext =  intToStringPadding(timesec);
  return sectext + "S";
}


void SmtTempProfile::changeActiveStage()
{
  if ( _index == 0)
  {
    _index = 1;
  } else
  {
    _index = 0;
  }
}

void SmtTempProfile::changeActiveStage(int index)
{
   if(index< MaxStageCount)
   {
    _index = index;
   }
  
}



bool SmtTempProfile::enterNextStage()
{
  if ( (_index + 1) < MaxStageCount)
  {
    OneTempStage *stages ;
    stages = &_stages[_index + 1];
    if ( stages->getTimeSec() > 0 && stages->getTemp() > 0 )
    {
      _index++;
      return true;
    }
  }
  return false;
}

void SmtTempProfile::saveProfile()
{

  int i;
  for (i = 0; i < MaxStageCount; i++)
  {

    int saveAddr = _profileEepromSaveAddress + i * OneTempStage::_stage_size;
    if (DebugProfile)
    {
      Serial.println("saveProfile addr " + String(saveAddr));
    }

    EEPROM.put( saveAddr,   _stages[i]);

  }
}

void SmtTempProfile::loadProfile()
{
  if (DebugProfile)
  {
    Serial.println("loadProfile()");
  }

  for (int i = 0; i < MaxStageCount; i++)
  {
    if (DebugProfile)
    {
      Serial.println("for()" + String(i));
    }

    OneTempStage *stage = &_stages[i];
    int loadAddr = _profileEepromSaveAddress + i * OneTempStage::_stage_size;
    if (DebugProfile)
    {
      Serial.println("addr " + String(loadAddr));
    }
    EEPROM.get( loadAddr,  _stages[i] );

    stage->fixName();
    if (DebugProfile)
    {
      Serial.println("loadProfile stage name " + String(stage->getName()));
      Serial.println("loadProfile stage temp " + String(stage->_temp));
      Serial.println("loadProfile stage timesec " + String(stage->_timeSec));
    }
  }

}

String SmtTempProfile::getName()
{
  return _name;
}
