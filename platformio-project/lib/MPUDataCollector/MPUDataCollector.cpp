/*
 * MPUDataCollector.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <MPUDataCollector.h>

#define MPUCHEXTTEMP        ".TEMP"
#define MPUCHEXTACCX        ".ACCX"
#define MPUCHEXTACCY        ".ACCY"
#define MPUCHEXTACCZ        ".ACCZ"
#define MPUCHEXTGYRX        ".GYRX"
#define MPUCHEXTGYRY        ".GYRY"
#define MPUCHEXTGYRZ        ".GYRZ"
#define MPUCHEXTACCANGLEX   ".ACAX"
#define MPUCHEXTACCANGLEY   ".ACAY"
#define MPUCHEXTANGLEX      ".ANGX"
#define MPUCHEXTANGLEY      ".ANGY"
#define MPUCHEXTANGLEZ      ".ANGZ"


///
/// Constructing using default channelname
MPUDataCollector::MPUDataCollector(const unsigned int sdaPIN,const unsigned int sclPIN)
{
    _data = new MPUTaskData();
    _sdaPIN = sdaPIN;
    _sclPIN = sclPIN;
    _lightWire = new TwoWire(0);
    _lightWire->setPins(_sdaPIN,_sclPIN);
    _lightWire->begin();
    _mpu = new MPU6050(*_lightWire);
}

///
/// Constructing with a channelName
MPUDataCollector::MPUDataCollector(std::string channelName, const unsigned int sdaPIN ,const unsigned int sclPIN)
{
    _data = new MPUTaskData();
    _channelName = channelName;
    _sdaPIN = sdaPIN;
    _sclPIN = sclPIN;
    _lightWire = new TwoWire(0);
    _lightWire->setPins(_sdaPIN,_sclPIN);
    _lightWire->begin();
    _mpu = new MPU6050(*_lightWire);
}

///
/// Constructing with a channelName
MPUDataCollector::MPUDataCollector(std::string channelName, TwoWire *lightWire)
{
    _data = new MPUTaskData();
    _channelName = channelName;
    _lightWire= lightWire;
    _mpu = new MPU6050(*_lightWire);
}

void MPUDataCollector::updateDataThread( void * taskData)
{
    MPUTaskData *localTaskData = (MPUTaskData*) taskData;
    for(;;) {

        localTaskData->initializedMPU->update();

        localTaskData->temp  = localTaskData->initializedMPU->getTemp();
        localTaskData->accX  = localTaskData->initializedMPU->getAccX();
        localTaskData->accY  = localTaskData->initializedMPU->getAccY();
        localTaskData->accZ  = localTaskData->initializedMPU->getAccZ();
        localTaskData->gyroX = localTaskData->initializedMPU->getGyroX();
        localTaskData->gyroY = localTaskData->initializedMPU->getGyroY();
        localTaskData->gyroZ = localTaskData->initializedMPU->getGyroZ();
        localTaskData->angleAccX  = localTaskData->initializedMPU->getAccAngleX();
        localTaskData->angleAccY  = localTaskData->initializedMPU->getAccAngleY();
        localTaskData->angleX  = localTaskData->initializedMPU->getAngleX();
        localTaskData->angleY  = localTaskData->initializedMPU->getAngleY();
        localTaskData->angleZ  = localTaskData->initializedMPU->getAngleZ();

        delay(100);
    }
}

///
/// Initializes the Component and its DataStorage
void MPUDataCollector::init(IDataStorage *storage)
{
    // Hold a link to the datastorage. But update only in update() call.
    _dataStorage = storage;

    byte status =_mpu->begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while(status!=0){ } // stop everything if could not connect to MPU6050

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    _mpu->calcOffsets(true,true); // gyro and accelero
    Serial.println("Done!\n");

    _data->initializedMPU = _mpu;

    xTaskCreatePinnedToCore(
        this->updateDataThread, /* Function to implement the task */
        "updateDataThread", /* Name of the task */
        10000,  /* Stack size in words */
        _data,  /* Task input parameter */
        0,  /* Priority of the task */
        &getDataTask,  /* Task handle. */
        0); /* Core where the task should run */
}

///
/// Initializes the Component and its DataStorage
bool MPUDataCollector::needsReInit()
{
    return _isInitialized;
}

///
/// Initializes the component
void MPUDataCollector::reInit()
{
    ;
}

/// 
/// Updates Data into DataStorage
void MPUDataCollector::updateData()
{
    #ifdef MPUDATACOLLECTOR_H_DEBUG
        Serial.println("DEBUG CODE ACTIVE! RANDOM DATA");
        long randomVal = random(-1000,1000);
        double result = randomVal/10;
        _dataStorage->addData(_channelName + MPUCHEXTACCX , result);
        _dataStorage->addData(_channelName + MPUCHEXTACCY , result);
        _dataStorage->addData(_channelName + MPUCHEXTACCZ , result);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRX , result);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRY , result);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRZ , result);
    #else
        // _mpu->update();
        _dataStorage->addData(_channelName + MPUCHEXTTEMP      , _data->temp);
        _dataStorage->addData(_channelName + MPUCHEXTACCX      , _data->accX );
        _dataStorage->addData(_channelName + MPUCHEXTACCY      , _data->accY );
        _dataStorage->addData(_channelName + MPUCHEXTACCZ      , _data->accZ );
        _dataStorage->addData(_channelName + MPUCHEXTGYRX      , _data->gyroX );
        _dataStorage->addData(_channelName + MPUCHEXTGYRY      , _data->gyroY );
        _dataStorage->addData(_channelName + MPUCHEXTGYRZ      , _data->gyroZ );
        _dataStorage->addData(_channelName + MPUCHEXTACCANGLEX , _data->angleAccX );
        _dataStorage->addData(_channelName + MPUCHEXTACCANGLEY , _data->angleAccY );
        _dataStorage->addData(_channelName + MPUCHEXTANGLEX    , _data->angleX );
        _dataStorage->addData(_channelName + MPUCHEXTANGLEY    , _data->angleY );
        _dataStorage->addData(_channelName + MPUCHEXTANGLEZ    , _data->angleZ );
    #endif
}

///
/// Returns this channelsName
std::string MPUDataCollector::getName(){
    return _channelName;
}

///
/// fake at the moment, set up sleep mode
void  MPUDataCollector::sleep() {
    ;
}
