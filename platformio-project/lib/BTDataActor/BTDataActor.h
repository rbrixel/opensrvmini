#ifndef BTDATAACTOR_H
#define BTDATAACTOR_H

#include <IDataActor.h>
#include "BluetoothSerial.h"

///
/// BTDataActor is configured to observe a datastorage and sends out all Daat via Bluetooth
class BTDataActor : public IDataActor {
       public:
              BTDataActor( );
              void init();
              void reInit();
              void sleep();
              void action(IDataStorage *dataStorage);
       protected:
               BluetoothSerial _serialBT;
              std::string _observedChannel;
              bool isInRange(double value);
              double extractChannelValue(IDataStorage *dataStorage);
};

#endif