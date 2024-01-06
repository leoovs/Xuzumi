#include "Xuzumi/String/Utf8Facts.hpp"

namespace Xuzumi::Internal
{
	std::size_t Utf8Facts::PickCodeUnitAmount(char32_t codePoint)
	{
		if (codePoint <= 0x0000007F)
		{
			return 1;
		}
		else if (codePoint <= 0x000007FF)	
		{
			return 2;
		}
		else if (codePoint <= 0x0000FFFF)
		{
			return 3;
		}
		else if (codePoint <= 0x0010FFFF)	
		{
			return 4;
		}
	
		return kInvalidCodeUnitAmount;
	}
}
