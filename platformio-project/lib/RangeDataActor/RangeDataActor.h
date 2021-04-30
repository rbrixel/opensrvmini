#ifndef RANGEDATAACTOR_H
#define RANGEDATAACTOR_H

#include <IDataActor.h>

///
/// BMEDataCollector implementing IDataCollector
/// used to collect temperature and pressure from a BME280 sensor
class RangeDataActor : public IDataActor {
       public:
              RangeDataActor(std::string observedChannel,double lowerBound, double upperBound,uint8_t outputPin, bool inRangeIsOn );
              void init();
              void reInit();
              void sleep();
              void action(IDataStorage *dataStorage);
              std::string getName();
       protected:
              std::string _observedChannel;
              double _lowerBound = 0.0f;
              double _upperBound= 999999999.0f; 
              bool _inRangeIsOn=true;
              uint8_t _outPutPin = 0;
};

#endif