/*
 * TemplateDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef TEMPLATEDATACOLLECTOR_H
#define TEMPLATEDATACOLLECTOR_H

#include <IDataCollector.h>
#include <Arduino.h>
#include <random>

// Comment this line to acitivate Real Sensors
#define OPENSRVDEBUG 

///
/// TemplateDataCollector implementing IDataCollector
/// Take a copy of this to start implementing your own collector
class TemplateDataCollector : public IDataCollector {
       public:
              TemplateDataCollector(std::string channelName);
              TemplateDataCollector(std::string channelName, byte config1);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              bool needsReInit();
              std::string getName();
       protected:
              byte _config1 = 0;
              bool _needsReInit;
              std::string _channelName = "TemplateCollector";
              
};

#endif