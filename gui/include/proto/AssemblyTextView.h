#pragma once

#include "spvgentwo/Grammar.h"
#include "spvgentwo/String.h"

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
		AssemblyTextView(spvgentwo::IAllocator* _pAllocator);
		~AssemblyTextView();

		void update(spvgentwo::Module& _module);
	private:
		spvgentwo::IAllocator* m_pAllocator = nullptr;
		spvgentwo::Grammar m_grammar;
		spvgentwo::String m_text;
		//bool m_indent = true;
		//bool m_offsets = false;
		//bool m_friendlyNames = true;
		bool m_autoScroll = false;
	};
} // !proto