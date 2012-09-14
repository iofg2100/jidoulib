#ifndef JIDOULIB_H
#define	JIDOULIB_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

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
*/
void motorSetDuty(uint8_t leftRatio, uint8_t rightRatio);

/**
	モーターの状態を設定
*/
void motorSetState(JLMotorState leftState, JLMotorState rightState);

void motorEnable();
void motorDisable();

/**
	パルス幅 1.5ms+/-角度分
	バルス周期 20ms
*/

/**
	サーボモータの初期化
*/
void servoInit();

void servoEnable(uint8_t index);
void servoDisable(uint8_t index);

/**
	サーボモータの位相を設定
	@param phase 位相 (0..255)
*/
void servoSetPhase(uint8_t index, unsigned phase);

void servoSetPulseWidth(uint8_t index, unsigned us);


void delayMs(unsigned ms);

#endif
