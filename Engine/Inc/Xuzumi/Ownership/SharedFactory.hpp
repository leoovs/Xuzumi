/**
 * @file Xuzumi/Ownership/SharedFactory.hpp
 * 
 * @brief Defines the SharedFactory<T> class.
 */

#ifndef XUZUMI_OWNERSHIP_SHARED_FACTORY_HPP_
#define XUZUMI_OWNERSHIP_SHARED_FACTORY_HPP_

#include <Xuzumi/Precompiled.hpp>

namespace Xuzumi
{
	/**
	 * @brief Provides a resource smart pointer with a weak reference 
	 * to a factory so that the resource can be safely deleted.
	 * 
	 * @tparam FactoryT The type of the factory object.
	 */
	template<typename FactoryT>
	class SharedFactory
	{
	public:
		using FactoryType = FactoryT;
		using FactoryPtr = FactoryType*;

		/**
		 * @brief Constructs a SharedFactory object with a strong 
		 * reference to the factory itself.
		 */
		SharedFactory(FactoryPtr factoryThisPtr)
			: m_SharedThisPtr{ factoryThisPtr, [](FactoryPtr) {} }
		{
		}

		SharedFactory(const SharedFactory& other) = delete;

		/**
		 * @brief Creates custom deleter for shared pointer.
		 * 
		 * The deleter references deleter method pointer and
		 * ensures that the factory object is valid by checking
		 * weak reference.
		 * 
		 * @tparam ResourceT The type of the resource.
		 * 
		 * @param deleter A pointer to the member function that
		 * is responsible for resource cleanup.
		 * 
		 * @return A lambda function that serves as the custom deleter.
		 */
		template<typename ResourceT>
		auto MakeDeleter(void(FactoryType::* deleter)(ResourceT*))
		{
			std::weak_ptr<FactoryType> factoryWeakRef{ m_SharedThisPtr };

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
		std::shared_ptr<FactoryType> m_SharedThisPtr;
	};
}

#endif // XUZUMI_OWNERSHIP_SHARED_FACTORY_HPP_
