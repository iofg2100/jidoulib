#include <stdlib.h>
#include "debug.h"
#include "jidoulib.h"
#include "linesensor.h"

void LineSensor::initialize()
{
	setThreshold();
	//GlobalTimer::addCallback(10, onTimerEvent);
}

void LineSensor::onTimerEvent()
{
	readAnalogValue(_values);
}

void LineSensor::readAnalogValue(int *array)
{
	Debug::printf("Line sensor value:");

	for (uint8_t i = 0; i < 5; ++i)
	{
		array[i] = ADCon::get(i);
		Debug::printf(" %u", array[i]);
	}

	Debug::printf("\n");
}

inline static void swap(int &v1, int &v2)
{
	int temp = v1;
	v1 = v2;
	v2 = temp;
}

static void bubble_sort(int *array, uint8_t count)
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

static int get_abs_max(int *array, uint8_t count, uint8_t *maxIndexOut)
{
	int max = array[0];
	int absmax = abs(max);
	uint8_t maxIndex = 0;

	for (uint8_t i = 1; i < count; ++i)
	{
		int value = array[i];
		int absvalue = abs(value);
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
	int values[5];
	readAnalogValue(values);	// AD変換した値を取得
	bubble_sort(values, 5);

	int diffs[4];	// 差分を計算する
	for (uint8_t i = 0; i < 4; ++i)
	{
		diffs[i] = values[i+1] - values[i];
	}

	uint8_t maxIndex;
	int maxDiff = get_abs_max(diffs, 4, &maxIndex);	// 差分の最大値とそのインデックスを取得

	//_threshold = values[maxIndex] + maxDiff * 3 / 4;	//閾値っぽい値を計算
	_threshold = values[maxIndex] + maxDiff / 2;	//閾値っぽい値を計算

	Debug::printf("Line seosor threshold: %d\n", _threshold);
}


Fixed16 LineSensor::getOffset(bool *ok)
{
	Fixed16 sum(0);
	uint8_t count = 0;
	
	for (uint8_t i = 0; i < 3; ++i)
	{
		uint8_t value = ADCon::get(i+1);
		
		if (value > _threshold)
		{
			sum += Fixed16(int(i) - 1);
			count++;
		}
	}
	
	if (count)
	{
		Fixed16 offset = sum / Fixed16(count);
		_prevOffset = offset;
		
		if (ok)
			*ok = true;
		
		return offset;
	}
	else
	{
		if (ok)
			*ok = false;
		
		return _prevOffset;
	}
}

bool LineSensor::getIfSideOnLine(JLDirection dir)
{
	uint8_t value = (dir == JLLeft) ? ADCon::get(4) : ADCon::get(0);
	
	bool on = value > _threshold;
	
	if (on != _sideLowPassState[dir])
		_sideLowPassCount[dir]++;
	else
		_sideLowPassCount[dir] = 0;
	
	if (_sideLowPassCount[dir] >= SideLowPassCount)
	{
		_sideLowPassCount[dir] = 0;
		_sideLowPassState[dir] = !(_sideLowPassState[dir]);
	}
	
	return _sideLowPassState[dir];
}

int LineSensor::_threshold;
int LineSensor::_values[5];
Fixed16 LineSensor::_prevOffset;
int LineSensor::_sideLowPassCount[2] = {0, 0};
bool LineSensor::_sideLowPassState[2] = {false, false};

