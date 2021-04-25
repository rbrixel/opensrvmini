#ifndef IDATACOLLECTOR_H
#define IDATACOLLECTOR_H

#include <IDataStorage.h>

class IDataCollector {
   public:
      // pure virtual function providing interface framework.
      virtual void setDataStorage(IDataStorage *storage)=0;
      virtual void updateData()=0;
      virtual std::string getName()=0;

   protected:
      IDataStorage *_dataStorage;

};

#endif