/*
 * IDataStorage.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */

#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <map>

#define UNKNOWNRESULT -99999.0f

///
/// IDataStorage to store sensorData in channels
/// Every channel is defined by its name
/// Future Idea: extend to store multiple values in a ringbuffer
class IDataStorage {
   public:
      ///
      /// stores a double Value (data) to the channel channelName
      virtual void addData(std::string channelName, double data)=0;

      ///
      /// retrieves a double from the specified channel
      virtual double getData(std::string channelName)=0;

      ///
      /// True if the given channel exists in storage and can be queried
      /// otherwise false
      virtual bool isChannelExistant(std::string channelName);

      virtual  std::map<std::string, double> getMapCopy() =0;
   protected:

};

#endif