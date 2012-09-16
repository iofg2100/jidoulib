#include "jidoulib.h"
#include "linesensor.h"
#include "debug.h"

#include "tracer.h"

uint8_t tracerSensorState;



void tracerTurn(JLDirection dir, unsigned ms, uint8_t speed)
{
	motorSetDuty(speed, speed);

	switch (dir)
	{
		case JLLeft:
			motorSetState(JLBackward, JLForward);
			break;
		case JLRight:
			motorSetState(JLForward, JLBackward);
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
	/*
	if (delta > 0)
	{
		motorSetDuty(speed - delta, speed);
	}
	else
	{
		motorSetDuty(speed, speed + delta);
	}*/
	
	//debugPrintf("turning offset: %d\n", delta);
	
	motorSetDuty(speed - delta, speed + delta);
	
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


static void tracerGoToNextCross()
{
	while (lineSensorIfOnCross())	// 交差点上を抜けるまで
	{
		tracerForward(TracerDeltaMs, TracerDefaultSpeed);
	}
			
	while (true)	// 線上を進む
	{
		if (lineSensorIfOnCross())
			return;
		
		int offset = lineSensorGetOffset();

		//debugPrintf("Tracer offset: %d\n", offset);

		tracerForwardTurning(TracerDeltaMs, TracerDefaultSpeed, offset * (TracerOffsetFactor / LineSensorOffsetFactor));
	}
	
	while (lineSensorIfOnCross())	// 交差点上を抜けるまで
	{
		tracerForward(TracerDeltaMs, TracerDefaultSpeed);
	}
}

void tracerGoToNextCrossFor(unsigned count)
{
	debugPrintf("going to next cross\n");
	
	motorEnable();
	
	while (count--)
		tracerGoToNextCross();
	
	debugPrintf("cross reached\n");
	
	tracerBrakeFor(100);
	
	motorDisable();
}

void tracerTurnInCross(JLDirection dir)
{
	debugPrintf("turning...\n");
	
	motorEnable();
	
	JLDirection opposite = directionSwitch(dir);
	
	if (lineSensorGetIfSideOnLine(opposite) == false)
	{
		while (lineSensorGetIfSideOnLine(opposite) == false)
			tracerTurn(dir, TracerDeltaMs, TracerTurningSpeed);
	}
	
	while (lineSensorGetIfSideOnLine(opposite))
		tracerTurn(dir, TracerDeltaMs, TracerTurningSpeed);
	
	while (lineSensorGetIfSideOnLine(opposite) == false)
		tracerTurn(dir, TracerDeltaMs, TracerTurningSpeed);
	
	while (lineSensorGetIfSideOnLine(opposite))
		tracerTurn(dir, TracerDeltaMs, TracerTurningSpeed);
		
	tracerBrakeFor(100);
	
	motorDisable();
}

