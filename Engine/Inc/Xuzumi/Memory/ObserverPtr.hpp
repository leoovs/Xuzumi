#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	template<typename T>
	class ObserverPtr
	{
	public:
		static_assert(
			!std::is_reference_v<T>,
			"ObserverPtr<T&> is not allowed"
		);

		using ValueType = T;
		using ReferenceType = std::add_lvalue_reference_t<ValueType>;
		using PointerType = std::add_pointer_t<ValueType>;

		ObserverPtr() = default;
		
		ObserverPtr(std::nullptr_t)
			: ObserverPtr()
		{
		}

		template<typename OtherT>
		ObserverPtr(const ObserverPtr<OtherT>& other)
			: mWatchedObject(other.Get())
		{
		}

		explicit ObserverPtr(PointerType watchedObject)
			: mWatchedObject(watchedObject)
		{
		}

		ObserverPtr(const ObserverPtr& other) = default;
		ObserverPtr(ObserverPtr&& other) noexcept = default;

		ObserverPtr& operator=(const ObserverPtr& other) = default;
		ObserverPtr& operator=(ObserverPtr&& other) noexcept = default;

		PointerType Release()
		{
			return std::exchange(mWatchedObject, nullptr);
		}
		
		void Reset(PointerType watchedObject = nullptr)
		{
			mWatchedObject = watchedObject;
		}

		PointerType Get() const
		{
			return mWatchedObject;
		}

		explicit operator bool() const
		{
			return bool(mWatchedObject);
		}

		ReferenceType operator*() const
		{
			// TODO: check if mWatchedObject is nullptr.

			return *mWatchedObject;
		}

		PointerType operator->() const
		{
			return mWatchedObject;
		}

	private:
		PointerType mWatchedObject = nullptr;
	};
}
