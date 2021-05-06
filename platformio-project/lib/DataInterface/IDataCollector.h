/*
 * IDataCollector.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef IDATACOLLECTOR_H
#define IDATACOLLECTOR_H

#include <IDataStorage.h>

///
/// IDataCollector Interface for Sensors collecting Data
/// init called once after instance creation to provide datastorage
/// updateData() called multiple times after init to let the sensor update its values in DataStorage
class IDataCollector {
   public:
      ///
      /// Called once after Instace creation
      /// Used to setup BUS protocoll etc used for communication
      virtual void init(IDataStorage *storage)=0;

      /// 
      /// Can be called multiple times after init
      /// used to reinitialize used protocolls in case of an overlaying event (wake up)
      /// but keeps the allready defined dataStorage
      /// called after sleep
      virtual void reInit()=0;

      ///
      /// Override this function 
      /// to tell the main loop that a reinit call is needed.
      /// Be sure to clean your data before.
      virtual bool needsReInit(){ return false; };

      ///
      /// called after init or reinit, not called after sleep
      /// Reads data from the Implemented Sensor and stores it to the DataStorage
      /// If multiple Values needs to be stored into one DataStorage use Channelformat name
      /// CHANNELNAME.VALUETYPE like BMECHANNEL.TEMP and BMECHANNEL.PRESSURE
      /// Can be called after init or reinit, calls after sleep() are not defined and may produce invalid data
      virtual void updateData()=0;

      ///
      /// Resolves the name of the Implemented Sensorchannel
      /// used as the first part of the DataStorage Channel (f.e. BMECHANNEL in BMECHANNEL.TEMP )
      virtual std::string getName()=0;

      ///
      /// informal call that the System wants to save current,
      /// used by implemenation to shutdown not used parts (timers, interrupts etc)
      /// no update calls should be done after sleep has been called.
      /// Wakeup is triggered by reinit
      virtual void sleep()=0;

   protected:
      IDataStorage *_dataStorage;

};

#endif