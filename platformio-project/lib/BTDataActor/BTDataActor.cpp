#include <BTDataActor.h>
#include <Arduino.h>

///
/// Instanciation of BTDataActor 
BTDataActor::BTDataActor()
{

}

///
/// Initializes the Component and its DataStorage
void BTDataActor::init()
{
        _serialBT.begin("ESP32"); //Name des ESP32;;
}

///
/// Initializes the component
void BTDataActor::reInit()
{
    ;
}

/// 
/// Updates BME Data into DataStorage
void BTDataActor::action(IDataStorage *dataStorage)
{
  std::map<std::string, double> data = dataStorage->getMapCopy();
  std::map<std::string, double>::iterator it;

  for (it = data.begin(); it != data.end(); it++)
  {
      _serialBT.printf("%s-%f\n",it->first.c_str(),it->second);
  }
}

///
/// fake at the moment, set up sleep mode
void  BTDataActor::sleep() {
    ; // Maybe Default Output if sleeping?
}
