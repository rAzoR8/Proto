#pragma once

#include "common/HeapVector.h"

// forward delcs

namespace spvgentwo
{
	class Module;
}

namespace proto
{
	class AssemblyTextView
	{
	public:
		AssemblyTextView();
		~AssemblyTextView();

		void update(const spvgentwo::HeapVector<unsigned int>& _module);
	private:
		bool m_indent = true;
		bool m_offsets = false;
		bool m_friendlyNames = true;
		bool m_autoScroll = false;
	};
} // !proto