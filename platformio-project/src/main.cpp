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
 * Libs: From PlatformIO INI 
 * - ks-tec/BME280_I2C @ ^1.4.1
 * - paulstoffregen/OneWire @ ^2.3.5
 * - milesburton/DallasTemperature @ ^3.9.1
 * - robtillaart/ADS1X15 @ ^0.3.0
 * - adafruit/Adafruit MPU6050 @ ^2.0.4
 * 
 * Author:  René Brixel <mail@campingtech.de>
 * Support: Frank Weichert <opensrvmini@humanbyte.de> (@weichei)
 * Date:    2021-04-22 (not so, but really initial Sketch)
 * Version: 20210422-1
 * Web:     https://campingtech.de/opensrv
 * GitHub:  https://github.com/rbrixel
 * 
 */

#include <main.h>


// Sample Usage of Interfaces for Data Collection
std::vector<IDataCollector *> dataCollectors;
std::vector<IDataActor *> dataActors;

IDataStorage *storage = new DataStorage();
volatile int speed=250;
void setSpeed(int s)
{
  Serial.printf("Set Speed to %d",s);
  speed=s;
}

void setup()
{
  Serial.begin(115200);

  /* *******************************************************************************
      Create Datacollectors 
  */
  dataCollectors.push_back(new BMEDataCollector("Channel-BME"));  // Get Temperature and Pressure / Uses Wire for communication
  dataCollectors.push_back(new ADSDataCollector("Channel-ADS", &Wire));  // get Board Voltage / Uses the Wire for communication. Wire is setup by BME Internal controller allready
  dataCollectors.push_back(new DTSDataCollector("Channel-DTS"));  // get outer temperature
  dataCollectors.push_back(new MPUDataCollector("Channel-MPU", GPIO_NUM_19, GPIO_NUM_18));  // get outer gyro acc / Setup to I2C to 18 and 19 to ensure asnyc polling is not disturbing BME I2C

  /* *******************************************************************************
      Create DataActors 
  */
  dataActors.push_back(new RangeDataActor("Channel-ADS.VCC",11.5f,15.0f,GPIO_NUM_10,true )); // Raise PIN 10 to HIGH as long as board voltage is good
  dataActors.push_back(new RangeDataActor("Channel-ADS.VCC", 0.0f,11.5f,GPIO_NUM_11,true )); // Raise PIN 11 as alarm to HIGH as voltage drops below 11.5V
  dataActors.push_back(new BTDataActor("OpenSRVmini")); 
  dataActors.push_back(new DisplayDataActor("Channel-MPU.ACAX")); 

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
    dataActors[i]->setSpeedCallback(&setSpeed);
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
    Serial.printf("UPDATING: %s\n", dataCollectors[i]->getName().c_str());
    dataCollectors[i]->updateData();
    Serial.printf("UPDATED: %s\n", dataCollectors[i]->getName().c_str());
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
  delay(speed);
}
