
class IDataStorage {
   public:
            virtual void storeDataDouble(double data);
            virtual void storeDataInt(int data);
            double getDataDouble();
            int getDataInt();

   protected:

};