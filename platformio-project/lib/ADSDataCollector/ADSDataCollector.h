/*
 * ADSDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef ADSDATACOLLECTOR_H
#define ADSDATACOLLECTOR_H

// Comment this line to acitivate Real Sensors
#define ADSDATACOLLECTOR_H_DEBUG 

#include <IDataCollector.h>
#include <ADS1X15.h>

#ifdef ADSDATACOLLECTOR_H_DEBUG
#include <random>
#endif

///
/// ADSDataCollector implementing IDataCollector
/// used to collect voltage of ADS1115 Sensor
class ADSDataCollector : public IDataCollector {
       public:
              ADSDataCollector(std::string channelName);
              ADSDataCollector(std::string channelName, byte address, uint8_t analogInputPin);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              std::string getName();
       protected:
              std::string _channelName = "ADSCollector";
              ADS1115 *_ads;
              byte _address=0x48;
              uint8_t _analogInputPin;
};

#endif