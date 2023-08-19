#pragma once

#include "Xuzumi/Memory/ControlBlock.hpp"

namespace Xuzumi::Internal
{
	enum class ReferencerMode
	{
		Strong,
		Weak
	};

	class Referencer
	{
	public:
		Referencer() = default;

		Referencer(const Referencer& other);
		Referencer(Referencer&& other) noexcept;

		~Referencer();

		Referencer& operator=(const Referencer& other);
		Referencer& operator=(Referencer&& other) noexcept;

		std::uint32_t UseCount() const;
		TypeInfo GetResourceTypeInfo() const;

		void Reset();
		void Weakify();
		void Strongify();
		
		template<typename ControlBlockT, typename... ArgsT>
		void Bind(ArgsT&&... args)
		{
			Reset();
			mControlBlock = new ControlBlockT{ std::forward<ArgsT>(args)... };
			mMode = ReferencerMode::Strong;
		}

	private:
		void IncrementRefs();
		void DecrementRefs();

		void SetMode(ReferencerMode mode);

		IControlBlock* mControlBlock = nullptr;
		ReferencerMode mMode = ReferencerMode::Strong;
	};
}
