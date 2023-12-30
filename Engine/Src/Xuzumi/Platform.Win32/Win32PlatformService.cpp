#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Platform.Win32/Win32PlatformService.hpp"

namespace Xuzumi::Internal
{
	void Win32PlatformService::Connect(ObserverPtr<EventBus> events)
	{
		mWindowClass.Connect(events);
	}

	void Win32PlatformService::PollEvents()
	{
		for (MSG msg = {}; PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE);)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	SharedPtr<WindowFrame> Win32PlatformService::CreateWindowFrame(
		const WindowFrameSpecification& specification)
	{
		return mWindowClass.CreateFrameFromSpecification(specification);
	}

	SharedPtr<Win32PlatformService> CreatePlatformServiceWin32()
	{
		return MakeShared<Win32PlatformService>();
	}
}

#endif // XZ_PLATFORM_IS(WIN32)
