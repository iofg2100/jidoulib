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
		
		PulseWidthHold = 2100,
		PulseWidthRelease = 1000,
		PulseWidthUp = 1500,
		PulseWidthDown = 1000
	};
	
	static void enable() { Servo::enable(ServoUpDown); Servo::enable(ServoHoldRelease); }
	
	static void hold() { Servo::setPulseWidth(ServoHoldRelease, PulseWidthHold); }
	static void release() { Servo::setPulseWidth(ServoHoldRelease, PulseWidthRelease); }
	static void up() { Servo::setPulseWidth(ServoUpDown, PulseWidthUp); }
	static void down() { Servo::setPulseWidth(ServoUpDown, PulseWidthDown); }
	
	static void disable();
};

class BasketArms
{
public:
	
	enum
	{
		ServoHoldRelease0 = 0,
		ServoHoldRelease1 = 1,
		
		PulseWidthHold0 = 1000,
		PulseWidthRelease0 = 1800,
		PulseWidthHold1 = 2250,
		PulseWidthRelease1 = 1000
	};
	
	static void enable()
	{
		Servo::enable(ServoHoldRelease0);
		Servo::enable(ServoHoldRelease1);
	}
	static void hold()
	{
		Servo::setPulseWidth(ServoHoldRelease0, PulseWidthHold0);
		Servo::setPulseWidth(ServoHoldRelease1, PulseWidthHold1);
	}
	static void release()
	{
		Servo::setPulseWidth(ServoHoldRelease0, PulseWidthRelease0);
		Servo::setPulseWidth(ServoHoldRelease1, PulseWidthRelease1);
	}
};

#endif