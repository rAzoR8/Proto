#pragma once

#include "common/HeapString.h"
#include "common/HeapVector.h"
#include "common/HeapCallable.h"

namespace proto
{
	
	namespace detail
	{
		struct Empty {};
	}

	template <typename Data = detail::Empty>
	struct ComboBoxEntry
	{
		ComboBoxEntry(const char* _name = nullptr) :
			name(_name) {}

		ComboBoxEntry(const char* _name, const Data& _data) :
			name(_name), data(_data) {}

		const char* name;
		Data data;
	};

	template <typename X = detail::Empty>
	class ComboBox : public spvgentwo::HeapVector<ComboBoxEntry<X>>
	{
	public:
		using OnSelectCallback = spvgentwo::HeapCallable<void(unsigned int)>;

		ComboBox(ComboBox&& _other) noexcept;

		ComboBox(const ComboBox& _other);

		ComboBox(const char* _pTitle = nullptr);

		template <typename ...Args>
		ComboBox(const char* _pTitle, Args&& ... _args);

		virtual ~ComboBox() override;

		void update();

		template <class Functor>
		void setOnSelectCallback(const Functor& _func);

		// if callback is not called onSelect will be called
		virtual void onSelect(unsigned int _index) {};

		int getSelectedIndex() const { return m_selected; }
		void setSelectedIndex(int _index) { m_selected = _index; }

		const ComboBoxEntry<X>& getSelected() const { return this->operator[](m_selected < 0 ? 0 : m_selected); }
		const X& getSelectedItem() const { return this->operator[](m_selected < 0 ? 0 : m_selected).data; }

		template <class ...Args>
		void emplace_back_args2(const char* _name, const X& _data, Args&& ..._tail);

	private:
		const char* m_pTitle = nullptr;
		// selection index, selected item
		OnSelectCallback m_onSelect;
		int m_selected = -1;
	};

	template <typename X>
	void add(ComboBox<X>* ptr, const char* _name = nullptr, const X& _data = {})
	{
		ptr->emplace_back(ComboBoxEntry{ _name, _data });
	}

	template<typename X>
	template<typename ...Args>
	inline ComboBox<X>::ComboBox(const char* _pTitle, Args&& ..._args) :
		m_pTitle(_pTitle)
	{
		emplace_back_args2(stdrep::forward<Args>(_args)...);
	}

	template<typename X>
	template<class Functor>
	inline void ComboBox<X>::setOnSelectCallback(const Functor& _func)
	{
		m_onSelect = _func;
	}

	template<typename X>
	template<class ...Args>
	inline void ComboBox<X>::emplace_back_args2(const char* _name, const X& _data, Args&& ..._tail) 
	{
		this->emplace_back(_name, _data);

		if constexpr (sizeof...(_tail) > 1)
		{
			emplace_back_args2(stdrep::forward<Args>(_tail)...);
		}
	}

	template<typename X>
	ComboBox<X>::ComboBox(ComboBox&& _other) noexcept : spvgentwo::HeapVector<ComboBoxEntry<X>>(stdrep::move(_other)),
		m_pTitle(_other.m_pTitle),
		m_onSelect(stdrep::move(_other.m_onSelect)),
		m_selected(_other.m_selected)
	{
		_other.m_selected = 0u;
		_other.m_pTitle = nullptr;
	}

	template<typename X>
	ComboBox<X>::ComboBox(const ComboBox& _other) : spvgentwo::HeapVector<ComboBoxEntry<X>>(_other),
		m_pTitle(_other.m_pTitle),
		m_onSelect(_other.m_onSelect),
		m_selected(_other.m_selected)
	{
	}

	template<typename X>
	ComboBox<X>::ComboBox(const char* _pTitle) :
		spvgentwo::HeapVector<ComboBoxEntry<X>>(),
		m_pTitle(_pTitle),
		m_onSelect()
	{
	}

	template<typename X>
	ComboBox<X>::~ComboBox()
	{
	}

	template<typename X>
	void ComboBox<X>::update()
	{
		if (this->empty())
			return;

		if (m_selected == -1)
		{
			if (m_onSelect)
			{
				m_onSelect(0);
			}
			else
			{
				onSelect(0);
			}
		}

		m_selected = m_selected >= this->m_elements ? 0 : m_selected;
		const char* current_item = this->operator[](m_selected).name;

		if (ImGui::BeginCombo(m_pTitle, current_item))
		{
			for (int n = 0; n < this->m_elements; n++)
			{
				const char* item = this->operator[](n).name;

				bool is_selected = (current_item == item);

				if (ImGui::Selectable(item, is_selected /*,0, ImVec2(1.f, 2.f)*/))
				{
					m_selected = n;

					ImGui::SetItemDefaultFocus();
					if (m_onSelect)
					{
						m_onSelect(m_selected);
					}
					else
					{
						onSelect(m_selected);
					}
				}
			}
			ImGui::EndCombo();
		}
	}

} // !proto