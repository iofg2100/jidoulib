#ifndef TRACER_H
#define TRACER_H

#include "stdint.h"

/**
  ライントレーサを旋回させる
*/
void tracerTurn(JL_DIRECTION dir, unsigned ms);

int tracerGetOffset(uint8_t sensorValue);

void tracerStop();

void tracerForward(unsigned ms);
void tracerBackward(unsigned ms);

void tracerGoToNextCross();
void tracerTurnInCross(JL_DIRECTION dir);

typedef enum
{
	TRACER_LINE,
	TRACER_CROSS,
	TRACER_CROSS_ROTATING
} TRACER_STATE;


#endif
