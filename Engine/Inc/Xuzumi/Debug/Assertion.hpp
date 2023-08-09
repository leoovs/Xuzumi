#ifndef XUZUMI_DEBUG_ASSERTION_HPP_
#define XUZUMI_DEBUG_ASSERTION_HPP_

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/Delegate.hpp"
#include "Xuzumi/Core/ObserverPtr.hpp"

namespace Xuzumi
{
	struct AssertionData
	{
		std::string SourceFileName;
		std::uint32_t Line;
		std::string Message;
		std::string FalseExpression;
		std::string ScopeName;

		AssertionData() = default;

		AssertionData(
			std::string_view sourceFileName,
			std::uint32_t line,
			std::string_view message,
			std::string_view falseExpression,
			std::string_view scopeName
		)
			: SourceFileName(sourceFileName)
			, Line(line)
			, Message(message)
			, FalseExpression(falseExpression)
			, ScopeName(scopeName)
		{
		}
	};

	using AssertionHandler = Delegate<void(const AssertionData&)>;

	class AssertionDispatcher
	{
	public:
		AssertionDispatcher();
		AssertionDispatcher(const AssertionDispatcher&) = delete;

		void Raise(AssertionData data);

		void SetHandler(AssertionHandler handler);

		void DefaultAssertionHandler(const AssertionData& data);

	private:
		void FallbackToDefaultHandler();

		AssertionHandler mHandler;
		std::stack<AssertionData> mAssertionStack;
	};

	class AssertionConfigurator
	{
	public:
		explicit AssertionConfigurator(
			ObserverPtr<AssertionDispatcher> configuredDispatcher
		);

		void SetHandler(AssertionHandler handler);
		void SetLoggerAsHandler();

	private:
		ObserverPtr<AssertionDispatcher> mConfiguredDispatcher;
	};
}

#endif // XUZUMI_DEBUG_ASSERTION_HPP_
