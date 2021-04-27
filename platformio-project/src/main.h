#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <BMEDataCollector.h>
#include <DTSDataCollector.h>
#include <ADSDataCollector.h>
#include <DataStorage.h>

void initOTA();

#endif
