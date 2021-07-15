/*
  Smoother.h - Library for smoothing inputs
  Frank Weichert
*/
#ifndef Smoother_h
#define Smoother_h

#include <Arduino.h>

/** \class
 * Smoother.h - Library for smoothing inputs
 * stores all inputs in a buffer with a given size at construction.
 * to get a smoothed value the content will be sortet, min and max values will be removed
 * and an average of the remaining items is calculated
 * 
 * Frank Weichert
 */
class Smoother
{
  public:
    Smoother(int arraySize);
    void cleanUp();
    void pushValue(int16_t value);
    void setSmoothingActive(bool active);
    int16_t getSmoothed();
  private:
    int   _arraySize;
    int16_t *_values;
    int16_t *_sortedValues;
    uint16_t _insertIndex;
    bool  _smoothingActive;
    bool  _isFirstValue;
    void  incrementInsertIndex();
    void  debugArray(char*tag , int16_t *arr);
    static int   qsortComparer(const void *cmp1, const void *cmp2);
};

#endif
