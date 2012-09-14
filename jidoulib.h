#ifndef JIDOULIB_H
#define	JIDOULIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

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


void allInit();

/**
  汎用入出力初期化
*/
void gpioInit();

/**
	USART初期化
*/
void usartInit();

/**
	USRTに一文字送信
	@param data 送信するデータ
*/
void usartPut(uint8_t data);


/**
	AD変換初期化
*/
void adConversionInit();

/**
	AD変換のデータを取得
	@param pin データを取得するピン
*/
uint8_t adConversionGet(uint8_t pin);

/**
	モーターの初期化 (タイマ0など)
*/
void motorInit();

/**
	モーターのデューティ比を設定
	@param dir 右or左
	@param ratio デューティ比 (0..255)
*/
void motorSetDuty(JLDirection dir, uint8_t ratio);

/**
	モーターの状態を設定
	@param dir 右or左
	@param state モーターの状態
*/
void motorSetState(JLDirection dir, JLMotorState state);

void motorStart();
void motorEnd();

/**
	パルス幅 1.5ms+/-角度分
	バルス周期 20ms
*/

/**
	サーボモータの初期化
*/
void servoInit();

/**
	サーボモータの位相を設定
	@param phase 位相 (0..255)
*/
void servoSetPhase(unsigned phase);

void servoSetPulseWidth(unsigned us);


void delayMs(unsigned ms);

#endif
