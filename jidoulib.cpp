
#include "jidoulib.h"

template <uint8_t Width, uint8_t Shift>
uint8_t setBits(uint8_t dst, uint8_t src)
{
	// bit幅分のマスクを生成
	uint8_t result = (1 << Width) - 1;
	result <<= Shift;
	result = ~result;

	result &= dst;
	result |= src << Shift;
	return result;
}


void allInit()
{
	gpioInit();
	usartInit();
	adConversionInit();
	motorInit();
	servoInit();
}

void gpioInit()
{
	DDRB = 0b00110110;
	DDRD = 0b11101100;
	PORTB = 0;
	PORTD = 0;
}

void usartInit()
{
	// 19200 Hz
	UBRR0L = 64;
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
	
	while (!(ADCSRA & (1 << ADIF)));
	
	return ADCH;
}

void motorInit()
{
	// タイマ0の初期化
	TCCR0B = 0b101;	// クロック/1024Hz
	TCNT0 = 0;	// カウンタ初期化
	TCCR0A = 0b00000011;
	
	OCR0A = 0;
	OCR0B = 0;
}

void motorSetDuty(JLDirection dir, uint8_t ratio)
{
	switch (dir)
	{
		case JLRight:
			OCR0A = ratio;
			break;
		case JLLeft:
			OCR0B = ratio;
			break;
		default:
			break;
	}
}

void motorSetState(JLDirection dir, JLMotorState state)
{
	switch (dir)
	{
		case JLRight:
			PORTB = setBits<2, 4>(PORTB, state);
			break;
		case JLLeft:
			PORTD = setBits<2, 2>(PORTD, state);
			break;
		default:
			return;
	}
}

void motorStart()
{
	TCCR0A |= 0b10100000;	// PWMを出力
}

void motorEnd()
{
	TCCR0A &= 0b00001111;	// PWMを出力しない
}

void servoInit()
{
	// 高速PWM 10bit 分周256
	// 12.8us per count
	TCCR1A = 0b10100011;
	TCCR1B = 0b00001100;
	
	TCNT1H = 0;
	TCNT1L = 0;
	OCR1AH = 0;
	OCR1AL = 0;
	OCR1BH = 0;
	OCR1BL = 0;
}

void servoSetPhase(unsigned phase)
{
	OCR1AL = phase;
	OCR1BL = phase;
}

void servoSetPulseWidth(unsigned us)
{
	servoSetPhase(us * 5 / 64);
}

void delayMs(unsigned ms)
{
	while (ms--)
		_delay_ms(1);
}

