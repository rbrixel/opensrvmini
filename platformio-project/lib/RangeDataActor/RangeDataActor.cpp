/*
 * RangeDataActor.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <RangeDataActor.h>
#include <Arduino.h>

///
/// Instanciation of Range Actor
/// observedChannel is the channel in Datasotrage of action() method which will be searched for
/// lowerbound and upperbound defines the range where an action will occur
/// outPutPin will define the pin which is switched
/// inRangeIsOn = true: if observed value is in range, output pin will be true
RangeDataActor::RangeDataActor(std::string observedChannel,float lowerBound, float upperBound, uint8_t outputPin ,bool inRangeIsOn)
{
              _observedChannel = observedChannel;
              _lowerBound = lowerBound;
              _upperBound = upperBound; 
              _inRangeIsOn  = inRangeIsOn;
              _gpio = outputPin;
              pinMode(_gpio,OUTPUT);
}

///
/// Initializes the Component and its DataStorage
void RangeDataActor::init()
{
    ;
}

void RangeDataActor::setSpeedCallback(void (*spcb)(int s))
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
    //log_w("Action on [%s]", _observedChannel.c_str());
    // TODO LOGIC HERE
    if (!dataStorage->isChannelExistant(_observedChannel))
    {
        // No Data; No Action
        //log_w("No Action ");
        return;
    }

    float value = dataStorage->getData(_observedChannel);
    if ( isInRange(value) )
    {
        log_w("Action on %s for value %f -> GPIO %d=%s",
            _observedChannel.c_str() ,
            value, 
            _gpio ,
            _inRangeIsOn ? "HIGH" : "LOW");
        digitalWrite( _gpio, _inRangeIsOn );
    }else{
        log_w("Action on %s for value %f -> %d=%s",
            _observedChannel.c_str() ,
            value,
            _gpio ,
            !_inRangeIsOn ? "HIGH" : "LOW");
        digitalWrite( _gpio, !_inRangeIsOn );
    }
}

/// 
/// Updates BME Data into DataStorage
bool RangeDataActor::isInRange(float value)
{
    return (value >= _lowerBound && value <=_upperBound);
}

///
/// fake at the moment, set up sleep mode
void  RangeDataActor::sleep() {
    ; // Maybe Default Output if sleeping?
}
