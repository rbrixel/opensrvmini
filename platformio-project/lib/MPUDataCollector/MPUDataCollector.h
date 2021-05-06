#ifndef MPUDATACOLLECTOR_H
#define MPUDATACOLLECTOR_H

#include <IDataCollector.h>
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <random>

// Comment this line to acitivate Real Sensors
#define OPENSRVDEBUG 

///
/// MPUDataCollector implementing IDataCollector
/// Take a copy of this to start implementing your own collector
class MPUDataCollector : public IDataCollector {
       public:
              MPUDataCollector();
              MPUDataCollector(std::string channelName);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              bool needsReInit();
              std::string getName();
       protected:
              bool _isInitialized = false;
              Adafruit_MPU6050 *_mpu;
              std::string _channelName = "MPU6050";
              
};

#endif