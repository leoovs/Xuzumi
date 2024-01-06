#pragma once

#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/String/UtfTextReader.hpp"

namespace Xuzumi
{
	class StringEncoder
	{
	public:
		enum class Status 
		{
			Idle,
			Success,
			ReaderNotSet,
			InvalidCharacter
		};

		StringEncoder() = default;
		StringEncoder(ObserverPtr<UtfTextReader> reader);

		void SetReader(ObserverPtr<UtfTextReader> reader);

		void EncodeUtf8(std::insert_iterator<std::string> dest);
		void EncodeUtf16(std::insert_iterator<std::u16string> dest);

		Status GetStatus() const;

	private:
		void ResetReader() const;

		ObserverPtr<UtfTextReader> mReader;
		Status mStatus = Status::Idle;
	};
}
