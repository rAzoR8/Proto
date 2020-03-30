#pragma once

// forward decls
#include "spvgentwo/Type.h"
#include "ComboBox.h"

namespace proto
{
	class FundamentalTypeComboBox : public ComboBox<>
	{
	public:
		FundamentalTypeComboBox(const char* _pTitle);
		~FundamentalTypeComboBox() override;

		const spvgentwo::Type& getType() const { return m_type; }

	private:
		void onSelect(unsigned int _index) final;

	private:
		spvgentwo::Type m_type;
	};
} // !proto