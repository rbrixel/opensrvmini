/*
  Smoother.h - Library for smoothing inputs
  Frank Weichert
*/
#ifndef Smoother_h
#define Smoother_h

#include <Arduino.h>

class Smoother
{
  public:
    Smoother(int arraySize);
    void cleanUp();
    void pushValue(int16_t value);
    int16_t getSmoothed();
    void setSmoothingActive(bool active);
  private:
    int _arraySize;
    int16_t *_values;
    int16_t *_sortedValues;
    uint16_t _insertIndex;
    bool _smoothingActive;
    bool _isFirstValue;
    void incrementInsertIndex();
    void debugArray(char*tag , int16_t *arr);
    static int qsortComparer(const void *cmp1, const void *cmp2);
};

#endif
