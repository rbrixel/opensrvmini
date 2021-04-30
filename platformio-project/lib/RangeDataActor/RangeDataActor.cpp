#include <RangeDataActor.h>
#include <Arduino.h>
#define CHANNELEXTTEMP ".TEMP"
#define CHANNELEXTPRESSURE ".PRES"

///
/// Instanciation of Range Actor
/// observedChannel is the channel in Datasotrage of action() method which will be searched for
/// lowerbound and upperbound defines the range where an action will occur
/// outPutPin will define the pin which is switched
/// inRangeIsOn = true: if observed value is in range, output pin will be true
RangeDataActor::RangeDataActor(std::string observedChannel,double lowerBound, double upperBound, uint8_t outputPin ,bool inRangeIsOn)
{
              _observedChannel = observedChannel;
              _lowerBound = lowerBound;
              _upperBound = upperBound; 
              _inRangeIsOn  = inRangeIsOn;
              _outPutPin = outputPin;
}

///
/// Initializes the Component and its DataStorage
void RangeDataActor::init()
{
    ;
}

///
/// Initializes the component
void RangeDataActor::reInit()
{
    ;
}

/// 
/// Updates BME Data into DataStorage
void RangeDataActor::action(IDataStorage *dataStorage)
{
    // TODO LOGIC HERE
}

///
/// fake at the moment, set up sleep mode
void  RangeDataActor::sleep() {
    ;
}
