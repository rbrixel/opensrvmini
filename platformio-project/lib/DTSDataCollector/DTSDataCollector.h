/*
 * DTSDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef DTSDATACOLLECTOR_H
#define DTSDATACOLLECTOR_H

// Comment this line to acitivate Real Sensors
//#define DTSDATACOLLECTOR_H_DEBUG 

#include <IDataCollector.h>
#include <random>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5 // GPIO 5 / "D5"


///
/// DTSDataCollector implementing IDataCollector
/// used to collect temperature and pressure from a DS18B2ß sensor
class DTSDataCollector : public IDataCollector {
       public:
              DTSDataCollector(std::string channelName);
              DTSDataCollector(std::string channelName,uint8_t oneWirePin);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              std::string getName();
       protected:
              OneWire *_oneWire;
              DallasTemperature *_ds18sensors;

              std::string _channelName = "DTSCollector";
              uint8_t _oneWirePin = 5; // GPIO 5 / "D5"
              double _temp = 0;
              double _pressure = 0;    
};

#endif