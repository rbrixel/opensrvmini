#ifndef RINGBUFFER_H
#define RINGBUFFER_H

/* Simple Ringbuffer Class to store double elements */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include <stdio.h>
#include <algorithm>
#include "string.h"
/*
#include "esp_spiffs.h"
#include "nvs_flash.h"
*/
#define RUNBUFDEBUG
#define BASEPATH "/ringbuffer"


class RingBuffer {
	
	/*
	 * RingBuffer *buf = new RingBuffer(4,filesystem?); // Future use filesystem to persist
	 * buf->addElement(5);
	 * buf->addElement(6);
	 * buf->addElement(7);
	 * int currentIndex = buf.getIndex(); // result = 2 (0,1,2)
	 * buf->addElement(8);
	 * buf->addElement(9); // overwrites 5
	 * buf->addElement(10); // overwrites 6
	 * currentIndex = buf.getIndex(); // result = 1 (second element)
	 * int[] bufferContent = buf->getContent(); // creates array backwards: {10,9,8,7} No access to buffer
	 * buf.persistDirty(); // writes all dirty (changed elements since last persist) into filesystem. In this example all exept allready overwritten 5 and 6
	 *
	 *
	 * */
public:
	RingBuffer(int size,const char* buffName);

	void	addElement(double element);
	int	 	getIndex();
	double*	getContentHistory();	//Return array with last stored elements, starting with last added element, in order of time added
	double* getContentCopy();		//Gets copy of the internal array.
	void 	debugBuffer(double* arr);	// Writes content to console (if RUNBUFDEBUG is defined)

private:
	void turnWriteIndex();

	/* Member */
	double *_bufferElements;
	int _currentWriteIndex=0;
	int _bufferSize=0;
	const char *TAG = "RingBuffer";
	//void init_spiffs();
};

#endif /* RINGBUFFER_H */
