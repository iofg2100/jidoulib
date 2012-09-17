#include <stdlib.h>
#include "debug.h"
#include "jidoulib.h"
#include "linesensor.h"

uint8_t lineSensorThreshold;

void LineSensor::readAnalogValue(int16_t *array)
{
	Debug::printf("Line sensor value:");

	for (uint8_t i = 0; i < 5; ++i)
	{
		array[i] = ADCon::get(i);
		Debug::printf(" %u", array[i]);
	}

	Debug::printf("\n");
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

void LineSensor::setThreshold()
{
	int16_t values[5];
	readAnalogValue(values);	// AD変換した値を取得
	bubble_sort(values, 5);

	int16_t diffs[4];	// 差分を計算する
	for (uint8_t i = 0; i < 4; ++i)
	{
		diffs[i] = values[i+1] - values[i];
	}

	uint8_t maxIndex;
	int16_t maxDiff = get_abs_max(diffs, 4, &maxIndex);	// 差分の最大値とそのインデックスを取得

	lineSensorThreshold = values[maxIndex] + maxDiff / 2;	//閾値っぽい値を計算

	Debug::printf("Line seosor threshold: %d\n", lineSensorThreshold);
}

Fixed16 LineSensor::_prevOffset;

Fixed16 LineSensor::getOffset()
{
	Fixed16 sum(0);
	uint8_t count = 0;
	
	for (uint8_t i = 0; i < 3; ++i)
	{
		uint8_t value = ADCon::get(i+1);
		
		if (value > lineSensorThreshold)
		{
			sum += Fixed16(int(i) - 1);
			count++;
		}
	}
	
	if (count)
	{
		Fixed16 offset = sum / Fixed16(count);
		_prevOffset = offset;
		return offset;
	}
	else
	{
		return _prevOffset;
	}
}

bool LineSensor::getIfSideOnLine(JLDirection dir)
{
	uint8_t value = (dir == JLLeft) ? ADCon::get(4) : ADCon::get(0);
	return value > lineSensorThreshold;
}


