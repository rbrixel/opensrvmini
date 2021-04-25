#ifndef IDATASTORAGE_H
#define IDATASTORAGE_H

#include <map>

class IDataStorage {
   public:
            virtual void storeDataDouble(std::string channelName, double data)=0;
            virtual void storeDataInt(std::string channelName, int data)=0;
            virtual double getDataDouble(std::string channelName)=0;
            virtual int getDataInt(std::string channelName)=0;
   protected:
};

#endif