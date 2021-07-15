/*
 * BTDataActor.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <BTDataActor.h>
#include <Arduino.h>

// class MyServerCallbacks: public BLEServerCallbacks {
//     void onConnect(BLEServer* pServer) {
//       Serial.println("Connected");
//       //BLEDevice::startAdvertising();
//     };
//     void onDisconnect(BLEServer* pServer) {
//       Serial.println("DISConnected");
//     }
// };
// class MyCallbacks: public BLECharacteristicCallbacks {
//   	void onNotify(BLECharacteristic* pChar){
//           Serial.printf("OnNotify %s",pChar->getData() );
//       }
// 	void onStatus(BLECharacteristic* pChar, Status s, uint32_t code){
//         Serial.printf("OnStatus %s",pChar->getData() );
//     }
//     void onWrite(BLECharacteristic *pCharacteristic) {
//       std::string value = pCharacteristic->getValue();
//       if (value.length() > 0) {
//         Serial.println("*********");
//         Serial.print("New value: ");
//         for (int i = 0; i < value.length(); i++)
//           Serial.print(value[i]);
//         Serial.println();
//         Serial.println("*********");
//       }
//     }
// };

void BTDataActor::setSpeedCallback(void (*spcb)(int s))
{
  _speedCallBack=spcb;
}

void BTDataActor::onConnect(BLEServer* pServer) {
  Serial.println("Connected");
  _speedCallBack(250);
};

void BTDataActor::onDisconnect(BLEServer* pServer) {
  Serial.println("DISConnected");
  _pAdvertising->start();
  _speedCallBack(5000);
}

///
/// Instanciation of BTDataActor 
BTDataActor::BTDataActor(std::string deviceName)
{
  _deviceName = deviceName;
}

BLECharacteristic*  BTDataActor::createDataChartacteristic(BLEUUID uuid)
{
  return _pService->createCharacteristic(
                                        uuid,
                                        BLECharacteristic::PROPERTY_READ 
                                        // | BLECharacteristic::PROPERTY_WRITE
                                        | BLECharacteristic::PROPERTY_NOTIFY 
                                        | BLECharacteristic::PROPERTY_INDICATE
                                    );
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
  BLECharacteristic *tmp = createDataChartacteristic(BLEUUID(strUUID));
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