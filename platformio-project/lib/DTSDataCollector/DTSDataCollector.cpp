#include <DTSDataCollector.h>
#include <Arduino.h>

#define CHANNELEXTTEMP ".TEMP"
#define CHANNELEXTPRESSURE ".PRES"

///
/// Constructing with a channelName
DTSDataCollector::DTSDataCollector(std::string channelName)
{
    _channelName = channelName;
    _ds18sensors = new DallasTemperature(_oneWire);
}

///
/// Constructing with a channelName
DTSDataCollector::DTSDataCollector(std::string channelName,uint8_t oneWirePin )
{
    _channelName = channelName;
    _oneWirePin = oneWirePin;
    _oneWire = new OneWire(_oneWirePin);
    _ds18sensors = new DallasTemperature(_oneWire);
}

///
/// Initializes the Component and its DataStorage
void DTSDataCollector::init(IDataStorage *storage)
{
    _dataStorage = storage;
}

///
/// Initializes the component
void DTSDataCollector::reInit()
{
    ;
}

/// 
/// Updates Dallas Temperature Data into DataStorage
void DTSDataCollector::updateData()
{
    if (_ds18sensors->getDeviceCount()==0){
        _dataStorage->addData(_channelName + CHANNELEXTTEMP , 0.0f);
        return;
    }
     _ds18sensors->requestTemperatures(); // Send the command to get temperatures
    _temp = _ds18sensors->getTempCByIndex(0); // read first sensor
    if(_temp != DEVICE_DISCONNECTED_C) {
        _dataStorage->addData(_channelName + CHANNELEXTTEMP , _temp);
    } else {
        _dataStorage->addData(_channelName + CHANNELEXTTEMP , 0.0f);
    }
}

///
/// Returns this channelsName
std::string DTSDataCollector::getName(){
    return _channelName;
}

///
/// fake at the moment, set up sleep mode
void  DTSDataCollector::sleep() {
    ;
}
