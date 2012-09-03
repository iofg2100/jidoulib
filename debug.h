#ifndef DEBUG_H
#define DEBUG_H

#include <stdarg.h>
#include "jidoulib.h"

#define JL_EOF	0x1A

inline int debugPutChar(char c)
{
	usartPut(c);
	return c;
}

int debugPutString(const char *str);

int debugPrintf (
	const char* str,	/* Pointer to the format string */
	...					/* Optional arguments... */
);

#endif
