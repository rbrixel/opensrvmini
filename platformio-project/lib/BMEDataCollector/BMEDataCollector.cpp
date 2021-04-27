#include <BMEDataCollector.h>
#include <Arduino.h>
#define CHANNELEXTTEMP ".TEMP"
#define CHANNELEXTPRESSURE ".PRES"

///
/// Constructing with a channelName
BMEDataCollector::BMEDataCollector(std::string channelName)
{
    _channelName = channelName;
}

///
/// Initializes the Component and its DataStorage
void BMEDataCollector::init(IDataStorage *storage)
{
    _dataStorage = storage;
}

///
/// Initializes the component
void BMEDataCollector::reInit()
{
    ;
}

/// 
/// Updates BME Data into DataStorage
void BMEDataCollector::updateData()
{
    readBMEData();
    _dataStorage->addData(_channelName + CHANNELEXTTEMP , _temp);
    _dataStorage->addData(_channelName + CHANNELEXTPRESSURE , _pressure);
}

///
/// Returns this channelsName
std::string BMEDataCollector::getName(){
    return _channelName;
}

///
/// fake at the moment, Reads BME Data
void  BMEDataCollector::readBMEData() {
    
    _temp =  std::rand()/((RAND_MAX + 1u)/35);;
    _pressure =  std::rand()/((RAND_MAX + 1u)/35);;
}

///
/// fake at the moment, set up sleep mode
void  BMEDataCollector::sleep() {
    ;
}
