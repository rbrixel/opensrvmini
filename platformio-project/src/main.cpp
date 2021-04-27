/*
 * OpenSRVmini
 * -----------
 * 
 * EN: OpenSRVmini is suitable for mobile homes and caravans. You can use it to
 *     measure the outside temperature, the inside temperature and the air humidity.
 *     Furthermore, a position sensor is included!
 * DE: OpenSRVmini ist für Wohnmobile, sowie Wohnwägen geeignet. Du kannst damit
 *     die Außentemperatur, die Innentemperatur und die -Luftfeuchte messen.
 *     Des Weiteren ist ein Lagesensor dabei!
 * 
 * Hardware: 
 * - ESP32 "Dev Kit"
 * - BME280 (temperature, humidity and air pressure for indoor)
 * - MPU-6050 (3-axis-gyroscope)
 * - DS18B20 (waterproof for outdoor measurement)
 * - ADS1115 (4-Channel, 16-Bit AD, i2c)
 * 
 * Info:
 * - I use the ADS1115-AD-Converter, because of the bad AD in the ESP32
 * - Wifi is only used for OTA-Update
 * - Bluetooth is used for communication with smartphone-app
 * 
 * Libs: (not up to date)
 * - Over-The-Air-Update (ArduinoOTA)
 * - OneWire (OneWire - use 2.3.0)
 * - Cactus.io BME280 (http://cactus.io/projects/weather/arduino-weather-station-bme280-sensor)
 * 
 * Author:  René Brixel <mail@campingtech.de>
 * Support: Frank Weichert <opensrvmini@humanbyte.de
 * Date:    2021-04-22 (not so, but really initial Sketch)
 * VErsion: 20210422-1
 * Web:     https://campingtech.de/opensrv
 * GitHub:  https://github.com/rbrixel
 * 
 */

#include <main.h>

const char* ssid = "OtaStation";
const char* password = "WelcomeAboard!";

// Sample Usage of Interfaces for Data Collection
std::vector< IDataCollector *> dataCollectors;
IDataStorage *storage = new DataStorage();

void setup() {
  
  // Sample Usage of Interfaces for Data Collection
  dataCollectors.push_back(new BMEDataCollector("Channel-1"));
  dataCollectors.push_back(new BMEDataCollector("Channel-2"));
  dataCollectors.push_back(new BMEDataCollector("Channel-3"));

  // Initialize Collectors (with storage and maybe more)
  for(std::size_t i = 0; i < dataCollectors.size(); ++i) {
      Serial.printf("INIT: %s\n",dataCollectors[i]->getName().c_str() );
      dataCollectors[i]->init(storage);
  }



  Serial.begin(115200);
  Serial.println("Over The Air Update - FullOTA");

  Serial.printf("Connect to %s\n",ssid);
  Serial.printf("Sketch size: %u\n", ESP.getSketchSize());
  Serial.printf("Free size: %u\n", ESP.getFreeSketchSpace());

  WiFi.softAP(ssid,password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // OTA Settings
  ArduinoOTA.setPort(3232);
 
  // OTA Start
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));;Serial.println();
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

void loop() {
  ArduinoOTA.handle();

  // Sample Usage of Interfaces for Data Collection
  Serial.println("************************************");
  for(std::size_t i = 0; i < dataCollectors.size(); ++i) {
      dataCollectors[i]->updateData();

      Serial.printf("UPDATE: %s\n",dataCollectors[i]->getName().c_str() );
  }

  Serial.println("************************************");
  
  // Read Data out of Storage
  std::map<std::string, double> data = storage->getMapCopy();
  std::map<std::string, double>::iterator it;

  for (it = data.begin(); it != data.end(); it++)
  {
      Serial.printf("READ: %s -> %f\n", 
                                   it->first.c_str(),
                                   it->second);
  }
  // for(std::size_t i = 0; i < data.size(); ++i) {

  //     Serial.printf("READ: %s -> %f\n", data[i]->getName().c_str(),
  //                                 storage->getData( dataCollectors[i]->getName()));
  // }

  Serial.println("Go Sleep ***************************");
  Serial.println("");
  delay(1000);
}
