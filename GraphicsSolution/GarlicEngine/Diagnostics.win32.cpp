#include "Diagnostics.h"
#include "Zstring.h"

#include <windows.h>

#define STR2(X) #X
#define STR(X) STR2(X)

Diagnostics::Diagnostics()
{
}


Diagnostics::~Diagnostics()
{
}


void ConsolePrint(const char *i_pFmt, ...) {
	const int MESSAGE_BUFFER_SIZE = 100;
	 char msgbuff[MESSAGE_BUFFER_SIZE];
	 va_list args;
	 va_start(args, i_pFmt);
	 vsprintf_s(msgbuff, i_pFmt, args);
	 va_end(args);
	 OutputDebugStringA(msgbuff);
 }

void PrintToWindowsDebug(const char *i_pMsg) {
	OutputDebugStringA(i_pMsg);
}
