#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <stdint.h>
#include "fixed.h"
#include "jidoulib.h"

class LineSensor
{
public:
	
	static void initialize();
	
	/**
	right - greater
	cernter - 0
	*/
	static Fixed16 getOffset();
	
	static bool getIfSideOnLine(JLDirection dir);
	
	static void setThreshold();
	static void onTimerEvent();
	
	static int16_t value(int index) { return _values[index]; }
	
private:
	static void readAnalogValue(int16_t *array);
	
	static int16_t _values[5];
	static Fixed16 _prevOffset;
	static uint8_t _threshold;
};


#endif
