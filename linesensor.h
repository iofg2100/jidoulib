#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <stdint.h>
#include "fixed.h"
#include "jidoulib.h"

class LineSensor
{
public:
	
	/**
	right - greater
	cernter - 0
	*/	
	static Fixed16 getOffset();
	
	static bool getIfSideOnLine(JLDirection dir);
	
	static void setThreshold();
	
private:
	static void readAnalogValue(int16_t *array);
	
	static Fixed16 _prevOffset;
};


#endif
