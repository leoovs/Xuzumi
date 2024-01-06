#include "Xuzumi/String/UnicodeFacts.hpp"

namespace Xuzumi
{
	bool UnicodeFacts::IsUnknownCharacter(char32_t codePoint)
	{
		return codePoint == kReplacementCharacterCodePoint
			|| codePoint == kWhiteSquareCharacterCodePoint;
	}
}
