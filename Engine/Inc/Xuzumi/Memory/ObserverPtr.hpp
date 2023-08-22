#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/PointerBase.hpp"

namespace Xuzumi
{
	template<typename T>
	class ObserverPtr
	{
	public:
		using Traits = Internal::PointerTraits<T>;

		using ReferenceType = typename Traits::ReferenceType;
		using PointerType = typename Traits::PointerType;

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
