/*
 * BTDataActor.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef BTDATAACTOR_H
#define BTDATAACTOR_H

#include <IDataActor.h>

//#include "BluetoothSerial.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                      "43633cad-1a87-42a6-9cde-7e2d437846dd" 

#define CHARACTERISTIC_TEMP_INHOUSE_UUID  "e51a315c-ab39-11eb-bcbc-0242ac130002"
#define CHARACTERISTIC_TEMP_OUTDOOR_UUID  "324c5286-eb49-4f24-89cb-d29b1bd98778"
#define CHARACTERISTIC_ROLL_X_UUID        "3adaba65-1b1e-49fb-9d40-8989bc6f2af4"
#define CHARACTERISTIC_ROLL_Y_UUID        "9ae1fe2b-7403-46b3-be19-60957342bf1a"
#define CHARACTERISTIC_ROLL_Z_UUID        "31f84bd7-a740-4691-ae91-ffd5a373680b"


///
/// BTDataActor is configured to observe a datastorage and sends out all Data via Bluetooth
class BTDataActor : public IDataActor , public BLEServerCallbacks { 
       public:
              BTDataActor(std::string deviceName);
              void init();
              void reInit();
              void sleep();
              void action(IDataStorage *dataStorage);

              void setSpeedCallback(void (*fcnPtr)(int s));

              void onDisconnect(BLEServer* pServer);
              void onConnect(BLEServer* pServer);

              void registerTempInhouseChannel(std::string channelName);
              void registerTempOutdoorChannel(std::string channelName);
              void registerRollXChannel(std::string channelName);
              void registerRollYChannel(std::string channelName);
              void registerRollZChannel(std::string channelName);

       protected:
              std::map<std::string, std::string> _channelUUIDMap;
              std::map<std::string, BLECharacteristic*> _channelCharacteristicMap;
              BLEServer *_pServer;
              BLEService *_pService;

              BLEAdvertising *_pAdvertising;
              BLEAdvertisementData *_pAdvertisementData;
              void (*_speedCallBack)(int s);
              std::string _deviceName;
              float count=0;
              BLECharacteristic* createDataChartacteristic(BLEUUID uuid);
              void addCreateChannelCharacteristic(std::string strUUID);

              void registerMapData(std::string channelName, std::string strUUID);
              std::string getChannelUUID(std::string channelName);
              bool isChannelRegistered(std::string channelName);

              
};

#endif