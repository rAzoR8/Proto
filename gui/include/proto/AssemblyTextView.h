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
	};
} // !proto