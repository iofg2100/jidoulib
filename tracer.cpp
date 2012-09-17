#include "jidoulib.h"
#include "linesensor.h"
#include "debug.h"

#include "tracer.h"

void Tracer::turn(JLDirection dir, unsigned ms, uint8_t speed)
{
	Motor::setDuty(speed, speed);

	switch (dir)
	{
		case JLLeft:
			Motor::setState(JLBackward, JLForward);
			break;
		case JLRight:
			Motor::setState(JLForward, JLBackward);
			break;
		default:
			break;
	}
	
	delayMs(ms);
}

void Tracer::halt()
{
	Motor::setState(JLHalt, JLHalt);
}

void Tracer::brake()
{
	Motor::setState(JLBrake, JLBrake);
}

void Tracer::forward(unsigned ms, uint8_t speed)
{
	Motor::setDuty(speed, speed);
	
	Motor::setState(JLForward, JLForward);

	delayMs(ms);
}

void Tracer::forwardTurning(unsigned ms, uint8_t speed, int delta)
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
	
	Motor::setDuty(speed - delta, speed + delta);
	//motorSetDuty(speed - 2 * delta, speed - delta);
	
	Motor::setState(JLForward, JLForward);

	delayMs(ms);
}

void Tracer::backward(unsigned ms, uint8_t speed)
{
	Motor::setDuty(speed, speed);

	Motor::setState(JLBackward, JLBackward);

	delayMs(ms);
}

void Tracer::brakeFor(unsigned ms)
{
	brake();
	Motor::setDuty(255, 255);
	delayMs(ms);
}

void Tracer::trace(uint8_t speed)
{
	Fixed16 offset = LineSensor::getOffset();

	forwardTurning(DeltaMs, speed, offset * Fixed16(speed / 2));
}

void Tracer::goToNextCross(uint8_t speed)
{
	while (LineSensor::getIfSideOnLine(JLLeft) || LineSensor::getIfSideOnLine(JLRight))
		trace(speed);
	
	while ((LineSensor::getIfSideOnLine(JLLeft) && LineSensor::getIfSideOnLine(JLRight)) == false)
		trace(speed);
}

void Tracer::goToNextCrossFor(unsigned count)
{
	if (count == 0)
		return;
	
	Debug::printf("going to next cross\n");
	
	Motor::enable();
	
	if (count == 1)
	{
		goToNextCross(_slowSpeed);
	}
	else if (count == 2)
	{
		goToNextCross(_slowSpeed);
		goToNextCross(_slowSpeed);
	}
	else
	{
		goToNextCross(_slowSpeed);
		
		count -= 2;
		while (count--)
		goToNextCross(_fastSpeed);
		
		brakeFor(100);
		
		goToNextCross(_slowSpeed);
	}
	
	Debug::printf("cross reached\n");
	
	brakeFor(100);
	
	Motor::disable();
}

void Tracer::turnInCross(JLDirection dir)
{
	Debug::printf("turning...\n");
	
	Motor::enable();
	
	JLDirection opposite = directionSwitch(dir);
	//JLDirection opposite = dir;
	
	while (LineSensor::getIfSideOnLine(opposite))
		turn(dir, DeltaMs, _turningSpeed);
	
	while (LineSensor::getIfSideOnLine(opposite) == false)
		turn(dir, DeltaMs, _turningSpeed);
	
	while (LineSensor::getIfSideOnLine(opposite))
		turn(dir, DeltaMs, _turningSpeed);
	
	brakeFor(100);
	
	Motor::disable();
}

int Tracer::_fastSpeed = 32;
int Tracer::_slowSpeed = 16;
int Tracer::_turningSpeed = 20;

