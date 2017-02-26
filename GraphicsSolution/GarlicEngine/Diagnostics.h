#pragma once
#include <stdio.h>

class Diagnostics
{
public:
	Diagnostics();
	~Diagnostics();
};

inline void EmitLocation(const int i_Line, const char *i_pFile);
inline void EmitFunctionName(const char *i_pName);
inline void EmitSystemInfo(const char *i_pName);
void ConsolePrint(const char *i_pName, ...);
void PrintToWindowsDebug(const char *i_pMsg);

#if defined(_DEBUG)
#define __GUARDBAND
#define DEBUG_LOCATION EmitLocation(__LINE__, __FILE__);
#define DEBUG_FUNCTION_NAME EmitFunctionName(__FUNCTION__);
#define DEBUG_SYSTEM_INFO EmitSystemInfo(__TIME__);
#define DEBUG_PRINT ConsolePrint
#define assert(cond) if((cond) == true) __debugbreak();
#else
#define DEBUG_LOCATION void(0);
#define DEBUG_FUNCTION_NAME void(0);
#define DEBUG_SYSTEM_INFO void(0);
#define DEBUG_PRINT(...) void(0);
#define assert(cond) void(0);
#endif

#include "Diagnostics.win32-inl.h"

