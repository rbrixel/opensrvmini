#include <MPUDataCollector.h>
#define MPUCHEXTACCX ".ACCX"
#define MPUCHEXTACCY ".ACCY"
#define MPUCHEXTACCZ ".ACCZ"

#define MPUCHEXTGYRX ".GYRX"
#define MPUCHEXTGYRY ".GYRY"
#define MPUCHEXTGYRZ ".GYRZ"

///
/// Constructing using default channelname
MPUDataCollector::MPUDataCollector()
{
    _mpu = new Adafruit_MPU6050();
}

///
/// Constructing with a channelName
MPUDataCollector::MPUDataCollector(std::string channelName)
{
    _channelName = channelName;
    _mpu = new Adafruit_MPU6050();
}

///
/// Initializes the Component and its DataStorage
void MPUDataCollector::init(IDataStorage *storage)
{
    // Hold a link to the datastorage. But update only in update() call.
    _dataStorage = storage;
 
    if (!_mpu->begin()) {
        Serial.println("Sensor init failed");
        _isInitialized = false;
        return;
    }
    _isInitialized = true;
    Serial.println("Found a MPU-6050 sensor");
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
    if (!_mpu->begin()) {
        Serial.println("Sensor init failed");
        _isInitialized = false;
        return;
    }
    _isInitialized = true;
    Serial.println("Found a MPU-6050 sensor");
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
        _dataStorage->addData(_channelName + MPUCHEXTGYRX , result);
        _dataStorage->addData(_channelName + MPUCHEXTGYRY , result);
        _dataStorage->addData(_channelName + MPUCHEXTGYRZ , result);
    #else
        sensors_event_t a, g, temp;
        _mpu->getEvent(&a, &g, &temp);
        _dataStorage->addData(_channelName + CHANNELEXTFAXE , a.acceleration.x);
        _dataStorage->addData(_channelName + CHANNELEXTFAXE , a.acceleration.y);
        _dataStorage->addData(_channelName + CHANNELEXTFAXE , a.acceleration.z);
        _dataStorage->addData(_channelName + CHANNELEXTFAXE , g.gyro.x);
        _dataStorage->addData(_channelName + CHANNELEXTFAXE , g.gyro.y);
        _dataStorage->addData(_channelName + CHANNELEXTFAXE , g.gyro.z);
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
