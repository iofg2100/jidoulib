
#include "jidoulib.h"
#include "linesensor.h"

#include "tracer.h"

#define MOTION_WAIT_MS 10

void tracerTurn(JL_DIRECTION dir)
{
	switch (dir)
	{
		case JL_LEFT:
			motorSetState(JL_LEFT, JL_BACKWARD);
			motorSetState(JL_RIGHT, JL_FORWARD);
			break;
		case JL_RIGHT:
			motorSetState(JL_LEFT, JL_FORWARD);
			motorSetState(JL_RIGHT, JL_BACKWARD);
			break;
		default:
			break;
	}
	delay_ms(MOTION_WAIT_MS);
	tracerLineSensorState = lineSensorGet();
}

void tracerForward()
{
	motorSetState(JL_LEFT, JL_FORWARD);
	motorSetState(JL_RIGHT, JL_FORWARD);
	
	delay_ms(MOTION_WAIT_MS);
	tracerLineSensorState = lineSensorGet();
}

void tracerBackward()
{
	motorSetState(JL_LEFT, JL_BACKWARD);
	motorSetState(JL_RIGHT, JL_BACKWARD);
	
	delay_ms(MOTION_WAIT_MS);
	tracerLineSensorState = lineSensorGet();
}

uint8_t tracerSensorState;

inline bool tracerAllSensorsOnLine()
{
	return tracerSensorState == 0b11111;
}

void tracerGoToNextCross()
{
	TRACER_STATE state;
	
	while (true)
	{
		motionForward();
		if (tracerAllSensorsOnLine())
			break;
	}

	while (true)
	{
		if (tracerAllSensorsOnLine())
			break;
	}
}

void tracerTurnInCross(JL_DIRECTION dir)
{
	while (true)
	{
		motionTurn(dir);
		if (tracerAllSensorsOnLine())
			break;
	}
	
	while (true)
	{
		motionTurn(dir);
		if (tracerAllSensorsOnLine())
			break;
	}
}

TRACER_STATE tracerState;

