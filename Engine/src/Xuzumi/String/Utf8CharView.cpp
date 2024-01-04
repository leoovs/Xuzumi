#include "Xuzumi/String/Utf8CharView.hpp"

namespace Xuzumi
{
	Utf8CharView::Utf8CharView(const std::uint8_t* character)
		: mCurrentCharacter(character)
		, mCurrentSize(DecodeCurrentSize())
	{}

	Utf8CharView Utf8CharView::Next() const
	{
		if (IsValid() && !IsTerminator())
		{
			return Utf8CharView(mCurrentCharacter + mCurrentSize);
		}

		return *this;
	}

	Utf8CharView Utf8CharView::Previous() const
	{
		const std::uint8_t* character = mCurrentCharacter;

		auto isContinuationByte = [&character]()
		{
			std::uint8_t current = *character;
			return (current & Internal::Utf8Facts::kContinuationByteMask)
				== Internal::Utf8Facts::kContinuationByteSignature;
		};
	
		do
		{
			character--;
		} while (isContinuationByte());

		return Utf8CharView(character);
	}

	bool Utf8CharView::IsValid() const
	{
		return nullptr != mCurrentCharacter
			&& Internal::Utf8Facts::kInvalidCharacterSize != mCurrentSize;
	}

	bool Utf8CharView::IsTerminator() const
	{
		return nullptr != mCurrentCharacter
			&& 0 == *mCurrentCharacter;
	}

	std::uint32_t Utf8CharView::DecodeCodePoint() const
	{
		std::uint8_t mask = Internal::Utf8Facts::kLeaderByteMasks[mCurrentSize - 1];
		mask = ~mask;
		auto codePoint = std::uint32_t(mask & *mCurrentCharacter);
	
		for (std::size_t iByte = 1; iByte < mCurrentSize; iByte++)
		{
			mask = Internal::Utf8Facts::kContinuationByteMask;
			mask = ~mask;
			auto bits = std::uint32_t(mask & mCurrentCharacter[iByte]);

			codePoint <<= Internal::Utf8Facts::kContinuationByteEncodedBitCount;
			codePoint |= bits;
		}

		return codePoint;
	}

	std::size_t Utf8CharView::DecodeCurrentSize() const
	{
		for (std::size_t charSize = 1;
			charSize <= Internal::Utf8Facts::kMaxCharacterSize; charSize++)
		{
			std::uint8_t mask = Internal::Utf8Facts::kLeaderByteMasks[charSize - 1];	
			std::uint8_t sign
				= Internal::Utf8Facts::kDecodedCharacterSizeSignatures[charSize - 1];

			if ((*mCurrentCharacter & mask) == sign)
			{
				return charSize;
			}
		}
	
		return Internal::Utf8Facts::kInvalidCharacterSize;
	}
}
