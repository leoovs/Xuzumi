/**
 * @file Xuzumi/Core/Delegate.hpp
 * 
 * @brief Contains delegate implementation. 
*/

#pragma once

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

	template<typename FunctorT, typename ReturnT, typename... ArgsT>
	class DelegateFunctorCaller : public IDelegateCaller<ReturnT, ArgsT...>
	{
	public:
		using typename IDelegateCaller<ReturnT, ArgsT...>::Traits;
		using ReturnType = typename Traits::ReturnType;

		DelegateFunctorCaller(FunctorT functor)
			: mFunctor(functor)
		{
		}

		ReturnType Call(ArgsT&&... args) override
		{
			return mFunctor(std::forward<ArgsT>(args)...);
		}

	private:
		FunctorT mFunctor;
	};
}

namespace Xuzumi
{
	template<typename>
	class Delegate;

	/**
	 * @brief Implements delegate functionality to combine functions, functors,
	 * and methods into a unified call interface.
	 * 
	 * This class enables the consolidation of functions, functors, and methods
	 * into a single interface for invocation.
	 * 
	 * @tparam ReturnT The return value's data type.
	 * @tparam ArgsT The types of the arguments.
	 */
	template<typename ReturnT, typename... ArgsT>
	class Delegate<ReturnT(ArgsT...)>
	{
	private:
		using Traits = Internal::DelegateTraits<ReturnT, ArgsT...>;

	public:
		/**
		 * @typedef ReturnType
		 * 
		 * @brief Return value type alias. 
		 */
		using ReturnType = typename Traits::ReturnType;

		/**
		 * @typedef FunctionPtr
		 * 
		 * @brief Function pointer type alias. 
		 */
		using FunctionPtr = typename Traits::FunctionPtr;
		
		/**
		 * @typedef MethodPtr<ClassT>
		 * 
		 * @brief Method pointer type alias.
		 * 
		 * @tparam ClassT Class instance type.
		 */
		template<typename ClassT>
		using MethodPtr = typename Traits::template MethodPtr<ClassT>;

		/**
		 * @brief Default constructor.
		 * 
		 * Constructs empty delegate. 
		 */
		Delegate() = default;

		/**
		 * @brief Captures function pointer and constructs delegate.
		 * 
		 * @param function Function pointer.
		 */
		Delegate(FunctionPtr function)
			: mCaller(
				std::make_shared<
					Internal::DelegateFunctionCaller<ReturnType, ArgsT...>
				>(function)
			)
		{
		}

		/**
		 * @brief Captures class instance and method pointer and constructs
		 * delegate.
		 * 
		 * Internally delegate stores a raw pointer to the class instance which may
		 * become dangling. Delegate caller is responsible for pointer validation.
		 *
		 * @tparam ClassT Type of the @p classInstance. 
		 *
		 * @param classInstance Pointer to the class instance.
		 * @param method Method pointer.
		*/
		template<typename ClassT>
		Delegate(ClassT* classInstance, MethodPtr<ClassT> method)
			: mCaller(
				std::make_shared<
					Internal::DelegateMethodCaller<ClassT, ReturnType, ArgsT...>
				>(classInstance, method)
			)
		{
		}

		/**
		 * @brief Captures functor instance and constructs delegate.
		 * 
		 * Delegate stores its own copy of the functor.
		 * 
		 * @tparam FunctorT type of the functor.
		 * 
		 * @param functor Functor instance.
		 */
		template<typename FunctorT>
		Delegate(FunctorT functor)
			: mCaller(
				std::make_shared<
					Internal::DelegateFunctorCaller<FunctorT, ReturnType, ArgsT...>
				>(functor)
			)
		{
		}

		/**
		 * @brief Resets delegate so that it becomes empty.
		 * 
		 * Delegate that has been reset is the same as default-constructed delegate 
		 * and does not hold raw pointer to a function, a class instance or a method
		 * pointer.
		 */
		void Reset()
		{
			mCaller.reset();
		}

		/**
		 * @brief Checks whether delegate references some callable object.
		 * 
		 * @retval true Delegate references some callable object.
		 * @retval false Delegate is empty.
		*/
		explicit operator bool() const
		{
			return bool(mCaller);
		}

		/**
		 * @brief Invokes an underlying captured callable.
		 * 
		 * @param args Parameters passed to the underlying callable.
		 * 
		 * @return Returns the same value as the underlying callable.
		 */
		ReturnType operator()(ArgsT... args) const
		{
			return mCaller->Call(std::forward<ArgsT>(args)...);
		}

	private:
		using CallerType = Internal::IDelegateCaller<ReturnType, ArgsT...>;

		std::shared_ptr<CallerType> mCaller;
	};
}
