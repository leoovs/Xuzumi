/**
 * @file Xuzumi/Messaging/Event.hpp
 * 
 * @brief Defines an event interface.
 */

#pragma once

#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"

namespace Xuzumi
{ 
	// TODO: rename to IEvent; replace type introspection methods with TypeInfo.

	/**
	 * @brief Event interface. 
	 *
	 * Typically, the interface should not be implemented directly but via special
	 * type `Event<T>`.
	 * 
	 * @see Xuzumi::Event
	 */
	class EventBase
	{
	public:
		/**
		 * @brief Virtual destructor enforces valid RAII. 
		 */
		virtual ~EventBase() = default;

		virtual TypeID GetTypeID() const = 0;
		virtual std::string GetTypeName() const = 0;
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
	class Event : public EventBase
	{
	public:
		TypeID GetTypeID() const override
		{
			return Xuzumi::GetTypeID<DerivedT>();
		}

		std::string GetTypeName() const override
		{
			return Xuzumi::GetTypeName<DerivedT>();
		}
	};
}
