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
              std::string getName();
       protected:
              byte _config1 = 0;
              std::string _channelName = "TemplateCollector";
              
};

#endif