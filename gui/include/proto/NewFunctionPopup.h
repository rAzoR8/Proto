#pragma once

#include "spvgentwo/List.h"
#include "TypeWidgets.h"
#include "spvgentwo/String.h"

// forward delcs
namespace spvgentwo
{
	class Module;
}

namespace proto
{
	class NewFunctionPopup
	{
	public:
		NewFunctionPopup(spvgentwo::IAllocator* _pAllocator);
		~NewFunctionPopup();

		void update(spvgentwo::Module& _module);

		void show(bool _visible) {m_visble = _visible;};

	private:
		spvgentwo::IAllocator* m_pAllocator = nullptr;
		spvgentwo::String m_name;
		FundamentalTypeComboBox m_returnType;
		spvgentwo::List<FundamentalTypeComboBox> m_parameterTypes;
		bool m_visble = false;
	};
}