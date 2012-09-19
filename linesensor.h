#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <stdint.h>
#include "fixed.h"
#include "jidoulib.h"

class LineSensor
{
public:
	
	enum
	{
		SideLowPassCount = 4
	};
	
	static void initialize();
	
	/**
	right - greater
	cernter - 0
	*/
	static Fixed16 getOffset();
	
	static bool getIfSideOnLine(JLDirection dir);
	static bool getIfEitherSiedOnLine() { return getIfSideOnLine(JLLeft) || getIfSideOnLine(JLRight); }
	static bool getIfBothSideOnLine() { return getIfSideOnLine(JLLeft) && getIfSideOnLine(JLRight); }
	
	static void setThreshold();
	static void onTimerEvent();
	
	static int value(int index) { return _values[index]; }
	
private:
	static bool jidgeWidhThreshold(int value);
	
	static void readAnalogValue(int *array);
	
	static int _values[5];
	static Fixed16 _prevOffset;
	static int _threshold;
	
	static int _sideOnLineCount[2];
};


#endif
