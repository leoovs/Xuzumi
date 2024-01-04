#pragma once

#include "Xuzumi/Messaging/Event.hpp"
#include "Xuzumi/Platform.Win32/Win32SDK.hpp"

namespace Xuzumi::Internal
{
	class Win32KeyDownEvent : public Event<Win32KeyDownEvent>
	{
	public:
		WPARAM MessageWParam;	
		LPARAM MessageLParam;

		Win32KeyDownEvent(WPARAM messageWParam, LPARAM messageLParam)
			: MessageWParam(messageWParam)
			, MessageLParam(messageLParam)
		{}
	};

	class Win32KeyUpEvent : public Event<Win32KeyUpEvent>
	{
	public:
		WPARAM MessageWParam;	
		LPARAM MessageLParam;

		Win32KeyUpEvent(WPARAM messageWParam, LPARAM messageLParam)
			: MessageWParam(messageWParam)
			, MessageLParam(messageLParam)
		{}
	};
}
