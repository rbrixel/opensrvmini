#ifndef BMEDATACOLLECTOR_H
#define BMEDATACOLLECTOR_H


#include <IDataCollector.h>
#include <random>

///
/// BMEDataCollector implementing IDataCollector
/// used to collect temperature and pressure from a BME280 sensor
class BMEDataCollector : public IDataCollector {
       public:
              BMEDataCollector(std::string channelName);
              void init(IDataStorage *storage);
              void reInit();
              void sleep();
              void updateData();
              std::string getName();
       protected:
              std::string _channelName = "BMECollector";
              void readBMEData();     
              double _temp = 0;
              double _pressure = 0;    
};

#endif