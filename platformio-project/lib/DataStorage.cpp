
#include "DataStorage.h"


class DataStorage : public IDataStorage {
   public:
        void storeDataDouble(double data)
        {
            _data = data;
        }

        void storeDataInt(int data)
        {
            _data = data;
        }

        double getDataDouble()
        {
            return _data;
        }

        int getDataInt()
        {
            return _data;
        }

   protected:
        double _data=0;

};