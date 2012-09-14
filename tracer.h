#ifndef TRACER_H
#define TRACER_H

#include "jidoulib.h"

enum TracerConstants
{
	TracerOffsetFactor = 4,
	TracerDefaultSpeed = 16,
	TracerTurningSpeed = 16,
	TracerDeltaMs = 0
};

/**
  ライントレーサを旋回させる
*/
void tracerTurn(JLDirection dir, unsigned ms, uint8_t speed);

int tracerGetOffset(uint8_t sensorValue);

void tracerHalt();
void tracerBrake();

void tracerForwardTurning(unsigned ms, uint8_t speed, uint8_t delta);
void tracerForward(unsigned ms, uint8_t speed);
void tracerBackward(unsigned ms, uint8_t speed);

void tracerGoToNextCross(unsigned count);
void tracerTurnInCross(JLDirection dir);


#endif
