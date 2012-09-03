
#include "jidoulib.h"
#include "debug.h"


int debugPutString(const char *str)
{
	while (*str)
		debugPutChar(*str++);
	return 0;
}

int debugPrintf (
	const char* str,	/* Pointer to the format string */
	...					/* Optional arguments... */
)
{
	va_list arp;
	uint8_t c, f, r;
	uint32_t val;
	char s[16];
	int i, w, res, cc;


	va_start(arp, str);

	for (cc = res = 0; cc != JL_EOF; res += cc) {
		c = *str++;
		if (c == 0) break;			/* End of string */
		if (c != '%') {				/* Non escape cahracter */
			cc = debugPutChar(c);
			if (cc != JL_EOF) cc = 1;
			continue;
		}
		w = f = 0;
		c = *str++;
		if (c == '0') {				/* Flag: '0' padding */
			f = 1; c = *str++;
		}
		while (c >= '0' && c <= '9') {	/* Precision */
			w = w * 10 + (c - '0');
			c = *str++;
		}
		if (c == 'l') {				/* Prefix: Size is long int */
			f |= 2; c = *str++;
		}
		if (c == 's') {				/* Type is string */
			cc = debugPutString(va_arg(arp, char*));
			continue;
		}
		if (c == 'c') {				/* Type is character */
			cc = debugPutChar(va_arg(arp, int));
			if (cc != JL_EOF) cc = 1;
			continue;
		}
		r = 0;
		if (c == 'd') r = 10;		/* Type is signed decimal */
		if (c == 'u') r = 10;		/* Type is unsigned decimal */
		if (c == 'X') r = 16;		/* Type is unsigned hexdecimal */
		if (r == 0) break;			/* Unknown type */
		if (f & 2) {				/* Get the value */
			val = (uint32_t)va_arg(arp, long);
		} else {
			val = (c == 'd') ? (uint32_t)(long)va_arg(arp, int) : (uint32_t)va_arg(arp, unsigned int);
		}
		/* Put numeral string */
		if (c == 'd') {
			if (val & 0x80000000) {
				val = 0 - val;
				f |= 4;
			}
		}
		i = sizeof(s) - 1; s[i] = 0;
		do {
			c = (uint8_t)(val % r + '0');
			if (c > '9') c += 7;
			s[--i] = c;
			val /= r;
		} while (i && val);
		if (i && (f & 4)) s[--i] = '-';
		w = sizeof(s) - 1 - w;
		while (i && i > w) s[--i] = (f & 1) ? '0' : ' ';
		cc = debugPutString(&s[i]);
	}
	
	va_end(arp);
	return (cc == JL_EOF) ? cc : res;
}
