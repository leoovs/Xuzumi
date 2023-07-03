#ifndef XUZUMI_CORE_DELEGATE_HPP_
#define XUZUMI_CORE_DELEGATE_HPP_

#include "Xuzumi/Precompiled.hpp"

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
			: mFunction(function)
		{
		}

		ReturnType Call(ArgsT&&... args) override
		{
			return mFunction(std::forward<ArgsT>(args)...);
		}

	private:
		FunctionPtr mFunction = nullptr;
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
			: mClassInstance(classInstance)
			, mMethod(method)
		{
		}

		ReturnType Call(ArgsT&&... args) override
		{
			return (mClassInstance->*mMethod)(std::forward<ArgsT>(args)...);
		}

	private:
		ClassPtr mClassInstance = nullptr;
		MethodPtr mMethod = nullptr;
	};
}

namespace Xuzumi
{
	template<typename>
	class Delegate;

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

		Delegate() = default;

		Delegate(FunctionPtr function)
			: mCaller(
				std::make_shared<
					Internal::DelegateFunctionCaller<ReturnType, ArgsT...>
				>(function)
			)
		{
		}

		template<typename ClassT>
		Delegate(ClassT* classInstance, MethodPtr<ClassT> method)
			: mCaller(
				std::make_shared<
					Internal::DelegateMethodCaller<ClassT, ReturnType, ArgsT...>
				>(classInstance, method)
			)
		{
		}

		explicit operator bool() const
		{
			return bool(mCaller);
		}

		ReturnType operator()(ArgsT... args) const
		{
			return mCaller->Call(std::forward<ArgsT>(args)...);
		}

	private:
		using CallerType = Internal::IDelegateCaller<ReturnType, ArgsT...>;

		std::shared_ptr<CallerType> mCaller;
	};
}

#endif // XUZUMI_CORE_DELEGATE_HPP_
