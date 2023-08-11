#ifndef XUZUMI_TYPEMETA_TYPEINFO_HPP_
#define XUZUMI_TYPEMETA_TYPEINFO_HPP_

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

#endif // XUZUMI_TYPEMETA_TYPEINFO_HPP_
