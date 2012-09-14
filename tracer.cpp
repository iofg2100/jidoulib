#include "jidoulib.h"
#include "linesensor.h"
#include "debug.h"

#include "tracer.h"

uint8_t tracerSensorState;

int tracerGetOffset(uint8_t sensorValue)
{
	int sum = 0;
	uint8_t onLineCount = 0;
	
	for (uint8_t i = 0; i < 5; ++i)
	{
		if (sensorValue & (1 << i))
		{
			int delta = int(i) - 2;
			
			if (delta == 2)
				delta = 3;
			
			if (delta == -2)
				delta = -3;
			
			sum += TracerOffsetFactor * delta;
			onLineCount++;
		}
	}

	return onLineCount ? (sum / onLineCount) : 0;
}

void tracerTurn(JLDirection dir, unsigned ms, uint8_t speed)
{
	motorSetDuty(speed, speed);

	switch (dir)
	{
		case JLLeft:
			motorSetState(JLBackward, JLForward);
			break;
		case JLRight:
			motorSetDuty(JLForward, JLBackward);
			break;
		default:
			break;
	}
	
	delayMs(ms);
}

void tracerHalt()
{
	motorSetState(JLHalt, JLHalt);
}

void tracerBrake()
{
	motorSetState(JLBrake, JLBrake);
}

void tracerForward(unsigned ms, uint8_t speed)
{
	motorSetDuty(speed, speed);
	
	motorSetState(JLForward, JLForward);

	delayMs(ms);
}

void tracerForwardTurning(unsigned ms, uint8_t speed, int delta)
{
	if (delta > 0)
	{
		motorSetDuty(speed - delta, speed);
	}
	else
	{
		motorSetDuty(speed, speed + delta);
	}
	
	motorSetState(JLForward, JLForward);

	delayMs(ms);
}

void tracerBackward(unsigned ms, uint8_t speed)
{
	motorSetDuty(speed, speed);

	motorSetState(JLBackward, JLBackward);

	delayMs(ms);
}

void tracerBrakeFor(unsigned ms)
{
	tracerBrake();
	motorSetDuty(255, 255);
	delayMs(ms);
}

int tracerPreviousOffset = 0;

inline bool tracerRightOnLine(uint8_t sensor)
{
	return 0b00001 & sensor;
}

inline bool tracerLeftOnLine(uint8_t sensor)
{
	return 0b10000 & sensor;
}

bool tracerOnCross(uint8_t sensor)
{
	/*
	uint8_t mask = 1;
	uint8_t count = 0;
	
	for (uint8_t i = 0; i < 5; ++i)
	{
		if (sensor & mask)
			++count;
		mask <<= 1;
	}
	
	return count >= 4;
	*/
	
	return tracerLeftOnLine(sensor) && tracerRightOnLine(sensor);
}

bool tracerSideOnLine(JLDirection dir, uint8_t sensor)
{
	if (dir == JLRight)
		return tracerRightOnLine(sensor);
	else
		return tracerLeftOnLine(sensor);
}

bool tracerOppositeSideOnLine(JLDirection dir, uint8_t sensor)
{
	if (dir == JLLeft)
		return tracerRightOnLine(sensor);
	else
		return tracerLeftOnLine(sensor);
}


void tracerGoToNextCross(unsigned count)
{
	motorEnable();
	
	while (count--)
	{
		while (tracerOnCross(lineSensorGet()))	// 交差点上を抜けるまで
		{
			tracerForward(TracerDeltaMs, TracerDefaultSpeed);
		}

		debugPrintf("Tracer now on line\n");
		
		while (true)	// 線上を進む
		{
			uint8_t sensorValue = lineSensorGet();
			
			if (tracerOnCross(sensorValue))
				break;
			
			int offset;
			
			if (sensorValue == 0)
			{
				if (tracerPreviousOffset > 0)
					offset = TracerOffsetFactor * 3;
				else
					offset = -TracerOffsetFactor * 3;
			}
			else
			{
				offset = tracerGetOffset(sensorValue);
				tracerPreviousOffset = offset;
			}

			debugPrintf("Tracer offset: %d\n", offset);

			tracerForwardTurning(TracerDeltaMs, TracerDefaultSpeed, offset);
		}
	}
	
	tracerBrakeFor(100);
	
	motorDisable();
}

void tracerTurnInCross(JLDirection dir)
{
	motorEnable();
	
	//tracerTurn(dir, 350, TracerDefaultSpeed);
	
	while (tracerOppositeSideOnLine(dir, lineSensorGet()))
		tracerTurn(dir, TracerDeltaMs, TracerTurningSpeed);
	
	while (!tracerOppositeSideOnLine(dir, lineSensorGet()))
		tracerTurn(dir, TracerDeltaMs, TracerTurningSpeed);
	
	tracerBrakeFor(100);
	
	motorDisable();
}

