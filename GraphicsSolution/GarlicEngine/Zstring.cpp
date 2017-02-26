#include "Zstring.h"
#include <stdio.h>

Zstring::Zstring()
{
}


Zstring::~Zstring()
{
}


char * Zstrcpy(char *i_pDest, const char *i_pSrc) {
	char *orig = i_pDest;
	while ((*i_pDest++ = *i_pSrc++) != '\0');
	return orig;
}

char * Zstrcat(char *i_pDest, const char *i_pSrc)
{
	char *rdest = i_pDest;

	while (*i_pDest)
		i_pDest++;
	while (*i_pDest++ = *i_pSrc++)
		;
	return rdest;
}

size_t Zstrlen(const char * i_pStr[]) {
	const char **n;
	n = i_pStr;

	size_t len = 0;

	while (*n != NULL) {
		*n++;
		len++;
	}
	return len;
}

size_t Zstrlength(const char * i_pStr) {
	const char *n;
	n = i_pStr;

	size_t len = 0;

	while (n != NULL) {
		n++;
		len++;
	}
	return len;
}


