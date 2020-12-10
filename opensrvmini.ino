/*
 * OpenSRVmini
 * -----------
 * 
 * EN: Description
 * DE: Beschreibung
 * 
 * Hardware: 
 * - ESP32 Dev Kit
 * - BME280 (temperature, humidity and air pressure)
 * - MPU-6050 (3-axis-gyroscope)
 * - DS18B20 (waterproof for outdoor measurement)
 * - ADS1115 (4-Channel, 16-Bit AD, i2c)
 * 
 * Software:
 * - Over-The-Air-Update (ArduinoOTA)
 * - OneWire (OneWire - use 2.3.0)
 * - Cactus.io BME280 (http://cactus.io/projects/weather/arduino-weather-station-bme280-sensor)
 * 
 * Author: René Brixel <mail@campingtech.de>
 * Date: 2020-09-14
 * Web: https://campingtech.de/opensrv
 * GitHub: https://github.com/rbrixel
 */

/*
 * Kurzinfos:
 * WLAN wird nur für Over-The-Air-Update benötigt
 * Bluetooth für Datenkommunikation mit Smartphone
 */

/*
 * General setup
 */
String osrvmVersion = "0.1"; // OpenSRVmini version

/*
 * BME280-setup
 */
/*
#include <cactus_io_BME280_I2C.h>
BME280_I2C bme(0x76); // uint i2c-address
*/

/*
 * DS18B20-setup
 */
/*
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5 // GPIO5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18sensors(&oneWire);
*/

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
//#define VCCINPORT 32
#include "ADS1X15.h"
ADS1115 ADS(0x48);


void setup() {
  Serial.begin(115200);

  // ADS1115 init
  ADS.begin();
}

void loop() {
  Serial.println(String(MeasureADS(0)) + "V");
  Serial.println();

  delay(1000);
}

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
