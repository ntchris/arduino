#include "Arduino.h"
#include <EEPROM.h>   // eeprom save profile


const bool DebugProfile = false;
String intToStringPadding(int i);


class OneTempStage
{

  public:
    static const int NameSize = 3;
  private:
    char _stageName;
  public:

    int _temp = 0;
    int _timeSec = 0;
    // constructor
    void fixName();
    
    OneTempStage();
    
    void setName(char stgname);
    
    String getName();
    
    int getTemp();
    
    int getTimeSec();
    
    void setTemp(int temp );
    
    void setTimeSec(int sec);
    
    static const int _stage_size ;
};


class SmtTempProfile
{

    // only need 2 stages:  soap, spike, just give it 3
    static const int MaxStageCount = 2;
    int _index = 0;
    OneTempStage _stages[MaxStageCount];
    static int _eeprom_save_address;
    int _profileEepromSaveAddress = 0;
    String _name = "";
  public:
    SmtTempProfile(String profile_name);
    void changeActiveStage();
    void changeActiveStage(int index);
    //if can enter next stage, return true, otherwise false
    bool enterNextStage();
    void setTempStage(int index,   int temp, int timeSec );

    void clearAllStageSettings();


    int getCurrentStageTemp();
    int getCurrentStageTimeSec();

    //int getStageTemp(int index);
    //int getStageTimeSec(int index);
    OneTempStage *getCurrentStage();
    OneTempStage *getStage(int index);

    //String getTempStageText(int index);
    String getStageTempPaddedText(   );
    String  getStageTimeSecPaddedText(   );
    void saveProfile();
    void loadProfile();
    String getName();

};
