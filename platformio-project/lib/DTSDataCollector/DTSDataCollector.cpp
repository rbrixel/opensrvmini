/*
 * DTSDataCollector.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <DTSDataCollector.h>
#include <Arduino.h>

#define CHANNELEXTTEMP ".TEMP"
#define CHANNELEXTPRESSURE ".PRES"

///
/// Constructing with a channelName
DTSDataCollector::DTSDataCollector(std::string channelName)
{
    _channelName = channelName;
    _oneWire = new OneWire(_oneWirePin);
    _ds18sensors = new DallasTemperature(_oneWire);
}

///
/// Constructing with a channelName and a onewire-pin
DTSDataCollector::DTSDataCollector(std::string channelName, uint8_t oneWirePin)
{
    _channelName = channelName;
    _oneWirePin = oneWirePin;
    _oneWire = new OneWire(_oneWirePin);
    _ds18sensors = new DallasTemperature(_oneWire);
}

///
/// Constructing with a channelName 
/// a positive or degative temperatureOffset to adjust sensor tolerances
/// a onewire-pin
DTSDataCollector::DTSDataCollector(std::string channelName, float temperatureOffset,uint8_t oneWirePin)
{
    _channelName = channelName;
    _oneWirePin = oneWirePin;
    _oneWire = new OneWire(_oneWirePin);
    _temperatureOffset = temperatureOffset;
    _ds18sensors = new DallasTemperature(_oneWire);
}

///
/// Initializes the Component and its DataStorage
void DTSDataCollector::init(IDataStorage *storage)
{
    _dataStorage = storage;
    //_ds18sensors->begin(); // #rb: begin() wird nicht benötigt - geht auch ohne! Ist wohl so Arduino-IDE-Ding ;)
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
    #ifdef DTSDATACOLLECTOR_H_DEBUG
        log_w("DEBUG CODE ACTIVE! RANDOM DATA");
        long randomVal = random(-200,400);
        float result = randomVal/10;
        _dataStorage->addData(_channelName + CHANNELEXTTEMP, result);
    #else
        // #rb: Der getDeviceCount führt dazu, dass die FW den Sensor nicht findet
        /*
        if (_ds18sensors->getDeviceCount()==0){
            log_w("SENSOR DS18B20 (GPIO5) NOT FOUND!");
            _dataStorage->addData(_channelName + CHANNELEXTTEMP , 0.0f);
            return;
        }
        */
        _ds18sensors->requestTemperatures(); // Send the command to get temperatures
        _temp = _ds18sensors->getTempCByIndex(0); // read first sensor
        if(_temp != DEVICE_DISCONNECTED_C) {
            _dataStorage->addData(_channelName + CHANNELEXTTEMP , _temp + _temperatureOffset);
        } else {
            _dataStorage->addData(_channelName + CHANNELEXTTEMP , 0.0f);
        }
    #endif

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
