#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Platform.Win32/Win32SDK.hpp"

namespace Xuzumi::Internal
{
	class Win32ErrorInfo
	{
	public:
		Win32ErrorInfo() = default;
		Win32ErrorInfo(HRESULT nativeErrorCode);

		std::string ToString() const;
		const char* ToCString() const;

	private:
		void QueryStringRepresentation();

		HRESULT mNativeErrorCode = S_OK;
		std::string mAsString;
	};
}
