/*
 * BTDataActor.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <BTDataActor.h>
#include <Arduino.h>

void BTDataActor::setSpeedCallback(void (*spcb)(int s))
{
  _speedCallBack=spcb;
}

/* ****************************************  BLE Server Callbacks         ************************ */
void BTDataActor::onConnect(BLEServer* pServer) {
  log_w("Connected");
  _speedCallBack(1500);
};

void BTDataActor::onDisconnect(BLEServer* pServer) {
  log_w("DISConnected");
  _pAdvertising->start();
  _speedCallBack(5000);
}

/* ****************************************  BLE Characteristic Callbacks ************************* */
void BTDataActor::onNotify(BLECharacteristic* pChar){
    log_w("OnNotify %s",pChar->getData() );
}

void BTDataActor::onStatus(BLECharacteristic* pChar, Status s, uint32_t code){
    log_w("OnStatus %s",pChar->getData() );
}

void BTDataActor::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  if (value.length()==4){
    std::memcpy(&_configuration, value.c_str(), sizeof(uint32_t));
    log_w("onWrite set configuration to [%d] *******************************************************************************",_configuration);
  }else{
    log_w("Ignoring reveived but invalid configuration data. Expects uint32. Keep [%d].",_configuration);
  }
}


///
/// Instanciation of BTDataActor 
BTDataActor::BTDataActor(std::string deviceName)
{
  _deviceName = deviceName;
}

///
/// Creates a BLECharacteristic with the given UUID (string parameter) not writable
BLECharacteristic*  BTDataActor::createDataCharacteristic(std::string strUUID)
{
  return createDataCharacteristic(BLEUUID(strUUID),false);
}

///
/// Creates a BLECharacteristic with the given UUID (BLEUUID parameter) not writable
BLECharacteristic*  BTDataActor::createDataCharacteristic(BLEUUID uuid)
{
  return createDataCharacteristic(uuid,false);
}

///
/// Creates a BLECharacteristic with the given UUID (string parameter), sets writable mode by parameter
BLECharacteristic*  BTDataActor::createDataCharacteristic(std::string strUUID, bool writable)
{
  return createDataCharacteristic(BLEUUID(strUUID), writable);
}

///
/// Creates a BLECharacteristic with the given UUID (BLEUUID parameter) sets writable mode by parameter
BLECharacteristic*  BTDataActor::createDataCharacteristic(BLEUUID uuid, bool writable)
{
  if (writable)
  {
    return _pService->createCharacteristic(
                                          uuid,
                                            BLECharacteristic::PROPERTY_READ 
                                          | BLECharacteristic::PROPERTY_WRITE
                                          | BLECharacteristic::PROPERTY_NOTIFY 
                                          | BLECharacteristic::PROPERTY_INDICATE
                                      );
  }else{
    return _pService->createCharacteristic(
                                          uuid,
                                            BLECharacteristic::PROPERTY_READ 
                                          | BLECharacteristic::PROPERTY_NOTIFY 
                                          | BLECharacteristic::PROPERTY_INDICATE
                                      );
  }
}

///
/// Initializes the Component and its DataStorage
void BTDataActor::init()
{
    BLEDevice::init(_deviceName);
    _pServer = BLEDevice::createServer();
    _pService = _pServer->createService(SERVICE_UUID);

    addCreateChannelCharacteristic(CHARACTERISTIC_TEMP_INHOUSE_UUID);
    addCreateChannelCharacteristic(CHARACTERISTIC_TEMP_OUTDOOR_UUID);
    addCreateChannelCharacteristic(CHARACTERISTIC_ROLL_X_UUID);
    addCreateChannelCharacteristic(CHARACTERISTIC_ROLL_Y_UUID);
    addCreateChannelCharacteristic(CHARACTERISTIC_ROLL_Z_UUID);
    _versionCharacteristic = createDataCharacteristic(CHARACTERISTIC_VERSION_UUID);    // Version for Protocol validation
    _configurationCharacteristic = createDataCharacteristic(CHARACTERISTIC_CONFIGURATION_UUID, true);    // Configuration Exchante, client writes new whiches into this bitmask
    _configurationCharacteristic->setCallbacks(this);

    _pService->start();
    
    _pServer->setCallbacks(this);
    _pAdvertising = _pServer->getAdvertising();
    _pAdvertising->addServiceUUID(SERVICE_UUID);
    _pAdvertising->setScanResponse(true);

    // _pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    // _pAdvertising->setMinPreferred(0x12);
    //_pAdvertisementData = new BLEAdvertisementData();
    //BLEAdvertisementData advertisementData;
    //_pAdvertisementData->setManufacturerData("  OpenSRVmini");
    //_pAdvertisementData->setShortName("osrv");
    //_pAdvertising->setAdvertisementData(*_pAdvertisementData);

    _pAdvertising->start();
}

///
/// creates a ChannelCharacteristic with the given UUID and adds it to the internal Map
void BTDataActor::addCreateChannelCharacteristic(std::string strUUID)
{
  BLECharacteristic *tmp = createDataCharacteristic(BLEUUID(strUUID));
  _channelCharacteristicMap[strUUID] = tmp;
}

///
/// Initializes the component
void BTDataActor::reInit()
{
    ;
}

/// 
/// Updates BME Data into DataStorage
void BTDataActor::action(IDataStorage *dataStorage)
{
    std::map<std::string, float> data = dataStorage->getMapCopy();
    std::map<std::string, float>::iterator it;
    std::string value;  
    //log_w("Action BLE\n");

    // Send Version with each update
    _versionCharacteristic->setValue( _bleVersion );
    _versionCharacteristic->notify(true);
    
    _configurationCharacteristic->setValue(_configuration);
    _versionCharacteristic->notify(true);

    BLECharacteristic *worker;
    for (it = data.begin(); it != data.end(); it++)
    {
        //char tmp[255];
        if (!isChannelRegistered(it->first))
        {
          log_w("Channel '%s' not registered ",it->first.c_str());
          continue;
        }
        std::string uuid = getChannelUUID(it->first);

        worker = _channelCharacteristicMap[uuid];
        worker->setValue(it->second);
        worker->notify(true);
        log_w("Update Channel [%s] to  UUID [%s] with Value =\t[%.2f]",it->first.c_str(), uuid.c_str(),it->second);
    }
}

///
/// fake at the moment, set up sleep mode
void  BTDataActor::sleep() {
    ; // Maybe Default Output if sleeping?
}

/************************* MAP DATA */
///
/// Registgers the channel with the given UUID#
/// the channel will be taken from datastorage and published to the connected device using the uuid as characteristics uuid
void  BTDataActor::registerMapData(std::string channelName, std::string UUID)
{
    _channelUUIDMap[channelName] = UUID;
}

///
/// registers the channel for the Inhouse Temperature UUID
void BTDataActor::registerTempInhouseChannel(std::string channelName)
{
  registerMapData(channelName, CHARACTERISTIC_TEMP_INHOUSE_UUID);
}

///
/// registers the channel for the Outdoor Temperature UUID
void BTDataActor::registerTempOutdoorChannel(std::string channelName)
{
  registerMapData(channelName, CHARACTERISTIC_TEMP_OUTDOOR_UUID);
}

///
/// registers the channel for the Roll X UUID
void BTDataActor::registerRollXChannel(std::string channelName)
{
  registerMapData(channelName, CHARACTERISTIC_ROLL_X_UUID);
}

///
/// registers the channel for the Roll Y UUID
void BTDataActor::registerRollYChannel(std::string channelName)
{
  registerMapData(channelName, CHARACTERISTIC_ROLL_Y_UUID);
}

///
/// registers the channel for the Roll Z UUID
void BTDataActor::registerRollZChannel(std::string channelName)
{
  registerMapData(channelName, CHARACTERISTIC_ROLL_Z_UUID);
}

///
/// Retrieves the UUID for a registered channel or an empty string if channel is not registered
std::string BTDataActor::getChannelUUID(std::string channelName)
{
    std::map<std::string, std::string>::iterator it = _channelUUIDMap.find(channelName);
    if (it == _channelUUIDMap.end())
    {
        return "";
    }
    
    return _channelUUIDMap[channelName];
}

///
/// Determines if the channel is allready registered before
bool BTDataActor::isChannelRegistered(std::string channelName)
{
    if (_channelUUIDMap.end() == _channelUUIDMap.find(channelName))
    {
        return false;
    }
    return true;
}