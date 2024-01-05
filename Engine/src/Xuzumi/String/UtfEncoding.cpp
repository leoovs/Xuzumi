#include "Xuzumi/String/UtfEncoding.hpp"

#include "Xuzumi/String/Utf16Facts.hpp"

namespace Xuzumi
{
	bool EncodeUtf16(UtfTextReader& reader, std::insert_iterator<std::u16string> to)
	{
		using Internal::Utf16Facts;

		bool noProblemsWhileEncoding = true;

		while (!reader.IsEnd())
		{
			char32_t codePoint = reader.ReadCharacter();
			reader.Advance();

			if (UnicodeFacts::kReplacementCharacterCodePoint == codePoint)
			{
				noProblemsWhileEncoding = false;
			}

			if (codePoint <= Utf16Facts::kCodePointSurrogatePairClip)
			{
				auto codeUnit = static_cast<char16_t>(codePoint);
				to = codeUnit;
				continue;
			}

			codePoint -= Utf16Facts::kCodePointSurrogatePairClip;
			
			char32_t lowBits = codePoint & char32_t(~Utf16Facts::kLowSurrogateMask);
			char32_t highBits = (codePoint >> Utf16Facts::kSurrogateEncodedBitCount)
				& char32_t(~Utf16Facts::kHighSurrogateMask);
		
			lowBits += char32_t(Utf16Facts::kLowSurrogateSignature);
			highBits += char32_t(Utf16Facts::kHighSurrogateSignature);
		
			to = static_cast<char16_t>(highBits);
			to = static_cast<char16_t>(lowBits);
		}
	
		return noProblemsWhileEncoding;
	}
}
