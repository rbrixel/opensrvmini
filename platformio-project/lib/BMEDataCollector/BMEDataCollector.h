#ifndef BMEDATACOLLECTOR_H
#define BMEDATACOLLECTOR_H


#include <IDataCollector.h>
#include <random>

class BMEDataCollector : public IDataCollector {
       public:
              BMEDataCollector(std::string channelName);
              void setDataStorage(IDataStorage *storage);
              void updateData();
              std::string getName();
       protected:
              std::string _channelName = "BMECollector";
              double readBMEData();      
};

#endif