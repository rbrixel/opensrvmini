/*
 * MPUDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef MPUDATACOLLECTOR_H
#define MPUDATACOLLECTOR_H

#include <IDataCollector.h>
#include <Arduino.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

#include <Smoother.h>
#include <Wire.h>

#include <random>

// Comment this line to acitivate Real Sensors
// #define OPENSRVDEBUG 

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
              // Adafruit_MPU6050 *_mpu;
              std::string _channelName = "MPU6050";


              Smoother *_smoother_X;
              Smoother *_smoother_Y;
              Smoother *_smoother_Z;
              const int _mpuAddr = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
              int16_t _accelerometer_x, _accelerometer_y, _accelerometer_z; // variables for accelerometer raw data
              // int16_t _gyro_x, _gyro_y, _gyro_z; // variables for gyro raw data
              
};

#endif