/*
 * ADSDataCollector.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <ADSDataCollector.h>
#include <Arduino.h>

#define CHANNELEXTVCC ".VCC"

///
/// Constructing with a channelName
ADSDataCollector::ADSDataCollector(std::string channelName)
{
    _channelName = channelName;
    _ads = new ADS1115(_address);
}

ADSDataCollector::ADSDataCollector(std::string channelName, TwoWire *adsWire )
{
    _channelName = channelName;
    _adsWire = adsWire;
    _ads = new ADS1115(_address, adsWire);
}

ADSDataCollector::ADSDataCollector(std::string channelName, byte address, const uint8_t analogInputPin)
{
    _channelName = channelName;
    _address=address;
    _analogInputPin = analogInputPin;
    _ads = new ADS1115(_address);
}

///
/// Initializes the Component and its DataStorage
void ADSDataCollector::init(IDataStorage *storage)
{
    _dataStorage = storage;
}

///
/// Initializes the component
void ADSDataCollector::reInit()
{
    ;
}


/// 
/// Updates BME Data into DataStorage
void ADSDataCollector::updateData()
{
    #ifdef ADSDATACOLLECTOR_H_DEBUG
        Serial.println("DEBUG CODE ACTIVE! RANDOM DATA");
        long randomVal = random(100,160);
        double result = randomVal/10;
    #else
        float reference_vcc = 3.3; // Volt... ESP32 system voltage
        float vcc_max = 16.0; // Volt... maximum input on voltage divider
        int resistor_to_gnd = 12000; // Ohm... Voltage-divider: Resistor to GND
        int resistor_to_gnd_max = 12210; // Ohm... calculated maximum to near reference_vcc
        // With 47k to VCC and 12k to GND, I get 3.254V on voltage divider output
        // With a 12k210 resistor to GND, I get 3.299V
        // So use the two values for calculating calibration
        _ads->setGain(0);
        float vout = _ads->readADC(_analogInputPin) * _ads->toVoltage(1);

        float voutRaw = vout / (reference_vcc / vcc_max);
        float rFactor = (float)resistor_to_gnd_max / (float)resistor_to_gnd; // It is important to cast a float!!

        double result =  (voutRaw * rFactor);
    #endif
    
    _dataStorage->addData(_channelName + CHANNELEXTVCC , result);
}

///
/// Returns this channelsName
std::string ADSDataCollector::getName(){
    return _channelName;
}

///
/// fake at the moment, set up sleep mode
void  ADSDataCollector::sleep() {
    ;
}
