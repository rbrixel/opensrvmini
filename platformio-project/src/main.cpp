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
 * Libs:
 * - Over-The-Air-Update (ArduinoOTA)
 * - OneWire (OneWire - use 2.3.0)
 * - Cactus.io BME280 (http://cactus.io/projects/weather/arduino-weather-station-bme280-sensor)
 * 
 * Author:        René Brixel <mail@campingtech.de>
 * Contributors:  dalander
 * Date:          2020-09-14 (initial Sketch)
 * Version:       20201210-1
 * Web:           https://campingtech.de/opensrv
 * GitHub:        https://github.com/rbrixel/opensrvmini
 */

/*
 * Header files
 */
#include <main.h>

/*
 * General setup
 */
String osrvmVersion = "0.1"; // OpenSRVmini version

/*
 *  Ringbuffer to hold Data
 */
RingBuffer *dataholder = new RingBuffer(20,"Buffer");

/*
 * BME280-setup
 */
#include <BME280_I2C.h>
BME280_I2C bme280; //BME280_I2C bme(0x76); // uint i2c-address

#define BME280_I2C_SCL    26
#define BME280_I2C_SDA    25

/*
 * DS18B20-setup
 */
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5 // GPIO5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18sensors(&oneWire);

/*
 * MPU-6050-setup
 */
/*
#include <Wire.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
*/

/*
 * Voltagedivider-setup
 */
#include "ADS1X15.h"
ADS1115 ADS(0x48);

/*
 * ## SETUP-Routine ##
 */
void setup() {
  Serial.begin(115200);

  // ADS1115 init
  ADS.begin();

  // DS18B20 init
  ds18sensors.begin();

  // BME280 init

    bme280.setAddress(BME280_ADDRESS, BME280_I2C_SDA, BME280_I2C_SCL);
    bool isStatus = bme280.begin(
      bme280.BME280_STANDBY_0_5,
      bme280.BME280_FILTER_16,
      bme280.BME280_SPI3_DISABLE,
      bme280.BME280_OVERSAMPLING_2,
      bme280.BME280_OVERSAMPLING_16,
      bme280.BME280_OVERSAMPLING_1,
      bme280.BME280_MODE_NORMAL);

    if (!isStatus) {
      Serial.println("can NOT initialize for using BME280.\n");
      while(true);
    } else {
      Serial.println("ready to using BME280.\n");
    }
}

/*
 * ## LOOP ##
 */
void loop() {

  Serial.println("-- DB18B20 --");
  ds18sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC = ds18sensors.getTempCByIndex(0); // read first sensor
  if(tempC != DEVICE_DISCONNECTED_C) {
    Serial.println(String(tempC) + " °C");
  } else {
    Serial.println("Error: Could not read temperature data");
  }

  Serial.println("-- BME280 --");
  bme280.read();

  char temp_c[12], humi_c[12], pres_c[12], altd_c[12];
  dataholder->addElement(bme280.data.temperature);
  ESP_LOGI(TAG , "- Temperature History ---------------------------" );
  dataholder->debugBuffer(dataholder->getContentHistory());
  ESP_LOGI(TAG , "- -----------------------------------------------" );

  sprintf(temp_c, "%2.2lf", bme280.data.temperature);
  sprintf(humi_c, "%2.2lf", bme280.data.humidity);
  sprintf(pres_c, "%4.2lf", bme280.data.pressure);
  sprintf(altd_c, "%4.2lf", bme280.data.altitude);
  Serial.println("-----------------------");
  Serial.print("Temperature: "); Serial.print(temp_c); Serial.println(" ℃");
  Serial.print("Humidity: ");    Serial.print(humi_c); Serial.println(" %");
  Serial.print("Pressure: ");    Serial.print(pres_c); Serial.println(" hPa");
  Serial.print("Altitude: ");    Serial.print(altd_c); Serial.println(" m");
  Serial.println("-----------------------");

  Serial.println("-- ADS1115 --");
  Serial.println(String(MeasureADS(0)) + " V"); // Read the voltage of BAT0

  Serial.println("---------");
  delay(2000);
}

/*
 * Function: Measure voltage and return in "human readable"
 */
float MeasureADS (int analog_input_pin) {
  float reference_vcc = 3.3; // Volt... ESP32 system voltage
  float vcc_max = 16.0; // Volt... maximum input on voltage divider
  int resistor_to_gnd = 12000; // Ohm... Voltage-divider: Resistor to GND
  int resistor_to_gnd_max = 12210; // Ohm... calculated maximum to near reference_vcc
  // With 47k to VCC and 12k to GND, I get 3.254V on voltage divider output
  // With a 12k210 resistor to GND, I get 3.299V
  // So use the two values for calculating calibration

  ADS.setGain(0);
  float vout = ADS.readADC(analog_input_pin) * ADS.toVoltage(1);

  float voutRaw = vout / (reference_vcc / vcc_max);
  float rFactor = (float)resistor_to_gnd_max / (float)resistor_to_gnd; // It is important to cast a float!!

  return (voutRaw * rFactor);
}