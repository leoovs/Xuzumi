#include "Xuzumi/TypeInfo/TypeID.hpp"

namespace Xuzumi::Internal
{
	TypeID GenerateNextTypeID()
	{
		static TypeID s_CurrentTypeID;
		return s_CurrentTypeID++;
	}
}
