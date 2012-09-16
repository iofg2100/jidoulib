#ifndef LINESENSOR_H
#define LINESENSOR_H

#include <stdint.h>
#include "jidoulib.h"

enum LineSensorConstants
{
	LineSensorOffsetFactor = 8
};

/**
	right - greater
	cernter - 0
*/
int lineSensorGetOffset();

bool lineSensorGetIfSideOnLine(JLDirection dir);

inline bool lineSensorIfOnCross()
{
	return lineSensorGetIfSideOnLine(JLLeft) || lineSensorGetIfSideOnLine(JLRight);
}

void lineSensorSetThreshold();

#endif
