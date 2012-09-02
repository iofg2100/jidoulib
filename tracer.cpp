#include "jidoulib.h"
#include "linesensor.h"

#include "tracer.h"


uint8_t tracerSensorState;

int tracerGetOffset(uint8_t sensorValue)
{
	int sum = 0;
	unsigned onLineCount = 0;
	
	for (unsigned i = 0; i < 5; ++i)
	{
		if (sensorValue & (1 << i))
		{
			sum += 16 * (int(i) - 2);
			onLineCount++;
		}
	}
	
	return sum / onLineCount;
}

inline bool tracerAllSensorsOnLine(uint8_t sensorValue)
{
	return sensorValue == 0b11111;
}

void tracerTurn(JL_DIRECTION dir, unsigned ms)
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
	delayMs(ms);
}

void tracerStop()
{
	motorSetState(JL_LEFT, JL_BRAKE);
	motorSetState(JL_RIGHT, JL_BRAKE);
}

void tracerForward(unsigned ms)
{
	motorSetState(JL_LEFT, JL_FORWARD);
	motorSetState(JL_RIGHT, JL_FORWARD);
	
	delayMs(ms);
}

void tracerBackward(unsigned ms)
{
	motorSetState(JL_LEFT, JL_BACKWARD);
	motorSetState(JL_RIGHT, JL_BACKWARD);
	
	delayMs(ms);
}

void tracerGoToNextCross()
{
	while (tracerAllSensorsOnLine(lineSensorGet()))	// 交差点上を抜けるまで
	{
		tracerForward(10);
	}
	
	while (true)
	{
		int sensorValue = lineSensorGet();
		
		if (tracerAllSensorsOnLine(sensorValue))
			break;
		
		int offset = tracerGetOffset(sensorValue);
		if (offset > 0)
			tracerTurn(JL_RIGHT, offset);
		if (offset < 0)
			tracerTurn(JL_LEFT, -offset);
	}
}

void tracerTurnInCross(JL_DIRECTION dir)
{
	while (tracerAllSensorsOnLine(lineSensorGet()))
		tracerTurn(dir, 10);
	
	while (!tracerAllSensorsOnLine(lineSensorGet()))
		tracerTurn(dir, 10);
}

TRACER_STATE tracerState;

