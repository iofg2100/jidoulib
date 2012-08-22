#ifndef JIDOULIB_H
#define	JIDOULIB_H

#include <stdint.h>

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



#endif