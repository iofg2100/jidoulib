#ifndef FIXED_H
#define FIXED_H

#include <stdint.h>

class Fixed16
{
public:
	
	enum
	{
		DecimalShift = 8,
		DecimalSize = 1 << DecimalShift
	};
	
	Fixed16() {}
	
	Fixed16(int value)
	{
		_d = value / DecimalSize;
	}
	
	Fixed16(float value)
	{
		_d = value * ( 1.f / float(DecimalSize));
	}
	
	Fixed16 operator+(const Fixed16 &x) const
	{
		return fromRaw(_d + x._d);
	}
	
	Fixed16 operator-(const Fixed16 &x) const
	{
		return fromRaw(_d - x._d);
	}
	
	Fixed16 operator*(const Fixed16 &x) const
	{
		return fromRaw(((int32_t)_d * (int32_t)x._d) >> (int32_t)DecimalShift);
	}
	
	Fixed16 operator/(const Fixed16 &x) const
	{
		return fromRaw(((int32_t)_d << (int32_t)DecimalShift) / (int32_t)x._d);
	}
	
	Fixed16 &operator+=(const Fixed16 &x)
	{
		return *this = *this + x;
	}
	
	Fixed16 &operator-=(const Fixed16 &x)
	{
		return *this = *this - x;
	}
	
	Fixed16 &operator*=(const Fixed16 &x)
	{
		return *this = *this * x;
	}
	
	Fixed16 &operator/=(const Fixed16 &x)
	{
		return *this = *this / x;
	}
	
	static Fixed16 fromRaw(int16_t raw)
	{
		Fixed16 r;
		r._d = raw;
		return r;
	}
	
	operator int() const
	{
		return _d >> 8;
	}
	
	operator float() const
	{
		return float(_d) * ( 1.f / float(DecimalSize));
	}
	
private:
	
	int16_t _d;
};

#endif
