#include "Xuzumi/String/Utf16CharView.hpp"

#include "Xuzumi/String/UnicodeFacts.hpp"

namespace Xuzumi
{
	Utf16CharView::Utf16CharView(const char16_t* character)
		: mCurrentCharacter(character)
		, mCodeUnitAmount(DecodeCodeUnitAmount())
	{}

	Utf16CharView Utf16CharView::Next() const
	{
		if (IsValid() && !IsTerminator())
		{
			return Utf16CharView(mCurrentCharacter + mCodeUnitAmount);
		}
		return *this;
	}

	const char16_t* Utf16CharView::Get() const
	{
		return mCurrentCharacter;
	}

	Utf16CharView Utf16CharView::Previous() const
	{
		const char16_t* character = mCurrentCharacter;
		character--;

		if ((*character & Internal::Utf16Facts::kLowSurrogateMask)
			== Internal::Utf16Facts::kLowSurrogateSignature)
		{
			character--;
		}
	
		return Utf16CharView(character);
	}

	bool Utf16CharView::IsValid() const
	{
		return nullptr != mCurrentCharacter
			&& Internal::Utf16Facts::kInvalidCodeUnitAmount != mCodeUnitAmount;
	}

	bool Utf16CharView::IsTerminator() const
	{
		return nullptr != mCurrentCharacter 
			&& 0 == *mCurrentCharacter;
	}

	bool Utf16CharView::IsSameMemory(const Utf16CharView& other) const
	{
		return mCurrentCharacter == other.mCurrentCharacter;
	}

	char32_t Utf16CharView::DecodeCodePoint() const
	{
		if (Internal::Utf16Facts::kMaxCodeUnits != mCodeUnitAmount)
		{
			return static_cast<std::uint32_t>(*mCurrentCharacter);
		}
	
		char16_t highSurrogate = GetHighSurrogate();
		if ((highSurrogate & Internal::Utf16Facts::kHighSurrogateMask)
			!= Internal::Utf16Facts::kHighSurrogateSignature)
		{
			return UnicodeFacts::kReplacementCharacterCodePoint;
		}

		char16_t lowSurrogate = GetLowSurrogate();
		if ((lowSurrogate & Internal::Utf16Facts::kLowSurrogateMask)
			!= Internal::Utf16Facts::kLowSurrogateSignature)
		{
			return UnicodeFacts::kReplacementCharacterCodePoint;
		}

		char16_t highBits = highSurrogate 
			- Internal::Utf16Facts::kHighSurrogateSignature;
		char16_t lowBits = lowSurrogate 
			- Internal::Utf16Facts::kLowSurrogateSignature;
	
		auto codePoint = static_cast<std::uint32_t>(highBits);
		codePoint <<= Internal::Utf16Facts::kSurrogateEncodedBitCount;
		codePoint |= static_cast<std::uint32_t>(lowBits);

		codePoint += Internal::Utf16Facts::kCodePointSurrogatePairClip;
	
		return codePoint;
	}

	std::size_t Utf16CharView::DecodeCodeUnitAmount() const
	{
		if ((GetHighSurrogate() & Internal::Utf16Facts::kHighSurrogateMask)
			== Internal::Utf16Facts::kHighSurrogateSignature)
		{
			if ((GetLowSurrogate() & Internal::Utf16Facts::kLowSurrogateMask)
				== Internal::Utf16Facts::kLowSurrogateSignature)
			{
				return Internal::Utf16Facts::kMaxCodeUnits;
			}
		}

		// Code unit numerically equal to the corresponding code point.
		return 1;
	}

	char16_t Utf16CharView::GetHighSurrogate() const
	{
		return mCurrentCharacter[0];
	}

	char16_t Utf16CharView::GetLowSurrogate() const
	{
		return mCurrentCharacter[1];
	}
}
