#pragma once

#include "Xuzumi/TypeMeta/TypeID.hpp"
#include "Xuzumi/TypeMeta/TypeName.hpp"

namespace Xuzumi
{
	struct TypeInfo
	{
		TypeID ID = kInvalidTypeID;
		std::string Name;
	
		template<typename T>
		static TypeInfo Get()
		{
			return
			{
				GetTypeID<T>(),
				GetTypeName<T>()
			};
		}

		std::string ToString() const;
		explicit operator bool() const;

		bool operator==(const TypeInfo& other) const;
		bool operator!=(const TypeInfo& other) const;
	};	
}
