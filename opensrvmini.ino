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
 * Author:  René Brixel <mail@campingtech.de>
 * Date:    2020-09-14 (initial Sketch)
 * VErsion: 20201210-1
 * Web:     https://campingtech.de/opensrv
 * GitHub:  https://github.com/rbrixel
 */
 
/*
 * General setup
 */
String osrvmVersion = "0.1"; // OpenSRVmini version

/*
 * BME280-setup
 */
#include <cactus_io_BME280_I2C.h>
BME280_I2C bme(0x76); // uint i2c-address

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
  if (!bme.begin()) {
    Serial.println("Could not find BME280 sensor, check wiring");
    while (1);
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
  bme.readSensor();
  float bTemp(NAN), bHum(NAN), bPres(NAN);
  bTemp = bme.getTemperature_C();
  bHum = bme.getHumidity();
  bPres = bme.getPressure_MB();
  Serial.println(String(bTemp) + " °C");
  Serial.println(String(bHum) + " %");
  Serial.println(String(bPres) + " mb");

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
