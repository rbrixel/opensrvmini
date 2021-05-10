/*
 * BMEDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef BMEDATACOLLECTOR_H
#define BMEDATACOLLECTOR_H

// Comment this line to acitivate Real Sensors
#define BMEDATACOLLECTOR_H_DEBUG 

#include <IDataCollector.h>
#include <BME280_I2C.h>

#ifdef BMEDATACOLLECTOR_H_DEBUG
#include <random>
#endif

///
/// BMEDataCollector implementing IDataCollector
/// used to collect temperature and pressure from a BME280 sensor
class BMEDataCollector : public IDataCollector {
       public:
              BMEDataCollector(std::string channelName);
              BMEDataCollector(std::string channelName, byte address,byte i2cSDA,byte i2cSCL);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              bool needsReInit();
              std::string getName();
       protected:
              BME280_I2C *_bme280; //BME280_I2C bme(0x76); // uint i2c-address
              std::string _channelName = "BMECollector";
              void readBMEData();     
              double _temp = 0;
              double _pressure = 0;    
              bool _bmeIsReady=0;
              byte _i2cSDA=25;
              byte _i2cSCL=26;
              byte _address=0x76;
};

#endif