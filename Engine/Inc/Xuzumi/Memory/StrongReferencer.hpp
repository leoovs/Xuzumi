#pragma once

#include "Xuzumi/Memory/ControlBlock.hpp"

namespace Xuzumi::Internal
{
	class StrongReferencer
	{
	public:
		StrongReferencer() = default;

		StrongReferencer(const StrongReferencer& other);
		StrongReferencer(StrongReferencer&& other) noexcept;

		~StrongReferencer();

		StrongReferencer& operator=(const StrongReferencer& other);
		StrongReferencer& operator=(StrongReferencer&& other) noexcept;

		std::uint32_t UseCount() const;
		TypeInfo GetResourceTypeInfo() const;

		void Reset();

		template<typename ControlBlockT, typename... ArgsT>
		void Bind(ArgsT&&...args)
		{
			DecrementRefs();
			mControlBlock = new ControlBlockT(std::forward<ArgsT>(args)...);	
		}

	private:
		void IncrementRefs();
		void DecrementRefs();

		IControlBlock* mControlBlock = nullptr;	
	};
}
