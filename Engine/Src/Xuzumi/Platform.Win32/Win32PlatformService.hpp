#pragma once

#include "Xuzumi/Platform/PlatformService.hpp"
#include "Xuzumi/Platform.Win32/Win32WindowClass.hpp"

namespace Xuzumi::Internal
{
	class Win32PlatformService : public PlatformService
	{
	public:
		void Connect(ObserverPtr<EventBus> events) override;
		void PollEvents() override;
	
		SharedPtr<WindowFrame> CreateWindowFrame(
			const WindowFrameSpecification& specification) override;

	private:
		Win32WindowClass mWindowClass;
	};

	SharedPtr<Win32PlatformService> CreatePlatformServiceWin32();
}
