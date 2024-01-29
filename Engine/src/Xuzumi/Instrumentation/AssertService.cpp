#include "Xuzumi/Instrumentation/AssertService.hpp"

namespace Xuzumi
{
	AssertHandler AssertService::CreateDefaultHandler()
	{
		return [](const AssertRecord& record)
		{
			assert(false && "Xuzumi: assertion failed");
		};
	}

	AssertHandler AssertService::CreateHandlerFromLogger(const Logger& logger)
	{
		return [loggerPtr = &logger](const AssertRecord& record)
		{
			loggerPtr->Log(
				LogRecordBuilder()
					.Level(LogLevel::Error)
					.AtFile(record.FileName)
					.AtFunc(record.FuncName)
					.AtLine(record.LineNumber)
					.Now()
					.Format(
						"Assertion failed: `%s`. Details: %s",
						record.UnmetConditionExpression.data(),
						record.Message.data()
					)
					.Finish()
			);
		};
	}

	void AssertService::Assert(const AssertRecord& record) const
	{
		mAssertDepth.fetch_add(1);
		if (mAssertDepth.load() > 1)
		{
			CreateDefaultHandler()(record);
		}
		else if (mHandler)
		{
			mHandler(record);
		}

		mAssertDepth.fetch_sub(1);
	}

	AssertService::AssertService(const AssertService& other)
		: mHandler(other.mHandler)
	{}

	AssertService::AssertService(AssertHandler handler)
		: mHandler(handler)
	{}

	AssertService& AssertService::operator=(const AssertService& other)
	{
		if (this == &other)
		{
			return *this;
		}

		mHandler = other.mHandler;

		return *this;
	}
}

