#include "Xuzumi/Debug/Assertion.hpp"

#include "Xuzumi/Core/StringUtils.hpp"
#include "Xuzumi/Debug/DebugSession.hpp"

namespace Xuzumi
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// 
	// AssertionDispatcher
	// 
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	AssertionDispatcher::AssertionDispatcher()
		: mHandler(this, &AssertionDispatcher::DefaultAssertionHandler)
	{
	}

	void AssertionDispatcher::Raise(AssertionData data)
	{
		if (!mAssertionStack.empty())
		{
			mAssertionStack.push(data);
			FallbackToDefaultHandler();
			return;
		}

		mAssertionStack.push(data);
		mHandler(data);

		if (!mAssertionStack.empty())
		{
			mAssertionStack.pop();
		}
	}

	void AssertionDispatcher::SetHandler(AssertionHandler handler)
	{
		mHandler = handler
			? handler
			: AssertionHandler(
				this,
				&AssertionDispatcher::DefaultAssertionHandler
			);
	}

	void AssertionDispatcher::DefaultAssertionHandler(
		const AssertionData& data
	)
	{
		std::cerr
			<< "Assertion failed: " << data.FalseExpression << "\n\tat "
			<< data.ScopeName << " <" << data.SourceFileName << ", " << data.Line
			<< ">\n\tDetails: " << data.Message << std::endl;
	}

	void AssertionDispatcher::FallbackToDefaultHandler()
	{
		while (!mAssertionStack.empty())
		{
			DefaultAssertionHandler(mAssertionStack.top());
			mAssertionStack.pop();
		}
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// 
	// AssertionConfigurator
	// 
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	AssertionConfigurator::AssertionConfigurator(
		ObserverPtr<AssertionDispatcher> configuredDispatcher
	)
		: mConfiguredDispatcher(configuredDispatcher)
	{
	}

	void AssertionConfigurator::SetHandler(AssertionHandler handler)
	{
		mConfiguredDispatcher->SetHandler(handler);
	}

	void AssertionConfigurator::SetLoggerAsHandler()
	{
		SetHandler(
			[](const AssertionData& data)
			{
				XZ_LOG(
					Error,
					"\nAssertion failed: %s\n"
					"\tat %s <%s, %d>\n"
					"\tDetails: %s",
					data.FalseExpression.data(),
					data.ScopeName.data(),
					data.SourceFileName.data(),
					data.Line,
					data.Message.data()
				);
			}
		);
	}
}
