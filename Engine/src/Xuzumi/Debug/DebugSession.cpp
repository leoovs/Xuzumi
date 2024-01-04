#include "Xuzumi/Debug/DebugSession.hpp"

#include "Xuzumi/Core/StringUtils.hpp"

namespace Xuzumi
{
	DebugSession& DebugSession::Get()
	{
		static DebugSession sInstance;
		return sInstance;
	}

	void DebugSession::ConfigureLogger(
		Delegate<void(LoggerConfigurator)> config
	)
	{
		if (config)
		{
			config(LoggerConfigurator(ObserverPtr<Logger>(&mLogger)));
		}
	}

	void DebugSession::LogInternal(LogData data, ...)
	{
		va_list args;
		va_start(args, data);
		data.Message = FormatString(data.Message, args);
		va_end(args);

		mLogger.Log(data);
	}

	void DebugSession::ConfigureAssertion(
		Delegate<void(AssertionConfigurator)> config
	)
	{
		if (config)
		{
			config(
				AssertionConfigurator(
					ObserverPtr<AssertionDispatcher>(&mAssertionDispatcher)
				)
			);
		}
	}

	bool DebugSession::RaiseAssertionInternal(AssertionData data, ...)
	{
		va_list args;
		va_start(args, data);
		data.Message = FormatString(data.Message, args);
		va_end(args);

		mAssertionDispatcher.Raise(data);

		return {};
	}
}
