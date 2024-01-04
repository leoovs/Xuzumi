#pragma once

#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/Platform/WindowFrame.hpp"
#include "Xuzumi/Platform.Win32/Win32SDK.hpp"

namespace Xuzumi::Internal
{
	class Win32WindowClass;

	class Win32WindowFrame : public WindowFrame	
	{
	public:
		Win32WindowFrame(
			const WindowFrameSpecification& specification,
			HWND nativeWindow,
			ObserverPtr<Win32WindowClass> parent
		);
		~Win32WindowFrame() override;

		const WindowFrameSpecification& GetSpecification() const override;

		void SetCaption(std::string_view caption) override;
		void SetSize(std::uint32_t width, std::uint32_t height) override;
		void AllowResizing(bool resizable) override; 
		void Show(bool visible) override;

		HWND GetNativeWindow() const;
		LRESULT OnWindowProcedure(UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		WindowFrameSpecification mSpecification;
		HWND mNativeWindow = nullptr;
		ObserverPtr<Win32WindowClass> mParent;
	};
}
