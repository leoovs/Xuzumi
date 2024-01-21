#include "Xuzumi/String/StringEncoder.hpp"

#include "Xuzumi/String/Utf8Facts.hpp"
#include "Xuzumi/String/Utf16Facts.hpp"
#include "Xuzumi/String/UnicodeFacts.hpp"

namespace Xuzumi
{
	using Internal::Utf8Facts;
	using Internal::Utf16Facts;

	StringEncoder::StringEncoder(ObserverPtr<UtfTextReader> reader)
	{
		SetReader(reader);
	}

	void StringEncoder::SetReader(ObserverPtr<UtfTextReader> reader)
	{
		mReader = reader;
		ResetReader();
		mStatus = Status::Idle;
	}

	void StringEncoder::EncodeUtf8(std::insert_iterator<std::string> dest)
	{
		if (!mReader)
		{
			mStatus = Status::ReaderNotSet;
			return;
		}
		ResetReader();

		for (; !mReader->IsEnd(); mReader->Advance())
		{
			if (!mReader->CharacterIsValid())
			{
				mStatus = Status::InvalidCharacter;
				return;
			}

			char32_t codePoint = mReader->ReadCharacter();
			if (UnicodeFacts::IsUnknownCharacter(codePoint))
			{
				mStatus = Status::InvalidCharacter;
				return;
			}

			std::size_t codeUnitAmount = Utf8Facts::PickCodeUnitAmount(codePoint);
			if (Utf8Facts::kInvalidCodeUnitAmount == codeUnitAmount)
			{
				mStatus = Status::InvalidCharacter;
				return;
			}

			if (1 == codeUnitAmount)
			{
				dest = static_cast<char>(codePoint);
				continue;
			}

			dest = static_cast<char>(
				codePoint >> Utf8Facts::kContinuationByteEncodedBitCount
				* (codeUnitAmount - 1)
				& ~Utf8Facts::kLeaderByteMasks[codeUnitAmount - 1]
				| Utf8Facts::kDecodedCharacterSizeSignatures[codeUnitAmount - 1]
			);
			auto codeUnitsLeft = static_cast<std::ptrdiff_t>(codeUnitAmount) - 1;

			for (std::ptrdiff_t iContinue = codeUnitsLeft - 1; iContinue >= 0;
				iContinue--)
			{
				dest = static_cast<char>(
					codePoint >> Utf8Facts::kContinuationByteEncodedBitCount * iContinue
					& ~Utf8Facts::kContinuationByteMask
					| Utf8Facts::kContinuationByteSignature
				);
			}
		}

		mStatus = Status::Success;
	}

	void StringEncoder::EncodeUtf16(std::insert_iterator<std::u16string> dest)
	{
		if (!mReader)
		{
			mStatus = Status::ReaderNotSet;
			return;
		}

		ResetReader();
		for (; !mReader->IsEnd(); mReader->Advance())
		{
			if (!mReader->CharacterIsValid())
			{
				mStatus = Status::InvalidCharacter;
				return;
			}

			char32_t codePoint = mReader->ReadCharacter();
			if (UnicodeFacts::IsUnknownCharacter(codePoint))
			{
				mStatus = Status::InvalidCharacter;
				return;
			}

			if (codePoint < Utf16Facts::kCodePointSurrogatePairClip)
			{
				auto codeUnit = static_cast<char16_t>(codePoint);
				dest = codeUnit;
				continue;
			}

			codePoint -= Utf16Facts::kCodePointSurrogatePairClip;

			char32_t lowBits = codePoint & char32_t(~Utf16Facts::kLowSurrogateMask);
			char32_t highBits = (codePoint >> Utf16Facts::kSurrogateEncodedBitCount)
				& char32_t(~Utf16Facts::kHighSurrogateMask);

			lowBits += char32_t(Utf16Facts::kLowSurrogateSignature);
			highBits += char32_t(Utf16Facts::kHighSurrogateSignature);

			dest = static_cast<char16_t>(highBits);
			dest = static_cast<char16_t>(lowBits);
		}

		mStatus = Status::Success;
	}

	StringEncoder::Status StringEncoder::GetStatus() const
	{
		return mStatus;
	}

	void StringEncoder::ResetReader() const
	{
		if (mReader)
		{
			mReader->Reset();
		}
	}
}
