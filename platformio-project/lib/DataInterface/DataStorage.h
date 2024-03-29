/*
 * DataStorage.h
 *
 *  Created on: April 2021
 *      Author: Frank Weichert
 */
#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <IDataStorage.h>
#include <map>

///
/// DataStorage implementation of IDataStorage
/// Stores the last with addData submitted value into 
/// the Channel with the channelName
class DataStorage : public IDataStorage {
   public:
      void addData(std::string channelName, float data);

      /// 
      /// Retrieved the last stored value of a channel
      /// requesting an unknown channelName will result in -99999.0
      /// use isChannelExistant(std::string channelName) to query if the requested channel exist
      float getData(std::string channelName) ;

      ///
      /// True if the given channel exists in storage and can be queried
      /// otherwise false
      bool isChannelExistant(std::string channelName);

      ///
      /// Returns a copy of the internal DataStorage Map
      std::map<std::string, float> getMapCopy();
   protected:
      std::map<std::string, float> _map;
};

#endif