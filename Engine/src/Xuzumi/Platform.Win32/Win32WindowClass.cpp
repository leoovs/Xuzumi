#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Platform.Win32/Win32WindowClass.hpp"

#include "Xuzumi/Debug/DebugSession.hpp"
#include "Xuzumi/Input/InputEvents.hpp"
#include "Xuzumi/Input.Win32/Win32InputEvents.hpp"
#include "Xuzumi/Platform/PlatformEvents.hpp"
#include "Xuzumi/Platform.Win32/Win32Debug.hpp"
#include "Xuzumi/Platform.Win32/Win32PlatformService.hpp"

namespace Xuzumi::Internal
{
	Win32WindowClass::Win32WindowClass()
	{
		Register();
	}	

	Win32WindowClass::~Win32WindowClass()
	{
		Unregister();
	}

	SharedPtr<Win32WindowFrame> Win32WindowClass::CreateFrameFromSpecification(
		const WindowFrameSpecification& specification
	)
	{
		DWORD nativeStyle = WS_OVERLAPPEDWINDOW;
		if (!specification.Resizable)
		{
			nativeStyle &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
		}
		if (specification.Visible)
		{
			nativeStyle |= WS_VISIBLE;
		}

		HWND nativeWindow = CreateWindowExW(
			0,
			kClassName.data(),
			L"",
			nativeStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<int>(specification.Width),
			static_cast<int>(specification.Height),
			nullptr,
			nullptr,
			mExecutableHandle,
			nullptr
		);
	
		if (nullptr == nativeWindow)
		{
			Win32ErrorInfo err = GetLastError();

			XZ_LOG(Error, "Could not create Win32 Window: %s", err.ToCString());

			return nullptr;
		}
	
		SharedPtr<Win32WindowFrame> frame = mFramePool.AllocateShared(
			specification,
			nativeWindow,
			ObserverPtr<Win32WindowClass>(this)
		);
	
		StoreFrameAsNativeWindowUserData(
			ObserverPtr<Win32WindowFrame>(frame.Get()),
			nativeWindow
		);

		frame->SetCaption(specification.Caption);

		return frame;
	}

	void Win32WindowClass::DestroyNativeWindow(HWND nativeWindow)
	{
		bool destroyed = DestroyWindow(nativeWindow);
	
		if (!destroyed)
		{
			XZ_LOG(Error, "Could not destroy native Win32 window");
		}
	}

	void Win32WindowClass::Connect(ObserverPtr<EventBus> events)
	{
		mPlatformEventPublisher.Connect(events);
	}

	void Win32WindowClass::NotifyWindowFrameClosed(
		ObserverPtr<Win32WindowFrame> closedFrame
	)
	{
		mPlatformEventPublisher.Publish<WindowFrameClosedEvent>(closedFrame);
	}

	void Win32WindowClass::NotifyWindowFrameResized(
		ObserverPtr<Win32WindowFrame> resizedFrame,
		std::uint32_t width,
		std::uint32_t height
	)
	{
		mPlatformEventPublisher.Publish<WindowFrameResizedEvent>(
			resizedFrame,
			width,
			height
		);
	}

	void Win32WindowClass::NotifyWin32KeyDown(
		WPARAM messageWParam,
		LPARAM messageLParam
	)
	{
		mPlatformEventPublisher.Publish<Win32KeyDownEvent>(
			messageWParam,
			messageLParam
		);
	}

	void Win32WindowClass::NotifyWin32KeyUp(
		WPARAM messageWParam,
		LPARAM messageLParam
	)
	{
		mPlatformEventPublisher.Publish<Win32KeyUpEvent>(
			messageWParam,
			messageLParam
		);
	}

	void Win32WindowClass::NotifyCharacterInput(char32_t unicodeCodePoint)
	{
		mPlatformEventPublisher.Publish<CharacterInputEvent>(unicodeCodePoint);
	}

	LRESULT CALLBACK Win32WindowClass::WindowProcedure(
		HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		ObserverPtr<Win32WindowFrame> frame = RestoreFrameFromNativeWindowUserData(
			hWnd
		);

		if (!frame)
		{
			return DefWindowProcA(hWnd, msg, wParam, lParam);
		}

		return frame->OnWindowProcedure(msg, wParam, lParam);
	}	

	void Win32WindowClass::StoreFrameAsNativeWindowUserData(
		ObserverPtr<Win32WindowFrame> frame,
		HWND nativeWindow
	)
	{
		SetWindowLongPtrW(
			nativeWindow,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(frame.Get())
		);
	}

	ObserverPtr<Win32WindowFrame>
	Win32WindowClass::RestoreFrameFromNativeWindowUserData(
		HWND nativeWindow
	)
	{
		LONG_PTR userdata = GetWindowLongPtrW(nativeWindow, GWLP_USERDATA);
	
		return ObserverPtr<Win32WindowFrame>(
			reinterpret_cast<Win32WindowFrame*>(userdata)
		);
	}
	
	void Win32WindowClass::Register()
	{
		WNDCLASSEXW nativeSpecification = {};
		nativeSpecification.cbSize = sizeof(nativeSpecification);
		nativeSpecification.lpfnWndProc = WindowProcedure;
		nativeSpecification.lpszClassName = kClassName.data();
		nativeSpecification.hInstance = mExecutableHandle;

		ATOM result = RegisterClassExW(&nativeSpecification);
		XZ_ASSERT(0 != result, "Could not register Win32 Window Class");	

		XZ_LOG(Info, "Win32 Window Class Registered");
	}

	void Win32WindowClass::Unregister()
	{
		auto unregistered = static_cast<bool>(
			UnregisterClassW(kClassName.data(), mExecutableHandle)
		);
	
		if (!unregistered)
		{
			XZ_LOG(Warning, "Could not unregister Win32 Window Class");
			return;
		}

		XZ_LOG(Info, "Win32 Window Class unregistered");
	}
}

#endif // XZ_PLATFORM_IS(WIN32)
