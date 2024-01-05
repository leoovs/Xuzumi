#include "Xuzumi/String/Utf8CharView.hpp"

#include "Xuzumi/String/UnicodeFacts.hpp"

namespace Xuzumi
{
	Utf8CharView::Utf8CharView(const char* character)
		: Utf8CharView(reinterpret_cast<const std::uint8_t*>(character))
	{}

	Utf8CharView::Utf8CharView(const std::uint8_t* character)
		: mCurrentCharacter(character)
		, mCodeUnitAmount(DecodeCodeUnitAmount())
	{}

	const std::uint8_t* Utf8CharView::Get() const
	{
		return mCurrentCharacter;
	}

	Utf8CharView Utf8CharView::Next() const
	{
		if (IsValid() && !IsTerminator())
		{
			return Utf8CharView(mCurrentCharacter + mCodeUnitAmount);
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
			&& Internal::Utf8Facts::kInvalidCodeUnitAmount != mCodeUnitAmount;
	}

	bool Utf8CharView::IsTerminator() const
	{
		return nullptr != mCurrentCharacter
			&& 0 == *mCurrentCharacter;
	}
	
	bool Utf8CharView::IsSameMemory(const Utf8CharView& other) const
	{
		return mCurrentCharacter == other.mCurrentCharacter;
	}

	char32_t Utf8CharView::DecodeCodePoint() const
	{
		std::uint8_t mask
			= Internal::Utf8Facts::kLeaderByteMasks[mCodeUnitAmount - 1];
		mask = ~mask;
		auto codePoint = std::uint32_t(mask & *mCurrentCharacter);
	
		for (std::size_t iCodeUnit = 1; iCodeUnit < mCodeUnitAmount; iCodeUnit++)
		{
			mask = Internal::Utf8Facts::kContinuationByteMask;
			uint8_t codeUnit = mCurrentCharacter[iCodeUnit];

			if ((codeUnit & mask) != Internal::Utf8Facts::kContinuationByteSignature)
			{
				return UnicodeFacts::kReplacementCharacterCodePoint;
			}

			mask = ~mask;
			auto bits = std::uint32_t(codeUnit & mask);

			codePoint <<= Internal::Utf8Facts::kContinuationByteEncodedBitCount;
			codePoint |= bits;
		}

		return codePoint;
	}

	std::size_t Utf8CharView::DecodeCodeUnitAmount() const
	{
		if (nullptr == mCurrentCharacter)
		{
			return Internal::Utf8Facts::kInvalidCodeUnitAmount;
		}

		for (std::size_t codeUnitAmount = 1;
			codeUnitAmount <= Internal::Utf8Facts::kMaxCodeUnits; codeUnitAmount++)
		{
			std::uint8_t mask = Internal::Utf8Facts::kLeaderByteMasks[codeUnitAmount - 1];	
			std::uint8_t sign = Internal::Utf8Facts::kDecodedCharacterSizeSignatures[codeUnitAmount - 1];

			if ((*mCurrentCharacter & mask) == sign)
			{
				return codeUnitAmount;
			}
		}
	
		return Internal::Utf8Facts::kInvalidCodeUnitAmount;
	}
}
