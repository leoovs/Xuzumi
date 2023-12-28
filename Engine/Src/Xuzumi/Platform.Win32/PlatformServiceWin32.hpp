#pragma once

#include "Xuzumi/Platform/PlatformService.hpp"

namespace Xuzumi
{
	class PlatformServiceWin32 : public PlatformService
	{
	public:
	};

	SharedPtr<PlatformServiceWin32> CreatePlatformServiceWin32();
}
