# OpenSRVmini

![OpenSRVmini Logo](/OpenSRVmini-Logo.jpg)

#WIP - Work in progress

## Description/Beschreibung

**(EN)** With OpenSRVmini you can measure the following in your caravan / motor home: temperature inside and outside, air humidity inside, 12V battery and the location using a gyro sensor.
The circuit communicates with a smartphone app via Bluetooth. A new firmware can be flashed via wifi via OTA update.

**(DE)** Mit OpenSRVmini kannst du folgendes in deinem Wohnwagen/Wohnmobil messen: Temperatur innen und außen, Lufgfeuchtigkeit innen, 12V Akku und die Lage anhand einem Kreiselsensor.
Die Schaltung kommuniziert über Bluetooth mit einer Smartphone-App. Über WLAN kann via OTA-Update eine neue Firmware geflashed werden.

## Hardware

* ESP32 "Dev Kit" (with wifi and bluetooth)
* BME280 (temperature, humidity and air pressure for indoor)
* MPU-6050 (3-axis-gyroscope)
* DS18B20 (waterproof for outdoor measurement)
* ADS1115 (4-Channel, 16-Bit AD, i2c)

**(EN)** I use the ADS1115 to measure the current because the EPS32's internal ADC is not accurate enough.

**(DE)** Ich verwende zur Messung des Stroms den ADS1115, da der interne ADC des EPS32 nicht genau genug ist.

## Software

Arduino-IDE with ESP32-Board-Addon.
