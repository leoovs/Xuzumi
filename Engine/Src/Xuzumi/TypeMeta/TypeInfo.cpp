#include "Xuzumi/TypeMeta/TypeInfo.hpp"

namespace Xuzumi
{
	std::string TypeInfo::ToString() const
	{
		std::string typeInfoStructName = GetTypeName<
			std::remove_const_t<
				std::remove_reference_t<decltype(*this)>
			>
		>();

		return typeInfoStructName + "{ "
			+ "ID = " + std::to_string(ID) + ", "
			+ "Name = '" + Name + "' }"; 
	}

	TypeInfo::operator bool() const
	{
		return kInvalidTypeID != ID;
	}

	bool TypeInfo::operator==(const TypeInfo& other) const
	{
		return ID == other.ID;
	}

	bool TypeInfo::operator!=(const TypeInfo& other) const
	{
		return ID != other.ID;
	}
}
