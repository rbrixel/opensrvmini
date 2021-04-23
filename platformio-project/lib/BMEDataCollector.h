#include "IDataCollector.h"

class BMEDataCollector : public IDataCollector {
   public:

        void setDataStorage(IDataStorage *storage);
        void updateData();

   protected:

        int readBMEData();      
};