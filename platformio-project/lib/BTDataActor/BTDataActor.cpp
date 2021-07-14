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
  _speedCallBack(2500);
};

void BTDataActor::onDisconnect(BLEServer* pServer) {
  Serial.println("DISConnected");
  _pAdvertising->start();
  _speedCallBack(30000);
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

    _pTempInhouseCharacteristic = createDataChartacteristic(BLEUUID(CHARACTERISTIC_TEMP_INHOUSE_UUID));
    _pTempOutdoorCharacteristic = createDataChartacteristic(BLEUUID(CHARACTERISTIC_TEMP_OUTDOOR_UUID));
    _pRollXCharacteristic = createDataChartacteristic(BLEUUID(CHARACTERISTIC_ROLL_X_UUID));
    _pRollYCharacteristic = createDataChartacteristic(BLEUUID(CHARACTERISTIC_ROLL_Y_UUID));
    _pRollZCharacteristic = createDataChartacteristic(BLEUUID(CHARACTERISTIC_ROLL_Z_UUID));

    _pTempInhouseCharacteristic->setValue("_pTempInhouseCharacteristic");
    _pTempOutdoorCharacteristic->setValue("_pTempOutdoorCharacteristic");
    _pRollXCharacteristic->setValue("_pRollXCharacteristic");
    _pRollYCharacteristic->setValue("_pRollYCharacteristic");
    _pRollZCharacteristic->setValue("_pRollZCharacteristic");

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
    //Serial.printf("Action BLE\n");
    for (it = data.begin(); it != data.end(); it++)
    {
        char tmp[255];
        if (it->first.compare("Channel-BME.TEMP")==0)
        {
          _pTempInhouseCharacteristic->setValue(it->second);
          _pTempInhouseCharacteristic->notify(true);
          sprintf(tmp, "%s = e51a315c-ab39-11eb-bcbc-0242ac130002 = \t%.2f",it->first.c_str(),it->second);
          Serial.println(tmp);
        }
        if (it->first.compare("Channel-DTS.TEMP")==0)
        {
          _pTempOutdoorCharacteristic->setValue(it->second);
          _pTempOutdoorCharacteristic->notify(true);
          sprintf(tmp, "%s = 324c5286-eb49-4f24-89cb-d29b1bd98778 = \t%.2f",it->first.c_str(),it->second);
          Serial.println(tmp);
        }
        if (it->first.compare("Channel-MPU.ANGX")==0)
        {
          _pRollXCharacteristic->setValue(it->second);
          _pRollXCharacteristic->notify(true);
          sprintf(tmp, "%s= 3adaba65-1b1e-49fb-9d40-8989bc6f2af4 = \t%.2f",it->first.c_str(),it->second);
          Serial.println(tmp);
        }
        if (it->first.compare("Channel-MPU.ANGY")==0)
        {
          _pRollYCharacteristic->setValue(it->second);
          _pRollYCharacteristic->notify(true);
          sprintf(tmp, "%s= 9ae1fe2b-7403-46b3-be19-60957342bf1a = \t%.2f",it->first.c_str(),it->second);
          Serial.println(tmp);
        }
        if (it->first.compare("Channel-MPU.ANGZ")==0)
        {
          _pRollZCharacteristic->setValue(it->second);
          _pRollZCharacteristic->notify(true);
          sprintf(tmp, "%s = 31f84bd7-a740-4691-ae91-ffd5a373680b  \t=%.2f",it->first.c_str(),it->second);
          Serial.println(tmp);
        }
    }
}

///
/// fake at the moment, set up sleep mode
void  BTDataActor::sleep() {
    ; // Maybe Default Output if sleeping?
}
