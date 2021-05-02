#ifndef IDATAACTOR_H
#define IDATAACTOR_H

#include <IDataStorage.h>

///
/// IDataCollector Interface for Sensors collecting Data
/// init called once after instance creation to provide datastorage
/// updateData() called multiple times after init to let the sensor update its values in DataStorage
class IDataActor {
   public:
      ///
      /// Called once after Instace creation
      /// Used to setup BUS protocoll etc used for communication
      virtual void init()=0;

      /// 
      /// Can be called multiple times after init
      /// used to reinitialize used protocolls in case of an overlaying event (wake up)
      /// but keeps the allready defined dataStorage
      /// called after sleep
      virtual void reInit()=0;

      ///
      /// called after init or reinit, not called after sleep
      /// Creates an action based on given Data in dataStorage
      /// Need some info to know what data channel should be used
      virtual void action(IDataStorage *_dataStorage)=0;

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