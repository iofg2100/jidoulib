#ifndef TRACER_H
#define TRACER_H

#include "jidoulib.h"

class Tracer
{
public:
	
	enum TracerConstants
	{
		DeltaMs = 0,
		MinumumTimeCount = 1000
	};
	
	static void turn(JLDirection dir, unsigned ms, uint8_t speed);

	static void halt();
	static void brake();

	static void forwardTurning(unsigned ms, uint8_t speed, int delta);
	static void backwardTurning(unsigned ms, uint8_t speed, int delta);
	static void forward(unsigned ms, uint8_t speed);
	static void backward(unsigned ms, uint8_t speed);
	
	static void goToNextCrossFor(unsigned count);
	static void goBackToPrevCrossFor(unsigned count);
	static void turnInCross(JLDirection dir);
	
	static void setFastSpeed(int speed) { _fastSpeed = speed; }
	static void setSlowSpeed(int speed) { _slowSpeed = speed; }
	static void setTurningSpeed(int speed) { _turningSpeed = speed; }
	static void setGoBackSpeed(int speed) { _goBackSpeed = speed; }
	
	static int fastSpeed() { return _fastSpeed; }
	static int slowSpeed() { return _slowSpeed; }
	static int turningSpeed () { return _turningSpeed; }
	static int goBackSpeed() { return _goBackSpeed; }
	
	static void brakeFor(unsigned ms);
	static void goToNextCross(uint8_t speed);
	static void goToNextCrossOrCollision(uint8_t speed);
	static void goToNextCrossFast() { goToNextCross(_fastSpeed); }
	static void goToNextCrossSlow() { goToNextCross(_slowSpeed); }
	static void goBackToPrevCross(uint8_t speed);
	
private:
	
	static void trace(uint8_t speed);
	static void traceBack(uint8_t speed);
	
	static int _fastSpeed, _slowSpeed, _turningSpeed, _goBackSpeed;
};


#endif
