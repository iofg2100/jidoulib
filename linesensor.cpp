#include <stdlib.h>
#include "jidoulib.h"
#include "linesensor.h"

uint8_t lineSensorThreshold;

static void lineSensorReadAnalogValue(uint8_t *array)
{
	for (unsigned i = 0; i < 5; ++i)
		array[i] = adConversionGet(i);
}

static void bubble_sort(uint8_t *array, unsigned count)
{
	for (unsigned i = 0; i < count; ++i)
	{
		for (unsigned j = count - 1; j > i; --j)
		{
			if (array[j] < array[j-1])
			{
				uint8_t temp = array[j];
				array[j] = array[j-1];
				array[j-1] = temp;
			}
		}
	}
}

static uint8_t get_max(uint8_t *array, unsigned count, unsigned *maxIndex)
{
	uint8_t max = array[0];
	*maxIndex = 0;

	for (unsigned i = 1; i < count; ++i)
	{
		uint8_t value = array[i];
		if (max < value)
		{
			max = value;
			*maxIndex = i;
		}
	}
	
	return max;
}

void lineSensorSetThreshold()
{
	uint8_t values[5];
	lineSensorReadAnalogValue(values);	// AD変換した値を取得
	bubble_sort(values, 5);
	
	uint8_t diffs[4];	// 差分を計算する
	for (unsigned i = 0; i < 4; ++i)
		diffs[i] = abs(values[i+i] - values[i]);
	
	unsigned maxIndex;
	uint8_t maxDiff = get_max(diffs, 4, &maxIndex);	// 差分の最大値とそのインデックスを取得
	
	lineSensorThreshold = values[maxIndex] + maxDiff / 2;	//閾値っぽい値を計算
}

uint8_t lineSensorGet()
{
	uint8_t values[5];
	lineSensorReadAnalogValue(values);
	
	uint8_t result = 0;
	
	for (unsigned i = 0; i < 5; ++i)
	{
		if (values[i] > lineSensorThreshold)
			result |= 1 << i;
	}
	
	return result;
}


