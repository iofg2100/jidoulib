#ifndef ARMS_H
#define ARMS_H

#include "jidoulib.h"

class BallArms
{
public:
	
	enum
	{
		ServoUpDown = 0,
		ServoHoldRelease = 1,
		
		PulseWidthHold = 2000,
		PulseWidthRelease = 1000,
		PulseWidthUp = 1500,
		PulseWidthDown = 1000
	};
	
	static void initialize();
	static void hold();
	static void release();
	
private:
	
	static void servoHold() { Servo::setPulseWidth(ServoHoldRelease, PulseWidthHold); }
	static void servoRelease() { Servo::setPulseWidth(ServoHoldRelease, PulseWidthRelease); }
	static void servoUp() { Servo::setPulseWidth(ServoUpDown, PulseWidthUp); }
	static void servoDown() { Servo::setPulseWidth(ServoUpDown, PulseWidthDown); }
	
};

class BasketArms
{
public:
	
	static void initialize();
	static void hold();
	static void release();	
};

#endif