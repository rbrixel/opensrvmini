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
#define SERVICE_UUID        "43633cad-1a87-42a6-9cde-7e2d437846dd" 
//"e07ec36a-ab39-11eb-bcbc-0242ac130002"
#define CHARACTERISTIC_UUID "e51a315c-ab39-11eb-bcbc-0242ac130002"
//#define CHARACTERISTIC2_UUID "f53ab440-b18c-11eb-8529-0242ac130003"

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
       protected:
              BLEServer *_pServer;
              BLEService *_pService;
              BLECharacteristic *_pCharacteristic;
              BLEAdvertising *_pAdvertising;
              BLEAdvertisementData *_pAdvertisementData;
              void (*_speedCallBack)(int s);
              std::string _deviceName;
};

#endif