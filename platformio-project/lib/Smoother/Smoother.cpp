/*
 * Smoother.h - Library for smoothing inputs
 * stores all inputs in a buffer with a given size at construction.
 * to get a smoothed value the content will be sortet, min and max values will be removed
 * and an average of the remaining items is calculated
 * 
 * Frank Weichert
*/
#include <Smoother.h>

/*
 * New Instance of a smoother object
 * @param arraySize the site of the internal array to build up. Will affect the latency of the smoothing effect
*/
Smoother::Smoother(int arraySize)
{
  _arraySize = arraySize;
  _values = new int16_t[_arraySize];
  _sortedValues = new int16_t[_arraySize];
  cleanUp();
}

/*
 * Cleans all internal arrays and values
*/
void Smoother::cleanUp()
{
  memset(_values,0,(sizeof *_values)*_arraySize);
  memset(_sortedValues,0,(sizeof *_values)*_arraySize);
  _insertIndex=0;
  _isFirstValue=true;
  _smoothingActive=true;
}

/*
 * Counts up the internal index (use a ringbuffer)
*/
void Smoother::incrementInsertIndex()
{
  _insertIndex++;
  if (_insertIndex>_arraySize)
  {
    _insertIndex=0;
  }
}

/*
 * Inserts a value at the current position in the internal array
 * @param value the value to be stored
*/
void Smoother::pushValue(int16_t value)
{
   /*
  Serial.print("Pushed: ");
  Serial.print(value);
  Serial.print(" to: ");
  Serial.print(_insertIndex);
  Serial.println();
  */
  if (_isFirstValue)
  {
    _isFirstValue=false;
    for(int count=0;count<_arraySize;count++)
    {
      _values[count]=value;
    }
  }

  _values[_insertIndex] = value;
  incrementInsertIndex();
  //debugArray("_values:",_values);
}

/*
 * Debug output the internal array to serial console
 * @param *tag pointer to a string to identify the caller 
 * @param *arr the array to be printed
*/
void Smoother::debugArray(char*tag, int16_t *arr)
{
  if(!Serial)
    return;
    
  Serial.print(tag);
  Serial.print("\t:\t");
  for(int i=0;i<_arraySize;i++)
  {
    Serial.print(arr[i]);
    Serial.print(" ; ");
  }
  Serial.println();  
}

/*
 * Switched smoothing on and off
 * @param active true if smoothing is on 
 */
 void Smoother::setSmoothingActive(bool active)
 {
    _smoothingActive=active;
 }

/*
 * returns the smooth calculated value
*/
int16_t Smoother::getSmoothed()
{
  if (!_smoothingActive)
  {
    return  _values[_insertIndex];
  }

  memcpy(_sortedValues,_values , (sizeof *_sortedValues)*_arraySize);
  qsort(_sortedValues, _arraySize, sizeof(_sortedValues[0]), qsortComparer );
  //debugArray("_sortedValues:",_sortedValues);
  int32_t average=0;

  for (int loopIndex = 1; loopIndex<_arraySize-1;loopIndex++)
  {
    average+=_sortedValues[loopIndex];
  }
  average = average / (_arraySize-2);
  return (int16_t) average;
}

int Smoother::qsortComparer(const void *cmp1, const void *cmp2)
{
  int a = *((int16_t *)cmp1);
  int b = *((int16_t *)cmp2);
  return b - a;
}
