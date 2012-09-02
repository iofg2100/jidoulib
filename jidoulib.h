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
typedef enum
{
	JL_RIGHT,
	JL_LEFT
} JL_DIRECTION;

/**
	モーターの状態
*/
typedef enum
{
	JL_FORWARD = 0b00,
	JL_BACKWARD = 0b11,
	JL_HALT = 0b01,
	JL_BRAKE = 0b10
} JL_MOTOR_STATE;


/**
	自動機1初期化
*/
void jidou1Init();

/**
	自動機2初期化
*/
void jidou2Init();

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
void motorSetDuty(JL_DIRECTION dir, uint8_t ratio);

/**
	モーターの状態を設定
	@param dir 右or左
	@param state モーターの状態
*/
void motorSetState(JL_DIRECTION dir, JL_MOTOR_STATE state);


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
void servoSetPhase(uint8_t phase);

void delayMs(unsigned ms);

#endif
