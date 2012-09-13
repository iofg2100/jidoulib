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

inline bool tracerAllSensorsOnLine(uint8_t sensorValue)
{
	return (0b00001 & sensorValue) && (0b10000 & sensorValue);
}

void tracerTurn(JLDirection dir, unsigned ms, uint8_t speed)
{
	motorSetDuty(JLLeft, speed);
	motorSetDuty(JLRight, speed);

	switch (dir)
	{
		case JLLeft:
			motorSetState(JLLeft, JLBackward);
			motorSetState(JLRight, JLForward);
			break;
		case JLRight:
			motorSetState(JLLeft, JLForward);
			motorSetState(JLRight, JLBackward);
			break;
		default:
			break;
	}
	
	delayMs(ms);
}

void tracerHalt()
{
	motorSetState(JLLeft, JLHalt);
	motorSetState(JLRight, JLHalt);
}

void tracerBrake()
{
	motorSetState(JLLeft, JLBrake);
	motorSetState(JLRight, JLBrake);
}

void tracerForward(unsigned ms, uint8_t speed)
{
	motorSetDuty(JLLeft, speed);
	motorSetDuty(JLRight, speed);

	motorSetState(JLLeft, JLForward);
	motorSetState(JLRight, JLForward);

	delayMs(ms);
}

void tracerForwardTurning(unsigned ms, uint8_t speed, uint8_t delta)
{
	motorSetDuty(JLLeft, speed - delta);
	motorSetDuty(JLRight, speed + delta);

	motorSetState(JLLeft, JLForward);
	motorSetState(JLRight, JLForward);

	delayMs(ms);
}

void tracerBackward(unsigned ms, uint8_t speed)
{
	motorSetDuty(JLLeft, speed);
	motorSetDuty(JLRight, speed);

	motorSetState(JLLeft, JLBackward);
	motorSetState(JLRight, JLBackward);

	delayMs(ms);
}

void tracerBrakeFor(unsigned ms)
{
	tracerBrake();
	motorSetDuty(JLLeft, 255);
	motorSetDuty(JLRight, 255);
	delayMs(ms);
}

int tracerPreviousOffset = 0;

void tracerGoToNextCross(unsigned count)
{
	motorStart();
	
	while (count--)
	{
		while (tracerAllSensorsOnLine(lineSensorGet()))	// 交差点上を抜けるまで
		{
			tracerForward(TracerDeltaMs, TracerDefaultSpeed);
		}

		debugPrintf("Tracer now on line\n");
		
		while (true)	// 線上を進む
		{
			uint8_t sensorValue = lineSensorGet();
			
			if (tracerAllSensorsOnLine(sensorValue))
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
	
	motorEnd();
}

void tracerTurnInCross(JLDirection dir)
{
	motorStart();
	
	tracerTurn(dir, 350, TracerDefaultSpeed);
	
	/*
	while (tracerAllSensorsOnLine(lineSensorGet()))
		tracerTurn(dir, TracerDeltaMs, TracerDefaultSpeed);
	
	while (!tracerAllSensorsOnLine(lineSensorGet()))
		tracerTurn(dir, TracerDeltaMs, TracerDefaultSpeed);
		*/
	
	tracerBrakeFor(100);
	
	motorEnd();
}

