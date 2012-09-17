#ifndef TRACER_H
#define TRACER_H

#include "jidoulib.h"

class Tracer
{
public:
	
	enum TracerConstants
	{
		DefaultSpeed = 32,
		SlowSpeed = 16,
		TurningSpeed = 24,
		DeltaMs = 0
	};
	
	static void turn(JLDirection dir, unsigned ms, uint8_t speed);

	static void halt();
	static void brake();

	static void forwardTurning(unsigned ms, uint8_t speed, int delta);
	static void forward(unsigned ms, uint8_t speed);
	static void backward(unsigned ms, uint8_t speed);
	
	static void goToNextCrossFor(unsigned count);
	static void turnInCross(JLDirection dir);
	
private:
	
	static void trace(uint8_t speed);
	static void brakeFor(unsigned ms);
	static void goToNextCross(uint8_t speed);
};


#endif
