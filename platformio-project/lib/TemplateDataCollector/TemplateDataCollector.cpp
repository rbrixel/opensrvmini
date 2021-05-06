/*
 * TemplateDataCollector.cpp
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#include <TemplateDataCollector.h>

#define CHANNELEXTFAXE ".FAKE"

///
/// Constructing with a channelName
TemplateDataCollector::TemplateDataCollector(std::string channelName)
{
    _channelName = channelName;
    _config1=1000; // Default Value configured here
}

TemplateDataCollector::TemplateDataCollector(std::string channelName, byte config1)
{
    _channelName = channelName;
    _config1 = config1; // Use customized given config
}

///
/// Initializes the Component and its DataStorage
void TemplateDataCollector::init(IDataStorage *storage)
{
    // Hold a link to the datastorage. But update only in update() call.
    _dataStorage = storage;

    
}

///
/// Initializes the component again
/// Usually the same as init, but maybe additional cleanup is needed
void TemplateDataCollector::reInit()
{
    _needsReInit=false;
}

///
/// Tells overlaying process if this component should be reinitialized
bool TemplateDataCollector::needsReInit()
{
    return _needsReInit;
}

/// 
/// Updates Data into DataStorage
void TemplateDataCollector::updateData()
{
        #ifdef OPENSRVDEBUG
            Serial.println("DEBUG CODE ACTIVE! RANDOM DATA");
            long randomVal = random(0,_config1*10);
            double result = randomVal/10;
            if (result > 900)
            {
                _needsReInit=true; // Create some fake reinit requests
            }
            _dataStorage->addData(_channelName + CHANNELEXTFAXE , result);
        #else
            _dataStorage->addData(_channelName + CHANNELEXTFAXE , 1.0f);
        #endif
}

///
/// Returns this channelsName
std::string TemplateDataCollector::getName(){
    return _channelName;
}

///
/// fake at the moment, set up sleep mode
void  TemplateDataCollector::sleep() {
    ;
}
