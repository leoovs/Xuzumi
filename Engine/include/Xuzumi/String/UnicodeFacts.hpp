#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	struct UnicodeFacts
	{
		static constexpr char32_t kReplacementCharacterCodePoint = 0xFFFD;
		static constexpr char32_t kWhiteSquareCharacterCodePoint = 0x25A1;
	
		static bool IsUnknownCharacter(char32_t character);
	};
}
