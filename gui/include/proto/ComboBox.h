#pragma once

#include "common/HeapString.h"
#include "common/HeapVector.h"
#include "common/HeapCallable.h"

namespace proto
{
	// todo: change to heaplist
	class ComboBox : public spvgentwo::HeapVector<spvgentwo::HeapString>
	{
	public:
		using OnSelectCallback = spvgentwo::HeapCallable<void(unsigned int)>;

		ComboBox(ComboBox&& _other) noexcept;

		ComboBox(const ComboBox& _other);

		ComboBox(const char* _pTitle = nullptr);

		template <typename ...Args>
		ComboBox(const char* _pTitle, const OnSelectCallback& _callback, const char* _firstElem, Args&& ... _args);

		template <typename ...Args>
		ComboBox(const char* _pTitle, const char* _firstElem, Args&& ... _args);

		virtual ~ComboBox() override;

		void update();

		template <class Functor>
		void setOnSelectCallback(const Functor& _func);

		// if callback is not called onSelect will be called
		virtual void onSelect(unsigned int _index) {};

	private:
		const char* m_pTitle = nullptr;
		// selection index, selected item
		OnSelectCallback m_onSelect;
		int m_selected = -1;
	};

	template<typename ...Args>
	inline ComboBox::ComboBox(const char* _pTitle, const OnSelectCallback& _callback, const char* _firstElem, Args&& ..._args) :
		HeapVector(_firstElem, spvgentwo::stdrep::forward<Args>(_args)...),
		m_onSelect(_callback),
		m_pTitle(_pTitle)
	{
	}

	template<typename ...Args>
	inline ComboBox::ComboBox(const char* _pTitle, const char* _firstElem, Args&& ..._args) :
		HeapVector(_firstElem, spvgentwo::stdrep::forward<Args>(_args)...),
		m_pTitle(_pTitle)
	{
	}

	template<class Functor>
	inline void ComboBox::setOnSelectCallback(const Functor& _func)
	{
		m_onSelect = _func;
	}
} // !proto