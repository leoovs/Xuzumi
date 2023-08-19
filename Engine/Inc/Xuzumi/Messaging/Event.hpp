#pragma once

#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"

namespace Xuzumi
{
	class EventBase
	{
	public:
		virtual ~EventBase() = default;

		virtual TypeID GetTypeID() const = 0;
		virtual std::string GetTypeName() const = 0;
	};

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
