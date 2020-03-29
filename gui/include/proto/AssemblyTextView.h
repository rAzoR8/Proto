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

		void update(spvgentwo::Module& _module);
	private:
		spvgentwo::HeapVector<unsigned int> m_binary;
		bool m_indent = true;
		bool m_offsets = false;
		bool m_friendlyNames = true;
	};
} // !proto