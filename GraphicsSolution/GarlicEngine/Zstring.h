#pragma once

class Zstring
{
public:
	Zstring();
	~Zstring();
};

char * Zstrcpy(char *i_pDest, const char *i_pSrc);
char * Zstrcat(char *i_pDest, const char *i_pSrc);
size_t Zstrlen(const char * i_pStr[]);
// TODO Need to test the implementation
size_t Zstrlength(const char * i_pStr);
