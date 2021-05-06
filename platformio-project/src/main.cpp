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



// Sample Usage of Interfaces for Data Collection
std::vector<IDataCollector *> dataCollectors;
std::vector<IDataActor *> dataActors;

IDataStorage *storage = new DataStorage();

void setup()
{
  Serial.begin(115200);

  /* *******************************************************************************
      Create Datacollectors 
  */
  dataCollectors.push_back(new BMEDataCollector("Channel-BME"));  // Get Temperature and Pressure
  dataCollectors.push_back(new ADSDataCollector("Channel-ADS"));  // get Board Voltage
  dataCollectors.push_back(new DTSDataCollector("Channel-DTS"));  // get outer temperature

  /* *******************************************************************************
      Create DataActors 
  */
  dataActors.push_back(new RangeDataActor("Channel-ADS.VCC",11.5f,15.0f,GPIO_NUM_18,true )); // Raise PIN 18 to HIGH as long as board voltage is good
  dataActors.push_back(new RangeDataActor("Channel-ADS.VCC", 0.0f,11.5f,GPIO_NUM_19,true )); // Raise PIN 19 as alarm to HIGH as voltage drops below 11.5V
  dataActors.push_back(new BTDataActor()); 

  Serial.printf("INIT: %d Collecotrs found\n", dataCollectors.size());
  Serial.printf("INIT: %d Actors found\n", dataActors.size());

  /* *******************************************************************************
      Init DataCollectors
  */
  // Initialize Collectors (with storage and maybe more)
  for (std::size_t i = 0; i < dataCollectors.size(); ++i)
  {
    Serial.printf("INIT: %s\n", dataCollectors[i]->getName().c_str());
    dataCollectors[i]->init(storage);
  }

  /* *******************************************************************************
      Init Actors
  */
  for (std::size_t i = 0; i < dataActors.size(); ++i)
  {
    Serial.printf("INIT Actor\n");
    dataActors[i]->init();
  }

}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

void loop()
{
  /* *******************************************************************************
      Check Initialization of DataCollectors
  */
  Serial.println("************************************");
  for (std::size_t i = 0; i < dataCollectors.size(); ++i)
  {
    if (dataCollectors[i]->needsReInit())
    {
      dataCollectors[i]->reInit();
      Serial.printf("Reinitialized: %s\n", dataCollectors[i]->getName().c_str());
    };
  }


  /* *******************************************************************************
      Update DataCollectors
  */
  Serial.println("************************************");
  for (std::size_t i = 0; i < dataCollectors.size(); ++i)
  {
    dataCollectors[i]->updateData();
    Serial.printf("UPDATE: %s\n", dataCollectors[i]->getName().c_str());
  }

  Serial.println("************************************");

  /* *******************************************************************************
      Update Actors
  */
  Serial.println("************************************");
  for (std::size_t i = 0; i < dataActors.size(); ++i)
  {
    dataActors[i]->action(storage);
  }

  Serial.println("************************************");

  /* *******************************************************************************
      Use collected Data
  */
  // Should use Actors do do something with data
  // std::map<std::string, double> data = storage->getMapCopy();
  // std::map<std::string, double>::iterator it;

  // for (it = data.begin(); it != data.end(); it++)
  // {
  //   Serial.printf("READ: %s -> %f\n",
  //                 it->first.c_str(), // Print ChannelName
  //                 it->second);       // Print Channel Value
  // }
  
  Serial.println("************************************");
  Serial.println("Can Sleep now for a while **********");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  delay(5000);
}
