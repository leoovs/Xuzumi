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

		// TODO: replace with some sort of string builder.
		return typeInfoStructName + "{ "
			+ "ID = " + std::to_string(ID) + ", "
			+ "Name = '" + Name + "', "
			+ "Size = " + std::to_string(Size) + ", "
			+ "Alignment = " + std::to_string(Alignment)
			+ " }";
	}

	void TypeInfo::Destruct(void* object) const
	{
		if (mDestructor && nullptr != object)
		{
			mDestructor(object);
		}
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
