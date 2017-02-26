#pragma once
#include "Diagnostics.h"

inline void EmitSystemInfo(const char *i_pName)
{
	const int MESSAGE_BUFFER_SIZE = 100;
	char msgbuff[MESSAGE_BUFFER_SIZE];
	sprintf_s(msgbuff, "\nTime : %s", i_pName);
	PrintToWindowsDebug(msgbuff);
}

inline void EmitLocation(const int i_Line, const char *i_pFile)
{
	const int MESSAGE_BUFFER_SIZE = 100;
	char msgbuff[MESSAGE_BUFFER_SIZE];
	sprintf_s(msgbuff, "\nLine number is %d", i_Line);
	PrintToWindowsDebug(msgbuff);
	sprintf_s(msgbuff, "\nFile path is %s", i_pFile);
	PrintToWindowsDebug(msgbuff);
}

inline void EmitFunctionName(const char *i_pName)
{
	const int MESSAGE_BUFFER_SIZE = 100;
	char msgbuff[MESSAGE_BUFFER_SIZE];
	sprintf_s(msgbuff, "\nExecuting function %s", i_pName);
	PrintToWindowsDebug(msgbuff);
}