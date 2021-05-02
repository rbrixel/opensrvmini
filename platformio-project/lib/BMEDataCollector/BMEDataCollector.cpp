#include <BMEDataCollector.h>
#include <Arduino.h>
#define CHANNELEXTTEMP ".TEMP"
#define CHANNELEXTPRESSURE ".PRES"

///
/// Constructing with a channelName
BMEDataCollector::BMEDataCollector(std::string channelName)
{
    _channelName = channelName;
    _bme280 = new BME280_I2C(_address,_i2cSDA, _i2cSCL);
}

BMEDataCollector::BMEDataCollector(std::string channelName, byte address,byte i2cSDA,byte i2cSCL)
{
    _channelName = channelName;
    _i2cSDA=i2cSDA;
    _i2cSCL=i2cSCL;
    _address=address;
    _bme280 = new BME280_I2C(_address,_i2cSDA, _i2cSCL);
}

///
/// Initializes the Component and its DataStorage
void BMEDataCollector::init(IDataStorage *storage)
{
    _dataStorage = storage;

    _bmeIsReady = _bme280->begin(
                                _bme280->BME280_STANDBY_0_5,
                                _bme280->BME280_FILTER_16,
                                _bme280->BME280_SPI3_DISABLE,
                                _bme280->BME280_OVERSAMPLING_2,
                                _bme280->BME280_OVERSAMPLING_16,
                                _bme280->BME280_OVERSAMPLING_1,
                                _bme280->BME280_MODE_NORMAL);

    // if (!_bmeIsReady) {
    //   Serial.println("can NOT initialize for using BME280.\n");
    // } else {
    //   Serial.println("ready to using BME280.\n");
    // }
}

///
/// Initializes the component
void BMEDataCollector::reInit()
{
    ;
}

/// 
/// Updates BME Data into DataStorage
void BMEDataCollector::updateData()
{
    if (!_bmeIsReady) {
        #ifdef OPENSRVDEBUG
            Serial.println("DEBUG CODE ACTIVE! RANDOM DATA");
            long randomVal = random(-200,400);
            double result = randomVal/10;
            long randomVal2 = random(90000,110000);
            double result2 = randomVal2;
            _dataStorage->addData(_channelName + CHANNELEXTTEMP , result);
            _dataStorage->addData(_channelName + CHANNELEXTPRESSURE ,result2);
        #else
            _dataStorage->addData(_channelName + CHANNELEXTTEMP , 0.0f);
            _dataStorage->addData(_channelName + CHANNELEXTPRESSURE ,0.0f);
        #endif
        return;
    }
    readBMEData();
    _dataStorage->addData(_channelName + CHANNELEXTTEMP , _temp);
    _dataStorage->addData(_channelName + CHANNELEXTPRESSURE , _pressure);
}

///
/// Returns this channelsName
std::string BMEDataCollector::getName(){
    return _channelName;
}

///
/// fake at the moment, Reads BME Data
void  BMEDataCollector::readBMEData() {
    _bme280->read();
    _temp = _bme280->data.temperature;
    _pressure = _bme280->data.pressure;
}

///
/// fake at the moment, set up sleep mode
void  BMEDataCollector::sleep() {
    ;
}
