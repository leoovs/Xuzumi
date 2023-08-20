#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"
#include "Xuzumi/Debug/DebugSession.hpp"

namespace Xuzumi::Internal
{
	template<typename FactoryT>
	class ResourceDeleterGuard
	{
	public:
		static_assert(
			!std::is_reference_v<FactoryT>
			&& !std::is_array_v<FactoryT>
		);

		template<typename ResourceT>
		using ResourceRequirement = std::enable_if_t<
			!std::is_reference_v<ResourceT> && !std::is_array_v<ResourceT>
		>;

		ResourceDeleterGuard(FactoryT* factory)
			: mFactory(factory, [](FactoryT*) {})
			, mFactoryWeakRef(mFactory)
		{
		}

		ResourceDeleterGuard(const ResourceDeleterGuard& other) = delete;

		template<
			typename ResourceT,
			typename CallableT,
			typename = ResourceRequirement<ResourceT> 
		>
		auto MakeDangleProtectedDeleter(CallableT callable)
		{
			return [callable, factory = mFactoryWeakRef](ResourceT* res)
			{
				if (factory.expired())
				{
					XZ_LOG(
						Error,
						"Could not free '%s' instance at memory location 0x%p: "
						"parent '%s' instance has been destroyed before the deallocation "
						"could happen",
						GetTypeName<ResourceT>().data(),
						res,
						GetTypeName<FactoryT>().data()
					);
					return;
				}

				callable(res);	
			};	
		}

		template<
			typename ResourceT,
			typename = ResourceRequirement<ResourceT> 
		>
		auto MakeDangleProtectedDeleter(void(FactoryT::*method)(ResourceT*))
		{
			return MakeDangleProtectedDeleter<ResourceT>(
				[method, factory = mFactory.get()](ResourceT* res)
				{
					(factory->*method)(res);
				}	
			);
		}

	private:
		std::shared_ptr<FactoryT> mFactory;
		std::weak_ptr<FactoryT> mFactoryWeakRef;
	};
}
