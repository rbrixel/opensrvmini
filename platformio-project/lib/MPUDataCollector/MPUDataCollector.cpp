/*
 * MPUDataCollector.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <MPUDataCollector.h>
#define MPUCHEXTACCX ".ACCX"
#define MPUCHEXTACCY ".ACCY"
#define MPUCHEXTACCZ ".ACCZ"

// #define MPUCHEXTGYRX ".GYRX"
// #define MPUCHEXTGYRY ".GYRY"
// #define MPUCHEXTGYRZ ".GYRZ"

///
/// Constructing using default channelname
MPUDataCollector::MPUDataCollector()
{
    // _mpu = new Adafruit_MPU6050();

    _smoother_X = new Smoother(15);
    _smoother_Y = new Smoother(15);;
    _smoother_Z = new Smoother(15);;

 
    // time=millis();
}

///
/// Constructing with a channelName
MPUDataCollector::MPUDataCollector(std::string channelName)
{
    _channelName = channelName;
    // _mpu = new Adafruit_MPU6050();
}

///
/// Initializes the Component and its DataStorage
void MPUDataCollector::init(IDataStorage *storage)
{
    // Hold a link to the datastorage. But update only in update() call.
    _dataStorage = storage;

    _smoother_X->cleanUp();
    _smoother_Y->cleanUp();
    _smoother_Z->cleanUp();

    Wire.begin();
    Wire.beginTransmission(_mpuAddr); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);

    // if (!_mpu->begin()) {
    //     Serial.println("Sensor init failed");
    //     _isInitialized = false;
    //     return;
    // }
    // _isInitialized = true;
    // Serial.println("Found a MPU-6050 sensor");
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
    _smoother_X->cleanUp();
    _smoother_Y->cleanUp();
    _smoother_Z->cleanUp();

    Wire.begin();
    Wire.beginTransmission(_mpuAddr); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
    // if (!_mpu->begin()) {
    //     Serial.println("Sensor init failed");
    //     _isInitialized = false;
    //     return;
    // }
    // _isInitialized = true;
    // Serial.println("Found a MPU-6050 sensor");
}

/// 
/// Updates Data into DataStorage
void MPUDataCollector::updateData()
{
    #ifdef OPENSRVDEBUG
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

        Wire.beginTransmission(_mpuAddr);
        Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
        Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
        Wire.requestFrom(_mpuAddr, 7*2, true); // request a total of 7*2=14 registers

        // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
        _accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
        _accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
        _accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
        //temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
        //gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
        //gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
        //gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
        _smoother_X->pushValue(_accelerometer_x);
        _smoother_Y->pushValue(_accelerometer_y);
        _smoother_Z->pushValue(_accelerometer_z);
        int16_t tmpX = _smoother_X->getSmoothed();
        int16_t tmpY = _smoother_Y->getSmoothed();
        int16_t tmpZ = _smoother_Z->getSmoothed();

        // sensors_event_t a, g, temp;
        // _mpu->getEvent(&a, &g, &temp);
        _dataStorage->addData(_channelName + MPUCHEXTACCX , tmpX);
        _dataStorage->addData(_channelName + MPUCHEXTACCY , tmpY);
        _dataStorage->addData(_channelName + MPUCHEXTACCZ , tmpZ);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRX , g.gyro.x);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRY , g.gyro.y);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRZ , g.gyro.z);
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
