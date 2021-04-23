
#include "IDataStorage.h"

class IDataCollector {
   public:
      // pure virtual function providing interface framework.
      virtual void setDataStorage(IDataStorage *storage);
      virtual void updateData();
   
   protected:
      IDataStorage *_dataStorage;

};