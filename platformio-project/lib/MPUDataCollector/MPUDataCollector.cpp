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

#ifdef MPU_USEGYRO
    #define MPUCHEXTGYRX ".GYRX"
    #define MPUCHEXTGYRY ".GYRY"
    #define MPUCHEXTGYRZ ".GYRZ"
#endif

#ifdef MPU_USETEMPERATURE
    #define MPUCHEXTTEMP ".TEMP"
#endif

///
/// Constructing using default channelname
MPUDataCollector::MPUDataCollector()
{
    // _mpu = new Adafruit_MPU6050();
    // mpuWire = new TwoWire(0);
    // mpuWire->setPins(19,18);
    // mpuWire->begin();
    lightWire.setPins(19,18);
    lightWire.begin();
    _mpu = new MPU6050(lightWire);

    // _smoother_X = new Smoother(3);
    // _smoother_Y = new Smoother(3);    
    // _smoother_Z = new Smoother(3);
}

///
/// Constructing with a channelName
MPUDataCollector::MPUDataCollector(std::string channelName)
{
    _channelName = channelName;
    // mpuWire = new TwoWire(0);
    // mpuWire->setPins(19,18);
    // mpuWire->begin();
    lightWire.setPins(19,18);
    lightWire.begin();
    _mpu = new MPU6050(lightWire);

    // _smoother_X = new Smoother(3);
    // _smoother_Y = new Smoother(3);
    // _smoother_Z = new Smoother(3);
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


    // mpuWire->begin();
    // mpuWire->beginTransmission(_mpuAddr); // Begins a transmission to the I2C slave (GY-521 board)
    // mpuWire->write(0x6B); // PWR_MGMT_1 register
    // mpuWire->write(0); // set to zero (wakes up the MPU-6050)
    // mpuWire->endTransmission(true);

    // _smoother_X->cleanUp();
    // _smoother_Y->cleanUp();
    // _smoother_Z->cleanUp();

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
    // mpuWire->begin();
    // mpuWire->beginTransmission(_mpuAddr); // Begins a transmission to the I2C slave (GY-521 board)
    // mpuWire->write(0x6B); // PWR_MGMT_1 register
    // mpuWire->write(0); // set to zero (wakes up the MPU-6050)
    // mpuWire->endTransmission(true);
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
        
    _mpu->update();

    Serial.print(F("TEMPERATURE: "));Serial.println(_mpu->getTemp());
    Serial.print(F("ACCELERO  X: "));Serial.print(_mpu->getAccX());
    Serial.print("\tY: ");Serial.print(_mpu->getAccY());
    Serial.print("\tZ: ");Serial.println(_mpu->getAccZ());
  
    Serial.print(F("GYRO      X: "));Serial.print(_mpu->getGyroX());
    Serial.print("\tY: ");Serial.print(_mpu->getGyroY());
    Serial.print("\tZ: ");Serial.println(_mpu->getGyroZ());
  
    Serial.print(F("ACC ANGLE X: "));Serial.print(_mpu->getAccAngleX());
    Serial.print("\tY: ");Serial.println(_mpu->getAccAngleY());
    
    Serial.print(F("ANGLE     X: "));Serial.print(_mpu->getAngleX());
    Serial.print("\tY: ");Serial.print(_mpu->getAngleY());
    Serial.print("\tZ: ");Serial.println(_mpu->getAngleZ());
    Serial.println(F("=====================================================\n"));
 
        
        // mpuWire->beginTransmission(_mpuAddr);
        // mpuWire->write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
        // mpuWire->endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
        // mpuWire->requestFrom(_mpuAddr, (int) 7*2, (int) true); // request a total of 7*2=14 registers
        
        // // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
        // _accelerometer_x = mpuWire->read()<<8 | mpuWire->read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
        // _accelerometer_y = mpuWire->read()<<8 | mpuWire->read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
        // _accelerometer_z = mpuWire->read()<<8 | mpuWire->read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)

        // #ifdef MPU_USETEMPERATURE
        // _temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
        // #endif 

        // #ifdef MPU_USEGYRO
        // _gyro_x = mpuWire->read()<<8 | mpuWire->read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
        // _gyro_y = mpuWire->read()<<8 | mpuWire->read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
        // _gyro_z = mpuWire->read()<<8 | mpuWire->read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
        // #endif

        // _smoother_X->pushValue(_accelerometer_x);
        // _smoother_Y->pushValue(_accelerometer_y);
        // _smoother_Z->pushValue(_accelerometer_z);

        // int16_t tmpX = _smoother_X->getSmoothed();
        // int16_t tmpY = _smoother_Y->getSmoothed();
        // int16_t tmpZ = _smoother_Z->getSmoothed();

        // _dataStorage->addData(_channelName + MPUCHEXTACCX , tmpX);
        // _dataStorage->addData(_channelName + MPUCHEXTACCY , tmpY);
        // _dataStorage->addData(_channelName + MPUCHEXTACCZ , tmpZ);

        // #ifdef MPU_USEGYRO
        // _dataStorage->addData(_channelName + MPUCHEXTGYRX , _gyro_x);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRY , _gyro_y);
        // _dataStorage->addData(_channelName + MPUCHEXTGYRZ , _gyro_z);
        // #endif

        // #ifdef MPU_USETEMPERATURE
        // _dataStorage->addData(_channelName + MPUCHEXTTEMP , _temperature);
        // #endif
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
