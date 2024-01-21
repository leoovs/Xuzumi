/**
 * @file Xuzumi/Platform/PlatformService.hpp
 *
 * @brief Defines the `PlatformService` abstract class.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Input/InputDevice.hpp"
#include "Xuzumi/Memory/SharedPtr.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"
#include "Xuzumi/Platform/WindowFrame.hpp"

namespace Xuzumi
{
	/**
	 * @brief The main part of the engine platform abstraction layer.
	 *
	 * Provides access to the platform dependent components such as input,
	 * graphics, audio, etc.
	 */
	class PlatformService
	{
	public:
		/**
		 * @brief Performs clean-up of platform-specific resources allocated for
		 * the service.
		 */
		virtual ~PlatformService() = default;

		/**
		 * @brief Provides the service with an event bus.
		 *
		 * @param events The event bus that will transmit events platform produces.
		 */
		virtual void Connect(ObserverPtr<EventBus> events) = 0;

		/**
		 * @brief Interviews the platform about any user-input events.
		 *
		 * The resulting events are passed to an event bus if one is connected.
		 */
		virtual void PollEvents() = 0;

		/**
		 * @brief Creates a window frame that is used for visual interaction with
		 * a user.
		 *
		 * @param specification A filled in window frame specification.
		 *
		 * @return A `SharedPtr` that references a window frame handle.
		 */
		virtual SharedPtr<WindowFrame> CreateWindowFrame(
			const WindowFrameSpecification& specification
		) = 0;

		/**
		 * @brief Queries a logical input device.
		 *
		 * @return A `SharedPtr` that references the logical input device.
		 */
		virtual SharedPtr<InputDevice> GetInputDevice() const = 0;
	};

	/**
	 * @brief Creates platform service.
	 *
	 * There should be only one platform service instance at a time.
	 *
	 * @return A `SharedPtr` that references a platform service handle.
	 */
	SharedPtr<PlatformService> CreatePlatformService();
}
