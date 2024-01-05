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

	char32_t Utf8TextReader::ReadCharacter() const
	{
		return mCurrentCharacter.DecodeCodePoint();
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

	char32_t Utf16TextReader::ReadCharacter() const
	{
		return mCurrentCharacter.DecodeCodePoint();
	}

	bool Utf16TextReader::IsEnd() const
	{
		return mCurrentCharacter.IsTerminator();
	}
}
