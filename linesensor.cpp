#include <stdlib.h>
#include "debug.h"
#include "jidoulib.h"
#include "linesensor.h"

uint8_t lineSensorThreshold;

static void lineSensorReadAnalogValue(int16_t *array)
{
	debugPrintf("Line sensor value:");

	for (uint8_t i = 0; i < 5; ++i)
	{
		array[i] = adConversionGet(i);
		debugPrintf(" %u", array[i]);
	}

	debugPrintf("\n");
}

inline static void swap(int16_t &v1, int16_t &v2)
{
	int temp = v1;
	v1 = v2;
	v2 = temp;
}

static void bubble_sort(int16_t *array, uint8_t count)
{
	for (uint8_t i = 0; i < count - 1; ++i)
	{
		for (uint8_t j = count - 1; j > i; --j)
		{
			if (array[j] < array[j-1])
				swap(array[j], array[j-1]);
		}
	}
}

static int16_t get_abs_max(int16_t *array, uint8_t count, uint8_t *maxIndexOut)
{
	int16_t max = array[0];
	int16_t absmax = abs(max);
	uint8_t maxIndex = 0;

	for (uint8_t i = 1; i < count; ++i)
	{
		int16_t value = array[i];
		int16_t absvalue = abs(value);
		if (absmax < absvalue)
		{
			max = value;
			absmax = absvalue;
			maxIndex = i;
		}
	}
	
	*maxIndexOut = maxIndex;
	return max;
}

void lineSensorSetThreshold()
{
	int16_t values[5];
	lineSensorReadAnalogValue(values);	// AD変換した値を取得
	bubble_sort(values, 5);

	int16_t diffs[4];	// 差分を計算する
	for (uint8_t i = 0; i < 4; ++i)
	{
		diffs[i] = values[i+1] - values[i];
	}

	uint8_t maxIndex;
	int16_t maxDiff = get_abs_max(diffs, 4, &maxIndex);	// 差分の最大値とそのインデックスを取得

	lineSensorThreshold = values[maxIndex] + maxDiff / 2;	//閾値っぽい値を計算

	debugPrintf("Line seosor threshold: %d\n", lineSensorThreshold);
}

int lineSensorPreviousOffset;

int lineSensorGetOffset()
{
	int sum = 0;
	uint8_t count = 0;
	
	for (uint8_t i = 0; i < 3; ++i)
	{
		uint8_t value = adConversionGet(i+1);
		
		if (value > lineSensorThreshold)
		{
			int delta = int(i) - 1;
			
			sum += LineSensorOffsetFactor * delta;
			count++;
		}
	}
	
	if (count)
	{
		int offset = sum / int(count);
		lineSensorPreviousOffset = offset;
		return offset;
	}
	else
	{
		return lineSensorPreviousOffset;
	}
}

bool lineSensorGetIfSideOnLine(JLDirection dir)
{
	uint8_t value = (dir == JLLeft) ? adConversionGet(4) : adConversionGet(0);
	return value > lineSensorThreshold;
}


