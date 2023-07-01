#ifndef XUZUMI_CORE_DELEGATE_HPP_
#define XUZUMI_CORE_DELEGATE_HPP_

#include <Xuzumi/Precompiled.hpp>

namespace Xuzumi::Internal
{
	template<typename ReturnT, typename... ArgsT>
	struct DelegateTraits
	{
		using ReturnType = ReturnT;
		using FunctionPtr = ReturnT(*)(ArgsT...);
		template<typename ClassT>
		using MethodPtr = ReturnT(ClassT::*)(ArgsT...);
	};

	template<typename ReturnT, typename... ArgsT>
	class IDelegateCaller
	{
	public:
		using Traits = DelegateTraits<ReturnT, ArgsT...>;
		using ReturnType = typename Traits::ReturnType;
		
		virtual ~IDelegateCaller() = default;
	
		virtual ReturnType Call(ArgsT&&... args) = 0;
	};

	template<typename ReturnT, typename... ArgsT>
	class DelegateFunctionCaller : public IDelegateCaller<ReturnT, ArgsT...>
	{
	public:
		using typename IDelegateCaller<ReturnT, ArgsT...>::Traits;
		using ReturnType = typename Traits::ReturnType;
		using FunctionPtr = typename Traits::FunctionPtr;

		DelegateFunctionCaller(FunctionPtr function)
			: m_Function{ function }
		{
		}

		ReturnType Call(ArgsT&&... args) override
		{
			return m_Function(std::forward<ArgsT>(args)...);
		}

	private:
		FunctionPtr m_Function{ nullptr };
	};

	template<typename ClassT, typename ReturnT, typename... ArgsT>
	class DelegateMethodCaller : public IDelegateCaller<ReturnT, ArgsT...>
	{
	public:
		using typename IDelegateCaller<ReturnT, ArgsT...>::Traits;
		using ReturnType = typename Traits::ReturnType;
		using MethodPtr = typename Traits::template MethodPtr<ClassT>;
		using ClassPtr = ClassT*;

		DelegateMethodCaller(ClassPtr classInstance, MethodPtr method)
			: m_ClassInstance{ classInstance }
			, m_Method{ method }
		{
		}

		ReturnType Call(ArgsT&&... args) override
		{
			return (m_ClassInstance->*m_Method)(std::forward<ArgsT>(args)...);
		}

	private:
		ClassPtr m_ClassInstance{ nullptr };
		MethodPtr m_Method{ nullptr };
	};
}

namespace Xuzumi
{
	template<typename>
	class Delegate;

	/**
	 * @brief Delegate type for sharing code and data.
	 * 
	 * @tparam ReturnT A return value type.
	 * @tparam ArgsT Argument types.
	 */
	template<typename ReturnT, typename... ArgsT>
	class Delegate<ReturnT(ArgsT...)>
	{
	private:
		using Traits = Internal::DelegateTraits<ReturnT, ArgsT...>;

	public:
		using ReturnType = typename Traits::ReturnType;
		using FunctionPtr = typename Traits::FunctionPtr;
		template<typename ClassT>
		using MethodPtr = typename Traits::MethodPtr<ClassT>;

		/**
		 * @brief Constructs an empty delegate.
		 */
		Delegate() = default;

		/**
		 * @brief Constructs a delegate that references
		 * non-member function pointer.
		 * 
		 * @param function A pointer to the function that delegate should invoke.
		 */
		Delegate(FunctionPtr function)
			: m_Caller
			{
				std::make_shared<
					Internal::DelegateFunctionCaller<ReturnType, ArgsT...>
				>(function)
			}
		{
		}

		/**
		 * @brief Constructs a delegate that references a class instance
		 * and its member function (method).
		 * 
		 * @tparam ClassT A type of the instance.
		 * 
		 * @param classInstance A pointer to the instance of ClassT.
		 * @param method A pointer to the member function that delegate
		 * should invoke.
		 */
		template<typename ClassT>
		Delegate(ClassT* classInstance, MethodPtr<ClassT> method)
			: m_Caller
			{
				std::make_shared<
					Internal::DelegateMethodCaller<ClassT, ReturnType, ArgsT...>
				>(classInstance, method)
			}
		{
		}

		/**
		 * @brief Checks if delegate is bound to function or method.
		 * 
		 * @return true if delegate is bound, otherwise false.
		 */
		explicit operator bool() const
		{
			return bool(m_Caller);
		}

		/**
		 * @brief Invokes the underlying bound function or method.
		 */
		ReturnType operator()(ArgsT... args) const
		{
			return m_Caller->Call(std::forward<ArgsT>(args)...);
		}

	private:
		using CallerType = Internal::IDelegateCaller<ReturnType, ArgsT...>;

		std::shared_ptr<CallerType> m_Caller;
	};
}

#endif // XUZUMI_CORE_DELEGATE_HPP_
