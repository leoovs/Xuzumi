#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	struct WindowFrameSpecification
	{
		std::string Caption = "Xuzumi Window Frame";
		std::uint32_t Width = 1280;
		std::uint32_t Height = 720;
		bool Resizable = false;
		bool Visible = true;
	};

	class WindowFrame
	{
	public:
		virtual ~WindowFrame() = default;
	
		virtual const WindowFrameSpecification& GetSpecification() const = 0;
	
		virtual void SetCaption(std::string_view caption) = 0;
		virtual void SetSize(std::uint32_t width, std::uint32_t height) = 0;
		virtual void AllowResizing(bool resizable = true) = 0;
		virtual void Show(bool visible = true) = 0;
	};
}
