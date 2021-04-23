
#include "IDataStorage.h"


class DataStorage : public IDataStorage {
   public:
        void storeDataDouble(double data);
        void storeDataInt(int data);
        double getDataDouble();
        int getDataInt();

   protected:

};