/*
 * DisplayDataActor.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 * requires
 * lib_deps = 
 *	fmalpartida/LiquidCrystal@^1.5.0
 */
#ifndef DISPLAYDATAACTOR_H
#define DISPLAYDATAACTOR_H

#include <IDataActor.h>
#include <LiquidCrystal.h>

///
/// DisplayDataActor is configured to observe a channel of a dataStorage
/// if the channel value is between lowerbound and upperbound (including the values itself)
/// the gpio pin will change its output
/// the output  is defined by inDisplayIsOn, 
class DisplayDataActor : public IDataActor {
       public:
              DisplayDataActor(std::string observedChannel);
              DisplayDataActor(std::string observedChannel, uint8_t rs,  uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
              void init();
              void reInit();
              void sleep();
              void action(IDataStorage *dataStorage);
              void setSpeedCallback(void (*spcb)(int s));
              std::string getName();
              LiquidCrystal *_lcd;
       protected:
              std::string _observedChannel;

              uint8_t _rs     = GPIO_NUM_25;
              uint8_t _enable = GPIO_NUM_26;
              uint8_t _d0     = GPIO_NUM_0;
              uint8_t _d1     = GPIO_NUM_4;
              uint8_t _d2     = GPIO_NUM_16;
              uint8_t _d3     = GPIO_NUM_17;

              double extractChannelValue(IDataStorage *dataStorage);
};

#endif