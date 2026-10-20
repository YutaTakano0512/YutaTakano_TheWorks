//#pragma once

//---------------------------------------------------------------------------
// debugPrintf.h
// 
// sougo hara  2024/04/24
//---------------------------------------------------------------------------
#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_

#include "WindowsErrorEscape.h"
#include <stdio.h>

inline void DebugPrintf(const char* pFormat, ...)
{
#if defined(_DEBUG) || defined(DEBUG)
	va_list argp;
	char buf[256];
	va_start(argp, pFormat);
	vsprintf_s(buf, 256, pFormat, argp);
	va_end(argp);
	OutputDebugString(buf);
#endif
}

#endif