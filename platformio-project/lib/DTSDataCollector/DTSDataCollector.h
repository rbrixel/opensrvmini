/*
 * DTSDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef DTSDATACOLLECTOR_H
#define DTSDATACOLLECTOR_H

// Comment this line to acitivate Real Sensors
#define DTSDATACOLLECTOR_H_DEBUG 

#include <IDataCollector.h>
#include <random>
#include <OneWire.h>
#include <DallasTemperature.h>

//#define ONE_WIRE_BUS 5 // #rb: wird nicht benötigt; geht auch ohne


///
/// DTSDataCollector implementing IDataCollector
/// used to collect temperature and pressure from a DS18B2ß sensor
class DTSDataCollector : public IDataCollector {
       public:
              DTSDataCollector(std::string channelName);
              DTSDataCollector(std::string channelName,uint8_t oneWirePin);
              DTSDataCollector(std::string channelName, float temperatureOffset,uint8_t oneWirePin = 5);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              std::string getName();
       protected:
              OneWire *_oneWire;
              DallasTemperature *_ds18sensors;

              std::string _channelName = "DTSCollector";
              uint8_t _oneWirePin = GPIO_NUM_23;
              float _temperatureOffset = 0;
              float _temp = 0;
              float _pressure = 0;    
};

#endif