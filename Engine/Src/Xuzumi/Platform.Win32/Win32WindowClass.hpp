#pragma once

#include "Xuzumi/Memory/PoolAllocator.hpp"
#include "Xuzumi/Messaging/EventBus.hpp"
#include "Xuzumi/Messaging/EventPublisher.hpp"
#include "Xuzumi/Platform.Win32/Win32SDK.hpp"
#include "Xuzumi/Platform.Win32/Win32WindowFrame.hpp"

namespace Xuzumi::Internal
{
	class Win32WindowClass
	{
	public:
		Win32WindowClass();
		~Win32WindowClass();

		SharedPtr<Win32WindowFrame> CreateFrameFromSpecification(
			const WindowFrameSpecification& specification
		);

		void DestroyNativeWindow(HWND nativeWindow);

		void Connect(ObserverPtr<EventBus> events);

		void NotifyWindowFrameClosed(ObserverPtr<Win32WindowFrame> closedFrame);
		void NotifyWindowFrameResized(
			ObserverPtr<Win32WindowFrame> resizedFrame,
			std::uint32_t width,
			std::uint32_t height
		);

	private:
		static LRESULT CALLBACK WindowProcedure(
			HWND hWnd,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam
		);

		static void StoreFrameAsNativeWindowUserData(
			ObserverPtr<Win32WindowFrame> frame,
			HWND nativeWindow
		);
		static ObserverPtr<Win32WindowFrame> RestoreFrameFromNativeWindowUserData(
			HWND nativeWindow
		);

		void Register();
		void Unregister();

		static constexpr std::size_t kMaxWindowFrames = 20;
		static constexpr std::string_view kClassName = "Xuzumi.Win32WindowFrame";

		PoolAllocator<Win32WindowFrame> mFramePool{ kMaxWindowFrames };
		HINSTANCE mExecutableHandle = nullptr;
		EventPublisher mPlatformEventPublisher;
	};
}
