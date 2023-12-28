/**
 * @file Xuzumi/Debug/DebugSession.hpp 
 *
 * @brief Defines the `DebugSession` class, `XZ_LOG` and `XZ_ASSERT` macros.
 */

#pragma once

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
			std::chrono::system_clock::now(), \
			__FILE__, \
			__LINE__, \
			format, \
			XZ_FUNCNAME \
		}, \
		##__VA_ARGS__ \
	)
#endif

// TODO: introduce custom Xuzumi assertion macro (XZ_ASSERT_ENABLED).
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
	/**
	 * @brief Singleton class that represents program global debug session.
	 */
	class DebugSession
	{
	public:
		/**
		 * @brief Retrieve the `DebugSession` instance.
		 * 
		 * @return A reference to a `DebugSession` object.
		 */
		static DebugSession& Get();

		/**
		 * @brief Handles user-defined logger configuration.
		 * 
		 * @param config User-defined configuration logic.
		 * 
		 * #### Usage 
		 * @code{.cpp}
		 * Xuzumi::DebugSession::Get().ConfigureLogger(
		 *   [additionalData](Xuzumi::LoggerConfigurator configurator)
		 *   {
		 *     // Utilize `configurator` to configure `Logger`.
		 *   }
		 * );
		 * @endcode
		 */
		void ConfigureLogger(Delegate<void(LoggerConfigurator)> config);

		/**
		 * @brief Composes a log entry my formatting passed data and hands it over
		 * to the `Logger`.
		 * 
		 * This method should not be used manually, instead the `XZ_LOG` macro
		 * should be utilized.
		 *
		 * @param data Premature logging metadata.
		 * @param ... Arguments to be formatted into the log message.
		 */
		void LogInternal(LogData data, ...);

		/**
		 * Handles user-defined assertion configuration.
		 * 
		 * @param config User-defined configuration logic.
		 *
		 * #### Usage 
		 * @code{.cpp}
		 * Xuzumi::DebugSession::Get().ConfigureAssertion(
		 *   [additionalData](Xuzumi::AssertionConfigurator configurator)
		 *   {
		 *     // Utilize `configurator` to configure `AssertionDispatcher`.
		 *   }
		 * );
		 * @endcode
		 */
		void ConfigureAssertion(Delegate<void(AssertionConfigurator)> config);

		/**
		 * @brief Prepares an `AssertionData` object and hands it over to the
		 * `AssertionDispatcher`. 
		 * 
		 * This method should not be used manually, instead the `XZ_ASSERT` macro
		 * should be utilized.
		 *
		 * @param data Premature assertion data.
		 * @param ... Arguments to be formatted into the assertion message.
		 */
		bool RaiseAssertionInternal(AssertionData data, ...);

	private:
		DebugSession() = default;
		DebugSession(const DebugSession& other) = delete;
	
		Logger mLogger;
		AssertionDispatcher mAssertionDispatcher;
	};
}
