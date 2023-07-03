#ifndef XUZUMI_OWNERSHIP_SHARED_FACTORY_HPP_
#define XUZUMI_OWNERSHIP_SHARED_FACTORY_HPP_

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	template<typename FactoryT>
	class SharedFactory
	{
	public:
		using FactoryType = FactoryT;
		using FactoryPtr = FactoryType*;

		SharedFactory(FactoryPtr factoryThisPtr)
			: mSharedThisPtr(factoryThisPtr, [](FactoryPtr) {})
		{
		}

		SharedFactory(const SharedFactory& other) = delete;

		template<typename ResourceT>
		auto MakeDeleter(void(FactoryType::* deleter)(ResourceT*))
		{
			std::weak_ptr<FactoryType> factoryWeakRef(mSharedThisPtr);

			return [factoryWeakRef, deleter](ResourceT* resource)
			{
				if (!factoryWeakRef.expired())
				{
					FactoryPtr factory = factoryWeakRef.lock().get();
					(factory->*deleter)(resource);

					return;
				}

				// TODO: output debug information about dangling resource.
			};
		}

	private:
		std::shared_ptr<FactoryType> mSharedThisPtr;
	};
}

#endif // XUZUMI_OWNERSHIP_SHARED_FACTORY_HPP_
