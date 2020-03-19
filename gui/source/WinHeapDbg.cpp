#include "proto/WinHeapDbg.h"

#ifdef _WIN32

	#ifndef _CRTDBG_MAP_ALLOC
		#define _CRTDBG_MAP_ALLOC
	#endif

	#include <crtdbg.h>

#endif

void proto::WinHeapDbg::init()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
}

void proto::WinHeapDbg::setBreakpoint(int _alloc)
{
#ifdef _WIN32
	_CrtSetBreakAlloc(_alloc);
#endif
}
