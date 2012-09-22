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

void Tracer::backwardTurning(unsigned int ms, uint8_t speed, int delta)
{
	Motor::setDuty(speed - delta, speed + delta);
	Motor::setState(JLBackward, JLBackward);
	
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
	bool ok;
	Fixed16 offset = LineSensor::getOffset(&ok);
	
	/*
	if (ok == false)
	{
		Tracer::brake();
		forever;
	}*/

	forwardTurning(DeltaMs, speed, offset * Fixed16(speed / 2));
}

void Tracer::traceBack(uint8_t speed)
{
	Fixed16 offset = LineSensor::getOffset();

	backwardTurning(DeltaMs, speed, offset * Fixed16(speed / 2));
}

void Tracer::goToNextCross(uint8_t speed)
{
	Debug::printf("tracing to next cross\n");
	
	while (LineSensor::getIfEitherSiedOnLine())
		trace(speed);
	
	uint32_t originalTimerCount = GlobalTimer::count();
	uint32_t timerCount;
	
	while (LineSensor::getIfBothSideOnLine() == false || (timerCount = GlobalTimer::count()) - originalTimerCount < MinumumTimeCount)
		trace(speed);
	
	//while (LineSensor::getIfBothSideOnLine() == false)
	//	trace(speed);
	
	Debug::printf("time : %u\n", timerCount - originalTimerCount);
}

void Tracer::goToNextCrossOrCollision(uint8_t speed)
{
	Debug::printf("tracing to next cross\n");
	
	while (LineSensor::getIfEitherSiedOnLine())
		trace(speed);
	
	while (LineSensor::getIfBothSideOnLine() == false && GPIO::isCollisionButtonOn() == false)
		trace(speed);
}

void Tracer::goBackToPrevCross(uint8_t speed)
{
	Debug::printf("tracing back to previous cross\n");
	
	while (LineSensor::getIfEitherSiedOnLine())
		backward(DeltaMs, speed);
	
	while (LineSensor::getIfBothSideOnLine() == false)
		backward(DeltaMs, speed);
}

void Tracer::goToNextCrossFor(unsigned count)
{
	if (count == 0)
		return;
	
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
	
	brakeFor(100);
	
	Motor::disable();
}

void Tracer::goBackToPrevCrossFor(unsigned int count)
{
	Motor::enable();
	
	while (count--)
		goBackToPrevCross(_goBackSpeed);
	
	brakeFor(100);
	
	Motor::disable();
}

void Tracer::turnInCross(JLDirection dir)
{
	//Motor::enable();
	
	JLDirection opposite = directionSwitch(dir);
	//JLDirection opposite = dir;
	
	Debug::printf("starting turning\n");
	while (LineSensor::getIfSideOnLine(opposite))
		turn(dir, DeltaMs, _turningSpeed);
	
	Debug::printf("off line\n");
	while (LineSensor::getIfSideOnLine(opposite) == false)
		turn(dir, DeltaMs, _turningSpeed);
	
	Debug::printf("on line again\n");
	while (LineSensor::getIfSideOnLine(opposite))
		turn(dir, DeltaMs, _turningSpeed);
	
	/*
	while (LineSensor::getIfBothSideOnLine())
		turn(dir, DeltaMs, _turningSpeed);
	
	while (LineSensor::getIfBothSideOnLine() == false)
		turn(dir, DeltaMs, _turningSpeed);
	
	while (LineSensor::getIfBothSideOnLine())
		turn(dir, DeltaMs, _turningSpeed);
	*/
	
	//brakeFor(100);
	
	//Motor::disable();
}

int Tracer::_fastSpeed = 50;
int Tracer::_slowSpeed = 16;
int Tracer::_turningSpeed = 20;
int Tracer::_goBackSpeed = 16;

