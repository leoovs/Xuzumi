#pragma once

#include "Xuzumi/String/Utf8Facts.hpp"

namespace Xuzumi::Internal
{
}

namespace Xuzumi
{
	class Utf8CharView
	{
	public:
		Utf8CharView() = default;
		Utf8CharView(const char* character);
		Utf8CharView(const std::uint8_t* character);

		const std::uint8_t* Get() const;

		Utf8CharView Next() const;
		Utf8CharView Previous() const;

		bool IsValid() const;
		bool IsTerminator() const;
		bool IsSameMemory(const Utf8CharView& other) const;

		char32_t DecodeCodePoint() const;

	private:
		std::size_t DecodeCodeUnitAmount() const;

		const std::uint8_t* mCurrentCharacter = nullptr;
		std::size_t mCodeUnitAmount = Internal::Utf8Facts::kInvalidCodeUnitAmount;
	};
}
