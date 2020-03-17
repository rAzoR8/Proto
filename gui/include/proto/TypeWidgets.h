#pragma once

// forward decls
#include "spvgentwo/Type.h"
#include "ComboBox.h"

namespace proto
{
	class FundamentalTypeComboBox : public ComboBox
	{
	public:
		FundamentalTypeComboBox(spvgentwo::IAllocator* _pAlloc, const char* _pTitle);
		~FundamentalTypeComboBox();

		const spvgentwo::Type& getType() const { return m_type; }

	private:
		void onSelect(unsigned int _index) final;

	private:
		spvgentwo::Type m_type;
		unsigned int m_selected = 0;
	};
} // !proto