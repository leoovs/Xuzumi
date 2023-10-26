/**
 * @file Xuzumi/Messaging/Event.hpp
 * 
 * @brief Defines an event interface.
 */

#pragma once

#include "Xuzumi/TypeMeta/TypeInfo.hpp"

namespace Xuzumi
{ 
	/**
	 * @brief Event interface. 
	 *
	 * Typically, the interface should not be implemented directly but via special
	 * type `Event<T>`.
	 * 
	 * @see Xuzumi::Event
	 */
	class IEvent
	{
	public:
		/**
		 * @brief Virtual destructor enforces valid RAII. 
		 */
		virtual ~IEvent() = default;

		/**
		 * @brief Retrieves `*this` type information.
		 *
		 * @return A `TypeInfo` instance containing valid type information.
		 */
		virtual TypeInfo GetTypeInfo() const = 0;
	};

	/**
	 * @brief `IEvent` implementation helper class.
	 * 
	 * Implements type information method required by `IEvent` interface based on
	 * the provided @p DerivedT type.
	 * 
	 * @tparam DerivedT The derived event type.
	 */
	template<typename DerivedT>
	class Event : public IEvent
	{
	public:
		/**
		 * @brief Retrieves @p DerivedT type information.
		 *
		 * @return A `TypeInfo` instance containing valid type information.
		 */
		TypeInfo GetTypeInfo() const override
		{
			return TypeInfo::Get<DerivedT>();
		}
	};
}
