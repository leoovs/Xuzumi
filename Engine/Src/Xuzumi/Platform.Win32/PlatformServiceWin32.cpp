#include "Xuzumi/Platform.Win32/PlatformServiceWin32.hpp"

namespace Xuzumi
{
	SharedPtr<PlatformServiceWin32> CreatePlatformServiceWin32()
	{
		return MakeShared<PlatformServiceWin32>();
	}
}
