#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <IDataStorage.h>
#include <map>

class DataStorage : public IDataStorage {
   public:
        void storeDataDouble(std::string channelName, double data);
        void storeDataInt(std::string channelName,int data);
        double getDataDouble(std::string channelName) ;
        int getDataInt(std::string channelName) ;
   protected:
      std::map<std::string, double> _map;
};

#endif