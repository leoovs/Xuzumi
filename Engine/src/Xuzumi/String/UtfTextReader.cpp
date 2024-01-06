#include "Xuzumi/String/UtfTextReader.hpp"

namespace Xuzumi
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Utf8TextReader
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Utf8TextReader::Utf8TextReader(Utf8CharView begin)
		: mBegin(begin)
		, mCurrentCharacter(begin)
	{}

	void Utf8TextReader::Advance()
	{
		mCurrentCharacter = mCurrentCharacter.Next();
	}

	void Utf8TextReader::RollBack()
	{
		if (!mCurrentCharacter.IsSameMemory(mBegin))
		{
			mCurrentCharacter = mCurrentCharacter.Previous();
		}
	}

	void Utf8TextReader::Reset()
	{
		mCurrentCharacter = mBegin;
	}

	char32_t Utf8TextReader::ReadCharacter() const
	{
		return mCurrentCharacter.DecodeCodePoint();
	}

	bool Utf8TextReader::CharacterIsValid() const
	{
		return mCurrentCharacter.IsValid();
	}

	bool Utf8TextReader::IsEnd() const
	{
		return mCurrentCharacter.IsTerminator();
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Utf16TextReader
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Utf16TextReader::Utf16TextReader(Utf16CharView begin)
		: mBegin(begin)
		, mCurrentCharacter(begin)
	{}

	void Utf16TextReader::Advance()
	{
		mCurrentCharacter = mCurrentCharacter.Next();
	}

	void Utf16TextReader::RollBack()
	{
		if (!mCurrentCharacter.IsSameMemory(mBegin))
		{
			mCurrentCharacter = mCurrentCharacter.Previous();
		}
	}

	void Utf16TextReader::Reset()
	{
		mCurrentCharacter = mBegin;
	}

	bool Utf16TextReader::CharacterIsValid() const
	{
		return mCurrentCharacter.IsValid();
	}
	
	char32_t Utf16TextReader::ReadCharacter() const
	{
		return mCurrentCharacter.DecodeCodePoint();
	}

	bool Utf16TextReader::IsEnd() const
	{
		return mCurrentCharacter.IsTerminator();
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Utf32TextReader
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Utf32TextReader::Utf32TextReader(const char32_t* begin)
		: mBegin(begin)
		, mCurrentCharacter(begin)
	{}

	void Utf32TextReader::Advance()
	{
		if (!IsEnd())
		{
			mCurrentCharacter++;
		}
	}

	void Utf32TextReader::RollBack()
	{
		if (mCurrentCharacter != mBegin)
		{
			mCurrentCharacter--;
		}
	}

	void Utf32TextReader::Reset()
	{
		mCurrentCharacter = mBegin;
	}

	bool Utf32TextReader::CharacterIsValid() const
	{
		return nullptr != mCurrentCharacter
			|| UnicodeFacts::IsUnknownCharacter(*mCurrentCharacter);
	}
	
	char32_t Utf32TextReader::ReadCharacter() const
	{
		return nullptr != mCurrentCharacter
			? *mCurrentCharacter
			: UnicodeFacts::kWhiteSquareCharacterCodePoint;
	}

	bool Utf32TextReader::IsEnd() const
	{
		return nullptr != mCurrentCharacter
			&& U'\0' == *mCurrentCharacter; 
	}
}
