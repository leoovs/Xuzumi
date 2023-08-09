#ifndef XUZUMI_DEBUG_DEBUGSESSION_HPP_
#define XUZUMI_DEBUG_DEBUGSESSION_HPP_

#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Core/FuncName.hpp"
#include "Xuzumi/Debug/Logging.hpp"
#include "Xuzumi/Debug/Assertion.hpp"

// TODO: introduce custom Xuzumi logging macro (XZ_LOG_ENABLED).
#ifdef NDEBUG
#	define XZ_LOG(level, format, ...)
#else
#	define XZ_LOG(level, format, ...) ::Xuzumi::DebugSession::Get() \
	.LogInternal( \
		{ \
			::Xuzumi::LogLevel::level, \
			std::chrono::high_resolution_clock::now(), \
			__FILE__, \
			__LINE__, \
			format, \
			XZ_FUNCNAME \
		}, \
		##__VA_ARGS__ \
	)
#endif

// TODO: introduce custom Xuzumi logging macro (XZ_ASSERT_ENABLED).
#ifdef NDEBUG
#	define XZ_ASSERT(expr, format, ...)
#else
#	define XZ_ASSERT(expr, format, ...) (bool(expr) || \
	::Xuzumi::DebugSession::Get() \
	.RaiseAssertionInternal( \
		{ \
			__FILE__, \
			__LINE__, \
			format, \
			#expr, \
			XZ_FUNCNAME \
		}, \
		##__VA_ARGS__ \
	) \
)
#endif

namespace Xuzumi
{
	class DebugSession
	{
	public:
		static DebugSession& Get();

		void ConfigureLogger(Delegate<void(LoggerConfigurator)> config);
		void LogInternal(LogData data, ...);

		void ConfigureAssertion(Delegate<void(AssertionConfigurator)> config);
		bool RaiseAssertionInternal(AssertionData data, ...);
		AssertionDispatcher& GetAssertionDispatcher();

	private:
		DebugSession() = default;
		DebugSession(const DebugSession& other) = delete;
	
		Logger mLogger;
		AssertionDispatcher mAssertionDispatcher;
	};
}

#endif // XUZUMI_DEBUG_DEBUGSESSION_HPP_
