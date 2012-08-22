#ifndef JIDOULIB_H
#define	JIDOULIB_H

#include <stdint.h>

/**
	ピン配置
	
	ADC0~4 : ラインセンサ1~5
	PB0, PB1 エンコーダ1
	PB2, PB3 エンコーダ2
	
	モータドライバ
	
	OC0A : PWM1
	PB4 : DIRA1
	PB5 : DIRB1
	
	OC0B : PWM2
	PD2 : DIRA2
	PD3 : DIRB2
	
	PD4 : スイッチ
	
	
	自動機2専用
	
	PB0 : スイッチ
	OC1A : サーボモータ用PWM
	PB2, PB3 : モータドライバ
	
	PWMは20kHzまで
*/

typedef enum
{
	JL_RIGHT,
	JL_LEFT
} JL_DIRECTION;

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
	ロータリーエンコーダーの値を取得
*/
uint8_t rotaryGet(JL_DIRECTION dir);

#endif