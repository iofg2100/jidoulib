
#include <avr/io.h>
#include "jidoulib.h"

template <int Width, int Shift>
uint8_t setBits(uint8_t dst, uint8_t src)
{
	uint8_t result = 0;
	for (int i = 0; i < Width; ++i)
	{
		result |= (1 << i);
	}
	result <<= Shift;
	result = ~result;
	
	result &= dst;
	result |= src << Shift;
	return result;
}


void jidou1Init()
{
}

void jidou2Init()
{
}

void usartInit()
{
	UBRR0L = 25;
	UBRR0H = 0;
	UCSR0C |= (1 << UCSZ01) + (1 << UCSZ00);
	UCSR0B |= (1 << RXEN0) + (1 << TXEN0);
}

void usartPut(uint8_t data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void adConversionInit()
{
	ADMUX = 0b01100000;
	ADCSRA = 0b10000111;	// ADCイネーブル、プリスケーラ設定
	ADCSRB = 0;
}

static void adConversionSetPin(uint8_t pin)
{
	ADMUX = setBits<4, 0>(ADMUX, pin);
}

uint8_t adConversionGet(uint8_t pin)
{
	adConversionSetPin(pin);
	
	ADCSRA |= (1 << ADIF) + (1 << ADSC);
	
	while (ADCSRA & (1 << ADIF));
	
	return ADCH;
}

/**
	ロータリーエンコーダーの割り込み用
*/
void rotaryOnSignalChanged()
{
	
}

uint8_t rotaryGet(JL_DIRECTION dir)
{
	uint8_t result;
	
	switch (dir)
	{
		case JL_RIGHT:
			result = PINB & 0b11;
		case JL_LEFT:
			result = PINB & 0b1100 >> 2;
		default:
			return 0;
	}
	
	result |= 0b11;
	return result;
}

void motorInit()
{
	// タイマ0の初期化
	TCCR0B = 0b101;	// クロック/1024Hz
	TCNT0 = 0;	// カウンタ初期化
	TCCR0A = 0b10100011; // OCO*を使用;
	
	OCR0A = 0;
	OCR0B = 0;
}

void motorSetDuty(JL_DIRECTION dir, uint8_t ratio)
{
	switch (dir)
	{
		case JL_RIGHT:
			OCR0A = ratio;
			break;
		case JL_LEFT:
			OCR0B = ratio;
			break;
		default:
			break;
	}
}

void motorSetState(JL_DIRECTION dir, JL_MOTOR_STATE state)
{
	switch (dir)
	{
		case JL_RIGHT:
			PORTB = setBits<2, 4>(PORTB, state);
			break;
		case JL_LEFT:
			PORTD = setBits<2, 2>(PORTD, state);
			break;
		default:
			return;
	}
}

void servoInit()
{
	// 高速PWM 8bit
	TCCR1A = 0b10000001;
	TCCR1B = 0b00001101;
	
	TCNT1H = 0;
	TCNT1L = 0;
	OCR1AL = 0;
}

void servoSetPhase(uint8_t phase)
{
	OCR1AL = phase;
}

