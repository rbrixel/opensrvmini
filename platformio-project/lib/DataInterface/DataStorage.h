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
        void addData(std::string channelName, double data);
        double getData(std::string channelName) ;

        ///
        /// Returns a copy of the internal DataStorage Map
        std::map<std::string, double> getMapCopy() ;
   protected:
      std::map<std::string, double> _map;
};

#endif