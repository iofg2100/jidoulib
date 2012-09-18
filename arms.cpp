#include "arms.h"

void BallArms::initialize()
{
	Servo::enable(ServoHoldRelease);
	Servo::enable(ServoUpDown);
	
	servoRelease();
	servoDown();
}

void BallArms::hold()
{
	// grab ball
	servoHold();
	
	delayMs(500);
	
	servoUp();
}

void BallArms::release()
{
	servoRelease();
	
	delayMs(500);
	
	servoHold();
}



