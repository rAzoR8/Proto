#pragma once

// forward decls
#include "spvgentwo/Type.h"

namespace proto
{
	class FundamentalTypeComboBox
	{
	public:
		FundamentalTypeComboBox(spvgentwo::IAllocator* _pAlloc, const char* _pTitle);
		~FundamentalTypeComboBox();

		void update();

		const spvgentwo::Type& getType() const { return m_type; }

	private:
		const char* m_pTitle = nullptr;
		spvgentwo::Type m_type;
		unsigned int m_selected = 0;
	};
} // !proto