#pragma once

#include "Xuzumi/String/Utf16Facts.hpp"

namespace Xuzumi
{
	class Utf16CharView
	{
	public:
		Utf16CharView() = default;
		Utf16CharView(const char16_t* character);

		const char16_t* Get() const;

		Utf16CharView Next() const;
		Utf16CharView Previous() const;

		bool IsValid() const;
		bool IsTerminator() const;
		bool IsSameMemory(const Utf16CharView& other) const;

		char32_t DecodeCodePoint() const;

	private:
		std::size_t DecodeCodeUnitAmount() const;
		char16_t GetHighSurrogate() const;
		char16_t GetLowSurrogate() const;

		const char16_t* mCurrentCharacter = nullptr;
		std::size_t mCodeUnitAmount = Internal::Utf16Facts::kInvalidCodeUnitAmount;
	};
}
