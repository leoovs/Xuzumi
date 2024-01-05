#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi::Internal
{
	struct Utf16Facts
	{
		static constexpr std::size_t kMaxCodeUnits = 2;
		static constexpr std::size_t kInvalidCodeUnitAmount = 0;

		static constexpr char16_t kHighSurrogateMask = 0xFC00;
		static constexpr char16_t kHighSurrogateSignature = 0xD800;
	
		static constexpr char16_t kLowSurrogateMask = kHighSurrogateMask;
		static constexpr char16_t kLowSurrogateSignature = 0xDC00;

		static constexpr std::size_t kSurrogateEncodedBitCount = 10;

		static constexpr std::uint32_t kCodePointSurrogatePairClip = 0x10000;
	};
}
