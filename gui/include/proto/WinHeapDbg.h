#pragma once

namespace proto
{
	class WinHeapDbg
	{
	public:
		static void init();
		static void setBreakpoint(int _alloc);
	};
} // !proto