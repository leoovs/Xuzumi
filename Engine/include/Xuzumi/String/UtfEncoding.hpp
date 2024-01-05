#pragma once

#include "Xuzumi/String/UtfTextReader.hpp"

namespace Xuzumi
{
	// TODO: add UTF-8 encoding and add proper error handling.

	bool EncodeUtf16(UtfTextReader& reader, std::insert_iterator<std::u16string> to);
}
