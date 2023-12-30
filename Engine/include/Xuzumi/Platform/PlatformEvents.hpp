#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"

namespace Xuzumi
{
	class WindowFrame;

	class WindowFrameClosedEvent : public Event<WindowFrameClosedEvent>
	{
	public:
		ObserverPtr<WindowFrame> ClosedFrame = nullptr;

		WindowFrameClosedEvent(ObserverPtr<class WindowFrame> closedFrame)
			: ClosedFrame(closedFrame)
		{}
	};

	class WindowFrameResizedEvent : public Event<WindowFrameResizedEvent>
	{
	public:
		ObserverPtr<WindowFrame> ResizedFrame = nullptr;
		std::uint32_t Width = 0;
		std::uint32_t Height = 0;
	
		WindowFrameResizedEvent(
			ObserverPtr<WindowFrame> resizedFrame,
			std::uint32_t width,
			std::uint32_t height
		)
			: ResizedFrame(resizedFrame)
			, Width(width)
			, Height(height)
		{}
	};
}
