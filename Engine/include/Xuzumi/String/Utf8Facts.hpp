#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi::Internal
{
	struct Utf8Facts
	{
		static constexpr std::size_t kMaxCodeUnits = 4;
		static constexpr std::size_t kInvalidCodeUnitAmount = 0;
	
		static constexpr std::array<std::uint8_t, kMaxCodeUnits> kLeaderByteMasks
		{
			0x80,
			0xE0,
			0xF0,
			0xF8
		};

		static constexpr std::array<std::size_t, kMaxCodeUnits>
		kLeaderByteEncodedBitCount
		{
			7,
			5,
			4,
			3
		};
	
		static constexpr std::array<std::uint8_t, kMaxCodeUnits>
		kDecodedCharacterSizeSignatures
		{
			0x00,
			0xC0,
			0xE0,
			0xF0
		};

		static constexpr std::array<char32_t, kMaxCodeUnits> kCodePointRanges
		{
			0x0000007F,
			0x000007FF,
			0x0000FFFF,
			0x001FFFFF
		};

		static constexpr std::uint8_t kContinuationByteMask = 0xC0;
		static constexpr std::uint8_t kContinuationByteSignature = 0x80;
		static constexpr std::size_t kContinuationByteEncodedBitCount = 6;
	
		static std::size_t PickCodeUnitAmount(char32_t codePoint);
	};
}
