#ifndef TRACER_H
#define TRACER_H

#include "jidoulib.h"

enum TracerConstants
{
	TracerOffsetFactor = 16,
	TracerDefaultSpeed = 32,
	TracerTurningSpeed = 24,
	TracerDeltaMs = 0
};

/**
  ライントレーサを旋回させる
*/
void tracerTurn(JLDirection dir, unsigned ms, uint8_t speed);

void tracerHalt();
void tracerBrake();

void tracerForwardTurning(unsigned ms, uint8_t speed, int delta);
void tracerForward(unsigned ms, uint8_t speed);
void tracerBackward(unsigned ms, uint8_t speed);

void tracerGoToNextCrossFor(unsigned count);
void tracerTurnInCross(JLDirection dir);

#endif
