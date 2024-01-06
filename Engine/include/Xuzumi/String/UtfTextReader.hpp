#pragma once

#include "Xuzumi/String/UnicodeFacts.hpp"
#include "Xuzumi/String/Utf8CharView.hpp"
#include "Xuzumi/String/Utf16CharView.hpp"

namespace Xuzumi
{
	class UtfTextReader 
	{
	public:
		virtual ~UtfTextReader() = default;

		virtual void Advance() = 0;
		virtual void RollBack() = 0;
		virtual void Reset() = 0;

		virtual char32_t ReadCharacter() const = 0;

		virtual bool CharacterIsValid() const = 0;
		virtual bool IsEnd() const = 0;
	};

	class Utf8TextReader : public UtfTextReader
	{
	public:
		Utf8TextReader(Utf8CharView begin);

		~Utf8TextReader() override = default;
	
		void Advance() override;
		void RollBack() override;
		void Reset() override;

		char32_t ReadCharacter() const override;

		bool CharacterIsValid() const override;
		bool IsEnd() const override;

	private:
	 	Utf8CharView mBegin;
		Utf8CharView mCurrentCharacter;
	};

	class Utf16TextReader : public UtfTextReader
	{
	public:
		Utf16TextReader(Utf16CharView begin);

		~Utf16TextReader() override = default;
	
		void Advance() override;
		void RollBack() override;
		void Reset() override;

		char32_t ReadCharacter() const override;

		bool CharacterIsValid() const override;
		bool IsEnd() const override;

	private:
		Utf16CharView mBegin;
		Utf16CharView mCurrentCharacter;
	};

	class Utf32TextReader : public UtfTextReader
	{
	public:
		Utf32TextReader(const char32_t* begin);

		~Utf32TextReader() override = default;

		void Advance() override;
		void RollBack() override;
		void Reset() override;

		char32_t ReadCharacter() const override;
		
		bool CharacterIsValid() const override;
		bool IsEnd() const override;

	private:
		const char32_t* mBegin = nullptr;
		const char32_t* mCurrentCharacter = nullptr;
	};
}
