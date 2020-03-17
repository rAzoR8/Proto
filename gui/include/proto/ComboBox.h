#pragma once

#include "spvgentwo/Vector.h"
#include "spvgentwo/String.h"
#include "Callable.h"

namespace proto
{
	class ComboBox : public spvgentwo::Vector<spvgentwo::String>
	{
	public:
		using OnSelectCallback = Callable<void(unsigned int)>;

		ComboBox(spvgentwo::IAllocator* _pAlloc = nullptr, const char* _pTitle = nullptr);
		~ComboBox();

		void update();

		template <class Functor>
		void setOnSelectCallback(const Functor& _func);

		// if callback is not called onSelect will be called
		virtual void onSelect(unsigned int _index) {};

	private:
		const char* m_pTitle = nullptr;
		// selection index, selected item
		OnSelectCallback m_onSelect;
		unsigned int m_selected = 0u;
	};

	template<class Functor>
	inline void ComboBox::setOnSelectCallback(const Functor& _func)
	{
		m_onSelect = _func;
	}
} // !proto