/*
 * MPUDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */

// Comment this line to acitivate Real Sensors
// #define MPUDATACOLLECTOR_H_DEBUG 

#ifndef MPUDATACOLLECTOR_H
#define MPUDATACOLLECTOR_H

#include <IDataCollector.h>
#include <Arduino.h>

//#include <Smoother.h>
#include <Wire.h>
#include <MPU6050_light.h>

//#define MPUDATACOLLECTOR_H_DEBUG
#ifdef MPUDATACOLLECTOR_H_DEBUG 
#include <random>
#endif

struct MPUTaskData
{
    MPU6050 *initializedMPU;
    volatile float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
    volatile float angleAccX, angleAccY;
    volatile float angleX, angleY, angleZ;
};

///
/// MPUDataCollector implementing IDataCollector
/// Take a copy of this to start implementing your own collector
class MPUDataCollector : public IDataCollector {
       public:
              MPUDataCollector(const unsigned int sdaPIN = GPIO_NUM_19,const unsigned int sclPIN  = GPIO_NUM_18 );
              MPUDataCollector(std::string channelName,const unsigned int sdaPIN = GPIO_NUM_19,const unsigned int sclPIN  = GPIO_NUM_18 );
              MPUDataCollector(std::string channelName, TwoWire *lightWire );
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              bool needsReInit();
              static void updateDataThread( void * parameter);
              std::string getName();
       protected:
              TwoWire *_lightWire;
              MPU6050 *_mpu;
              
              int _sdaPIN=GPIO_NUM_19;
              int _sclPIN=GPIO_NUM_18;
              
              bool _isInitialized = false;
              std::string _channelName = "MPU6050";
 
              // Threading
              TaskHandle_t getDataTask;
              MPUTaskData *_data;
};

#endif