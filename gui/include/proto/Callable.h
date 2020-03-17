#pragma once

#include "spvgentwo/Allocator.h"

namespace proto
{
	template <typename T>
	class Callable;

	template <typename ReturnValue, typename... Args>
	class Callable<ReturnValue(Args...)>
	{
		struct ICallable
		{
			virtual ~ICallable() = default;
			virtual ReturnValue invoke(Args...) = 0;
			virtual ICallable* copy(spvgentwo::IAllocator* _pAllocator) const = 0;
		};

		template <typename Functor>
		struct CallableImpl : public ICallable
		{
			CallableImpl(const Functor& t) : m_func(t) {}
			CallableImpl(Functor&& t) : m_func(spvgentwo::stdrep::move(t)) {}
			CallableImpl(const CallableImpl& _other) : m_func(_other.m_func) {}
			CallableImpl(CallableImpl&& _other) : m_func(spvgentwo::stdrep::move(_other.m_func)) {}

			~CallableImpl() override = default;

			ReturnValue invoke(Args... args) final
			{
				return m_func(spvgentwo::stdrep::forward<Args>(args)...);
			}

			ICallable* copy(spvgentwo::IAllocator* _pAllocator) const final
			{
				return _pAllocator->construct<CallableImpl<Functor>>(m_func);
			}

		private:
			Functor m_func;
		};

	public:

		Callable(spvgentwo::IAllocator* _pAllocator = nullptr) : 
			m_pAllocator(_pAllocator)
		{}

		template <typename Functor>
		Callable(spvgentwo::IAllocator* _pAllocator, const Functor& f) :
			m_pAllocator(_pAllocator),
			m_pCallable(_pAllocator->construct<CallableImpl<Functor>>(f))
		{}

		template <typename Functor>
		Callable(spvgentwo::IAllocator* _pAllocator, Functor&& f) :
			m_pAllocator(_pAllocator),
			m_pCallable(_pAllocator->construct<CallableImpl<Functor>>(spvgentwo::stdrep::move(f)))
		{}

		Callable(const Callable& _other) :
			m_pAllocator(_other.m_pAllocator),
			m_pCallable(_other.m_pCallable->copy(_other.m_pAllocator))
		{}

		Callable(Callable&& _other) :
			m_pAllocator(_other.m_pAllocator),
			m_pCallable(_other.m_pCallable)
		{	
			_other.m_pAllocator = nullptr;
			_other.m_pCallable = nullptr;
		}

		template <typename Functor>
		Callable& operator=(const Functor& f)
		{
			reset();
			m_pCallable = m_pAllocator->construct<CallableImpl<Functor>>(f);
			return *this;
		}

		template <typename Functor>
		Callable& operator=(Functor&& f)
		{
			reset();
			m_pCallable = m_pAllocator->construct<CallableImpl<Functor>>(spvgentwo::stdrep::move(f));
			return *this;
		}

		Callable& operator=(const Callable& _other)
		{
			reset();
			m_pAllocator = _other.m_pAllocator;
			m_pCallable = _other.m_pCallable->copy(m_pAllocator);
			return *this;
		}

		Callable& operator=(Callable&& _other)
		{
			reset();
			m_pAllocator = _other.m_pAllocator;
			m_pCallable = _other.m_pCallable;
			_other.m_pAllocator = nullptr;
			_other.m_pCallable = nullptr;
			return *this;
		}

		ReturnValue operator()(Args... args) const
		{
			return m_pCallable->invoke(spvgentwo::stdrep::forward<Args>(args)...);
		}

		operator bool() const { return m_pCallable != nullptr; }

		void reset()
		{
			if (m_pAllocator != nullptr && m_pCallable != nullptr)
			{
				m_pAllocator->destruct(m_pCallable);
				m_pCallable = nullptr;
			}
		}

		~Callable()
		{
			reset();
		}

	private:

		spvgentwo::IAllocator* m_pAllocator = nullptr;
		ICallable* m_pCallable = nullptr;
	};
} //! proto