#include "Xuzumi/Core/PlatformInfo.hpp"

#if XZ_PLATFORM_IS(WIN32)

#include "Xuzumi/Platform.Win32/Win32WindowFrame.hpp"

#include "Xuzumi/Debug/DebugSession.hpp"
#include "Xuzumi/String/UtfEncoding.hpp"
#include "Xuzumi/Platform.Win32/Win32WindowClass.hpp"

namespace Xuzumi::Internal
{
	Win32WindowFrame::Win32WindowFrame(
		const WindowFrameSpecification& specification,
		HWND nativeWindow,
		ObserverPtr<Win32WindowClass> parent
	)
		: mSpecification(specification)
		, mNativeWindow(nativeWindow)
		, mParent(parent)
	{
	}

	Win32WindowFrame::~Win32WindowFrame()
	{
		mParent->DestroyNativeWindow(std::exchange(mNativeWindow, nullptr));
	}

	const WindowFrameSpecification& Win32WindowFrame::GetSpecification() const
	{
		return mSpecification;
	}

	void Win32WindowFrame::SetCaption(std::string_view caption)
	{
		mSpecification.Caption = caption;

		Utf8TextReader captionReader(caption.data());
		std::u16string utf16Caption;
		EncodeUtf16(
			captionReader,
			std::inserter(utf16Caption, utf16Caption.begin())
		);
	
		SetWindowTextW(
			mNativeWindow,
			reinterpret_cast<LPCWSTR>(utf16Caption.data())
		);
	}
	
	void Win32WindowFrame::SetSize(std::uint32_t width, std::uint32_t height)
	{
		mSpecification.Width = width;
		mSpecification.Height = height;

		SetWindowPos(
			mNativeWindow,
			nullptr,
			0,
			0,
			static_cast<int>(width),
			static_cast<int>(height),
			SWP_NOMOVE | SWP_NOZORDER
		);
	}
	
	void Win32WindowFrame::AllowResizing(bool resizable)
	{
		mSpecification.Resizable = resizable;
	
		auto nativeStyle = static_cast<DWORD>(
			GetWindowLongPtrA(mNativeWindow, GWL_STYLE)
		);
		DWORD nativeResizable = WS_SIZEBOX | WS_MAXIMIZEBOX;
	
		resizable
			? nativeStyle |= nativeResizable
			: nativeStyle &= ~nativeResizable;
	
		SetWindowLongPtrA(
			mNativeWindow,
			GWL_STYLE,
			static_cast<LONG_PTR>( nativeStyle)
		);
	}

	void Win32WindowFrame::Show(bool visible)
	{
		mSpecification.Visible = visible;
		ShowWindow(mNativeWindow, visible ? SW_SHOW : SW_HIDE);
	}

	LRESULT Win32WindowFrame::OnWindowProcedure(
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		if (WM_CLOSE == msg)
		{
			mParent->NotifyWindowFrameClosed(ObserverPtr<Win32WindowFrame>(this));
			return 0;
		}
		else if (WM_SIZE == msg)
		{
			std::uint32_t width = LOWORD(lParam);
			std::uint32_t height = HIWORD(lParam);
			
			mSpecification.Width = width;
			mSpecification.Height = height;

			mParent->NotifyWindowFrameResized(
				ObserverPtr<Win32WindowFrame>(this),
				width,
				height
			);

			return 0;
		}
		else if (WM_PAINT == msg)
		{
			PAINTSTRUCT ps = {};
			HDC deviceContext = BeginPaint(mNativeWindow, &ps);
			FillRect(deviceContext, &ps.rcPaint, (HBRUSH)COLOR_WINDOW);
			EndPaint(mNativeWindow, &ps);

			return 0;
		}
		else if (WM_KEYDOWN == msg)
		{
			mParent->NotifyWin32KeyDown(wParam, lParam);

			return 0;
		}
		else if (WM_KEYUP == msg)
		{
			mParent->NotifyWin32KeyUp(wParam, lParam);

			return 0;
		}
		else if (WM_SYSKEYDOWN == msg)
		{
			mParent->NotifyWin32KeyDown(wParam, lParam);

			return 0;
		}
		else if (WM_SYSKEYUP == msg)
		{
			mParent->NotifyWin32KeyUp(wParam, lParam);

			return 0;
		}

		return DefWindowProcA(mNativeWindow, msg, wParam, lParam);
	}
}

#endif // XZ_PLATFORM_IS(WIN32)
