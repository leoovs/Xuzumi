#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/SharedPtr.hpp"

namespace Xuzumi
{
	class PlatformService
	{
	public:
		virtual ~PlatformService() = default;
	};

	SharedPtr<PlatformService> CreatePlatformService();
}
