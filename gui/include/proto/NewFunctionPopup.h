#pragma once

#include "TypeWidgets.h"
#include "common/HeapList.h"
#include "common/HeapString.h"

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
		NewFunctionPopup();
		~NewFunctionPopup();

		void update(spvgentwo::Module& _module);

		void show(bool _visible) {m_visble = _visible;};

	private:
		spvgentwo::String m_name;
		FundamentalTypeComboBox m_returnType;
		spvgentwo::HeapList<FundamentalTypeComboBox> m_parameterTypes;
		bool m_visble = false;
	};
}