#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi::Internal
{
	struct Utf8Facts
	{
		static constexpr std::size_t kInvalidCharacterSize = 0;
		static constexpr std::size_t kMaxCharacterSize = 4;
	
		static constexpr std::array<std::uint8_t, kMaxCharacterSize> kLeaderByteMasks
		{
			0x80,
			0xE0,
			0xF0,
			0xF8
		};
	
		static constexpr std::array<std::uint8_t, kMaxCharacterSize>
		kDecodedCharacterSizeSignatures
		{
			0x00,
			0xC0,
			0xE0,
			0xF0
		};

		static constexpr std::uint8_t kContinuationByteMask = 0xC0;
		static constexpr std::uint8_t kContinuationByteSignature = 0x80;
		static constexpr std::size_t kContinuationByteEncodedBitCount = 6;
	};
}

namespace Xuzumi
{
	class Utf8CharView
	{
	public:
		Utf8CharView() = default;
		Utf8CharView(const std::uint8_t* character);

		Utf8CharView Next() const;
		Utf8CharView Previous() const;

		bool IsValid() const;
		bool IsTerminator() const;

		std::uint32_t DecodeCodePoint() const;

	private:
		std::size_t DecodeCurrentSize() const;

		const std::uint8_t* mCurrentCharacter = nullptr;
		std::size_t mCurrentSize = 0;
	};
}
