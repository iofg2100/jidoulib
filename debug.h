#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>
#include "jidoulib.h"

#define JL_EOF	0x1A

class Debug
{
public:
	static int putChar(char c)
	{
		USART::put(c);
		return c;
	}
	
	static int putString(const char *str);
	
	static int printf (
	const char* str,	/* Pointer to the format string */
	...					/* Optional arguments... */
	);
};

#endif
