#ifndef JIDOULIB_H
#define	JIDOULIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "fixed.h"

#define forever while(true)

/**
	ピン配置
	
	ADC0~4 : ラインセンサ1~5
	
	PB0 : スイッチ
	OC1A : サーボモータ用PWM
	PB2, PB3 : モータドライバ
	
	モータドライバ
	
	OC0A : PWM1
	PB4 : DIRA1
	PB5 : DIRB1
	
	OC0B : PWM2
	PD2 : DIRA2
	PD3 : DIRB2
	
	PD4 : スイッチ
	
	PWMは20kHzまで
*/

/**
	右or左
*/
enum JLDirection
{
	JLRight,
	JLLeft
};

/**
	右を左、左を右にする
	@param dir 変更する方向
*/
inline JLDirection directionSwitch(JLDirection dir)
{
	return (dir == JLRight) ? JLLeft : JLRight;
}

/**
	モーターの状態
*/
enum JLMotorState
{
	JLForward = 0b00,
	JLBackward = 0b11,
	JLHalt = 0b01,
	JLBrake = 0b10
};

/**
	すべての機能の初期化
*/
void jlInit();

class GPIO
{
public:
	static void init();
	
	static bool isPushButtonOn() { return PIND & 0b10000; }
	static bool isCollisionButtonOn() { return PINB & 1; }
	
	static void waitUntilButtonNegated();
	static void setLEDOn(bool on);
};

class USART
{
public:
	static void init();
	static void put(uint8_t data);
};



class ADCon
{
public:
	static void init();
	static uint8_t get(uint8_t pin);
	
private:
	static void setPin(uint8_t pin);
};



class Timer2
{
public:
	static void init();
};


typedef void (*JLCallback)();

class GlobalTimer
{
public:
	
	enum
	{
		MaxCallbackCount = 8
	};
	
	static uint32_t count() { return _count; }
	
	static void onTimerOverflow() { _count++; }
	
	static void addCallback(uint32_t interval, JLCallback callback);
	
private:
	static uint32_t _count;
	//static uint8_t _callbackCount;
	//static uint32_t _callbackIntervals[MaxCallbackCount];
	//static JLCallback _callbacks[MaxCallbackCount];
};



class Timer0
{
public:
	
	static void init();
};



class Motor
{
public:
	static void setDuty(uint8_t left, uint8_t right);
	static void setState(JLMotorState left, JLMotorState right);
	
	static void enable();
	static void disable();
};

class Timer1
{
public:
	static void init();	
};


/**
	パルス幅 1.5ms+/-角度分
	バルス周期 20ms
*/
class Servo
{
public:
	static void enable(uint8_t index);
	static void disable(uint8_t index);
	
	static void setPhase(uint8_t index, unsigned phase);
	static void setPulseWidth(uint8_t index, unsigned ms);
	
	static void setDegree(uint8_t index, unsigned degree);
};


void delayMs(unsigned ms);

#endif
