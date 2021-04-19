
#include "../RingBuffer/RingBuffer.h"
#include "../RingBuffer/RingBuffer.h"


RingBuffer::RingBuffer(int size, const  char* buffName)
{
	_bufferSize=size;
	_bufferElements = new double[size];
	std::fill(_bufferElements, _bufferElements+size, 0);
}

void RingBuffer::addElement(double element)
{
	_bufferElements[_currentWriteIndex]=element;
	turnWriteIndex();
	debugBuffer(_bufferElements);
}

void RingBuffer::turnWriteIndex(){
	_currentWriteIndex=((_currentWriteIndex+1)%_bufferSize);
}

int RingBuffer::getIndex(){
	return _currentWriteIndex;
}

double* RingBuffer::getContentHistory(){
	double* result = new double[_bufferSize];
	int startIndex = _currentWriteIndex;

	for (int destIndex=0;destIndex<_bufferSize;destIndex++)
	{
		ESP_LOGI(TAG,"BufferIndex=%d = %d",destIndex, (destIndex+startIndex)%_bufferSize);
		result[destIndex]=_bufferElements[(destIndex+startIndex)%_bufferSize];
	}
	return result;
}

double* RingBuffer::getContentCopy(){
	double* result = new double[_bufferSize];
	memcpy(result,_bufferElements, _bufferSize*sizeof(_bufferElements));
	return result;
}

void RingBuffer::debugBuffer(double* arr)
{
#ifdef RUNBUFDEBUG
	for (int i=0;i<_bufferSize;i++)
	{
		if(i==_currentWriteIndex)
		{
			ESP_LOGI(TAG , "--- Array Clean %d = %lf <-*",i,arr[i] );
		}else{
			ESP_LOGI(TAG , "--- Array Clean %d = %lf",i,arr[i] );
		}
	}
#endif
}
