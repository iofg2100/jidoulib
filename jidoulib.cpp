
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

void motorSetDuty(uint8_t leftRatio, uint8_t rightRatio)
{
	OCR0A = rightRatio;
	OCR0B = leftRatio;
}

void motorSetState(JLMotorState leftState, JLMotorState rightState)
{
	PORTB = setBits<2, 4>(PORTB, rightState);
	PORTD = setBits<2, 2>(PORTD, leftState);
}

void motorEnable()
{
	TCCR0A |= 0b10100000;	// PWMを出力
}

void motorDisable()
{
	TCCR0A &= 0b00001111;	// PWMを出力しない
}

void servoInit()
{
	// 高速PWM 10bit 分周256
	// 12.8us per count
	TCCR1A = 0b00000011;
	TCCR1B = 0b00001100;
	
	TCNT1H = 0;
	TCNT1L = 0;
	OCR1AH = 0;
	OCR1AL = 0;
	OCR1BH = 0;
	OCR1BL = 0;
}

void servoEnable(uint8_t index)
{
	switch (index)
	{
		case 0:
			TCCR1A = setBits<2, 6>(TCCR1A, 0b10);
			break;
		case 1:
			TCCR1A = setBits<2, 4>(TCCR1A, 0b10);
			break;
		default:
			break;
	}
}

void servoDisable(uint8_t index)
{
	switch (index)
	{
		case 0:
			TCCR1A = setBits<2, 6>(TCCR1A, 0b00);
			break;
		case 1:
			TCCR1A = setBits<2, 4>(TCCR1A, 0b00);
			break;
		default:
			break;
	}
}

void servoSetPhase(uint8_t index, unsigned phase)
{
	switch (index)
	{
		case 0:
			OCR1AL = phase;
			OCR1AH = phase >> 8;
			break;
		case 1:
			OCR1BL = phase;
			OCR1BH = phase >> 8;
			break;
		default:
			break;
	}
}

void servoSetPulseWidth(uint8_t index, unsigned us)
{
	servoSetPhase(index, us * 5 / 64);
}

void delayMs(unsigned ms)
{
	while (ms--)
		_delay_ms(1);
}

