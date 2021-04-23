#include "BMEDataCollector.h"


class BMEDataCollector : public IDataCollector {
   public:

      void setDataStorage(IDataStorage *storage)
      {
          _dataStorage = storage;
      }

      void updateData()
      {
          double value = readBMEData();
          _dataStorage->storeDataDouble(value);
      }
   
   protected:
     
    int readBMEData() {
        return 451;
    }
};