#ifndef TRACER_H
#define TRACER_H

#include "stdint.h"

void tracerTurn(JL_DIRECTION dir);

void tracerForward();
void tracerBackward();

void tracerGoToNextCross();
void tracerTurnInCross(JL_DIRECTION dir);

typedef enum
{
	TRACER_LINE,
	TRACER_CROSS,
	TRACER_CROSS_ROTATING
} TRACER_STATE;


#endif
