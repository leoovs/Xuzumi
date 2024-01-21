/**
 * @file Xuzumi/Platform/PlatformEvents.hpp
 *
 * @brief Defines platform events such as window frame events.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"

namespace Xuzumi
{
	class WindowFrame;

	/**
	 * @brief An event that occurs when a window frame is closed.
	 */
	class WindowFrameClosedEvent : public Event<WindowFrameClosedEvent>
	{
	public:
		/**
		 * @brief A pointer to the window frame that was closed.
		 */
		ObserverPtr<WindowFrame> ClosedFrame = nullptr;

		/**
		 * @brief Constructs a `WindowFrameClosedEvent`.
		 *
		 * @param closedFrame A pointer to the `WindowFrame` that was closed.
		 */
		WindowFrameClosedEvent(ObserverPtr<WindowFrame> closedFrame)
			: ClosedFrame(closedFrame)
		{}
	};

	/**
	 * @brief An event that occurs when a window frame resized
	 */
	class WindowFrameResizedEvent : public Event<WindowFrameResizedEvent>
	{
	public:
		/**
		 * @brief A pointer to the window frame that was closed.
		 */
		ObserverPtr<WindowFrame> ResizedFrame = nullptr;

		/**
		 * @brief A fresh width of the client area of the window frame.
		 */
		std::uint32_t Width = 0;

		/**
		 * @brief A fresh height of the client area of the window frame.
		 */
		std::uint32_t Height = 0;

		/**
		 * @brief Constructs a `WindowFrameResizedEvent`.
		 *
		 * @param resizedFrame A pointer to the resized window frame.
		 * @param width A fresh width of the window frame.
		 * @param height A fresh height of the window frame.
		 */
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
