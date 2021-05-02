#ifndef RANGEDATAACTOR_H
#define RANGEDATAACTOR_H

#include <IDataActor.h>

///
/// RangeDataActor is configured to observe a channel of a dataStorage
/// if the channel value is between lowerbound and upperbound (including the values itself)
/// the gpio pin will change its output
/// the output  is defined by inRangeIsOn, 
/// if inRangeIsOn==true, the output is HIGH if the channel is in the range and LOW if channel value is out of range
/// if inRangeIsOn==false, the output is LOW if the channel is in the range and HIGH if channel value is out of range
class RangeDataActor : public IDataActor {
       public:
              RangeDataActor(std::string observedChannel,double lowerBound, double upperBound,uint8_t gpio, bool inRangeIsOn );
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
              uint8_t _gpio = 0;
              bool isInRange(double value);
              double extractChannelValue(IDataStorage *dataStorage);
};

#endif