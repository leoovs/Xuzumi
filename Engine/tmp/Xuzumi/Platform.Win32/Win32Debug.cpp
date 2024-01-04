#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Platform.Win32/Win32Debug.hpp"

namespace Xuzumi::Internal
{
	Win32ErrorInfo::Win32ErrorInfo(HRESULT nativeErrorCode)
		: mNativeErrorCode(nativeErrorCode)
	{
		QueryStringRepresentation();
	}

	std::string Win32ErrorInfo::ToString() const
	{
		return mAsString;
	}

	const char* Win32ErrorInfo::ToCString() const
	{
		return mAsString.c_str();
	}

	void Win32ErrorInfo::QueryStringRepresentation()
	{
		LPSTR buffer = nullptr;
		DWORD formatResult = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			mNativeErrorCode,
			// We want errors in English worldwide.
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			reinterpret_cast<LPSTR>(&buffer),
			0,
			nullptr
		);
	
		if (0 == formatResult)
		{
			mAsString = "FORMATTING FAILURE";
			return;
		}
	
		mAsString.assign(buffer);
		LocalFree(buffer);
	}
}

#endif // XZ_PLATFORM_IS(WIN32)
