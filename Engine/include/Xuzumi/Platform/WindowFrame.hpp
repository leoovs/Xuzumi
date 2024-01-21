/**
 * @file Xuzumi/Platform/WindowFrame.hpp
 *
 * @brief Defines the `WindowFrame` abstract class.
 */
#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	/**
	 * @brief Options of a window frame.
	 */
	struct WindowFrameSpecification
	{
		/**
		 * @brief The title of the window frame.
		 */
		std::string Caption = "Xuzumi Window Frame";

		/**
		 * @brief The width of the client area of the window frame.
		 */
		std::uint32_t Width = 1280;

		/**
		 * @brief The height of the client area of the window frame.
		 */
		std::uint32_t Height = 720;

		/**
		 * @brief Controls whether a user can resize the window frame.
		 *
		 * If this flag is set to `true`, the window frame will still be resizable
		 * programmatically.
		 */
		bool Resizable = false;

		/**
		 * @brief Controls whether the window frame is visible.
		 */
		bool Visible = true;
	};

	/**
	 * @brief A user interface component that is used to display graphics.
	 */
	class WindowFrame
	{
	public:
		/**
		 * @brief Releases the resources of the window frame.
		 */
		virtual ~WindowFrame() = default;

		/**
		 * @brief Queries the window frame specification that reflects its current
		 * state.
		 *
		 * @return The window frame specification.
		 */
		virtual const WindowFrameSpecification& GetSpecification() const = 0;

		/**
		 * @brief Sets the title of the window frame.
		 *
		 * @param caption A UTF-8 encoded string containing the title.
		 */
		virtual void SetCaption(std::string_view caption) = 0;

		/**
		 * @brief Sets the size of the window frame client area.
		 *
		 * @param width The client are width.
		 * @param height The client are height.
		 */
		virtual void SetSize(std::uint32_t width, std::uint32_t height) = 0;

		/**
		 * @brief Allows the user to resize the window frame.
		 *
		 * @param resizable Controls whether the  window frame is visible.
		 */
		virtual void AllowResizing(bool resizable = true) = 0;

		/**
		 * @brief Allows the user to see the window frame.
		 *
		 * @param visible Controls whether a user can see the window frame.
		 */
		virtual void Show(bool visible = true) = 0;
	};
}
