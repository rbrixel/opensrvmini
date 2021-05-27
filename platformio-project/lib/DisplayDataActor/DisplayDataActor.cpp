/*
 * DisplayDataActor.cpp
 *
 *  Created on: May 2021
 *      Author: Frank Weichert
 * Default connectins for a Devkit 4
 * Display pinname | Display pin | ESP GPIO pin
 * rs              |   4         | GPIO_NUM_25
 * enable          |   6         | GPIO_NUM_26
 * d0              |  11         | GPIO_NUM_0
 * d1              |  12         | GPIO_NUM_4
 * d2              |  13         | GPIO_NUM_16
 * d3              |  14         | GPIO_NUM_17
 */
#include <DisplayDataActor.h>
#include <Arduino.h>

///
/// Instanciation of Display Actor
/// A default 16/2 display is used, first line contains the observed channel, second line the value.
/// observedChannel is the channel in Datasotrage, this will be displayed on a 16,2 display in action() method 
DisplayDataActor::DisplayDataActor(std::string observedChannel)
        : _observedChannel(observedChannel)
{
    _lcd = new LiquidCrystal(_rs, _enable, _d0, _d1, _d2, _d3);
}

///
/// Instanciation of Display Actor
/// A default 16/2 display is used, first line contains the observed channel, second line the value.
/// observedChannel is the channel in Datasotrage, this will be displayed on a 16,2 display in action() method 
///  using parameters rs,  enable, d0, d1, d2, d3 it is possible to erconfigure the wiring
DisplayDataActor::DisplayDataActor(std::string observedChannel, uint8_t rs,  uint8_t enable,
                             uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
        : _observedChannel(observedChannel), _rs(rs),_enable(enable),_d0(d0),_d1(d1),_d2(d2),_d3(d3)
{
    _lcd = new LiquidCrystal(_rs, _enable, _d0, _d1, _d2, _d3);
}

///
/// Initializes the Component and its DataStorage
void DisplayDataActor::init()
{
    _lcd->begin(16,2);
    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->printf("%.16s",_observedChannel.c_str());
}

void DisplayDataActor::setSpeedCallback(void (*spcb)(int s))
{
  ;
}

///
/// Initializes the component
void DisplayDataActor::reInit()
{
    ;
}

/// 
/// Updates BME Data into DataStorage
void DisplayDataActor::action(IDataStorage *dataStorage)
{
    if (!dataStorage->isChannelExistant(_observedChannel))
    {
        // No Data; No Action
        //Serial.printf("No Action \n");
        return;
    }
    _lcd->setCursor(0,1);
    _lcd->printf("%.2f",dataStorage->getData(_observedChannel));
}

///
/// fake at the moment, set up sleep mode
void  DisplayDataActor::sleep() {
    ; // Maybe Default Output if sleeping?
}
