#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <IDataStorage.h>
#include <map>

class DataStorage : public IDataStorage {
   public:
        void addData(std::string channelName, double data);
        double getData(std::string channelName) ;
        std::map<std::string, double> getMapCopy() ;
   protected:
      std::map<std::string, double> _map;
};

#endif