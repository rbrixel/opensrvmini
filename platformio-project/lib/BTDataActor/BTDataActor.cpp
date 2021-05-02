#include <BTDataActor.h>
#include <Arduino.h>


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};

///
/// Instanciation of BTDataActor 
BTDataActor::BTDataActor()
{

}

///
/// Initializes the Component and its DataStorage
void BTDataActor::init()
{
       // _serialBT.begin("ESP32"); //Name des ESP32;;
  BLEDevice::init("MyESP32");
  _pServer = BLEDevice::createServer();

  _pService = _pServer->createService(SERVICE_UUID);

  _pCharacteristic = _pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

    _pCharacteristic->setCallbacks(new MyCallbacks());

    _pCharacteristic->setValue("");
    _pService->start();

    BLEAdvertising *pAdvertising = _pServer->getAdvertising();
    pAdvertising->start();
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
    std::map<std::string, double> data = dataStorage->getMapCopy();
    std::map<std::string, double>::iterator it;
    std::string value;  
    Serial.printf("Action BLE\n");
    for (it = data.begin(); it != data.end(); it++)
    {
        char tmp[255];
        sprintf(tmp, "BLE: %s-%f\n",it->first.c_str(),it->second);
        value.append( tmp);
        Serial.printf("BLE: %s-%f\n",it->first.c_str(),it->second);
    }
    _pCharacteristic->setValue(value.c_str());
}

///
/// fake at the moment, set up sleep mode
void  BTDataActor::sleep() {
    ; // Maybe Default Output if sleeping?
}
