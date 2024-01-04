#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Input/InputDevice.hpp"
#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"
#include "Xuzumi/Platform/WindowFrame.hpp"

namespace Xuzumi
{
	class PlatformService
	{
	public:
		virtual ~PlatformService() = default;
	
		virtual void Connect(ObserverPtr<EventBus> events) = 0;
		virtual void PollEvents() = 0;
	
		virtual SharedPtr<WindowFrame> CreateWindowFrame(
			const WindowFrameSpecification& specification
		) = 0;
	
		virtual SharedPtr<InputDevice> GetInputDevice() const = 0;
	};

	SharedPtr<PlatformService> CreatePlatformService();
}
