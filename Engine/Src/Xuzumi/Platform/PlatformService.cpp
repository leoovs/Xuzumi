#include "Xuzumi/Platform/PlatformService.hpp"

#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Platform.Win32/PlatformServiceWin32.hpp"

namespace Xuzumi
{
	SharedPtr<PlatformService> CreatePlatformService()
	{
		return CreatePlatformServiceWin32();
	}
}

#else
#	error "Xuzumi: currently, only Windows platform service is implemented"
#endif
