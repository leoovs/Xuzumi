#include "Xuzumi/Instrumentation/LogRecordBuilder.hpp"

#include "Xuzumi/Core/FormatString.hpp"

namespace Xuzumi
{
	LogRecordBuilder& LogRecordBuilder::Level(LogLevel level)
	{
		mProduct.Level = level;
		return *this;
	}

	LogRecordBuilder& LogRecordBuilder::AtSource(std::string_view fileName)
	{
		mProduct.FileName = fileName;
		return *this;
	}

	LogRecordBuilder& LogRecordBuilder::AtFunc(std::string_view scopeName)
	{
		mProduct.FuncName = scopeName;
		return *this;
	}

	LogRecordBuilder& LogRecordBuilder::AtLine(std::int32_t lineNumber)
	{
		mProduct.LineNumber = lineNumber;
		return *this;
	}

	LogRecordBuilder& LogRecordBuilder::Now()
	{
		mProduct.HighResTimestamp = std::chrono::high_resolution_clock::now();
		mProduct.Timestamp = std::chrono::system_clock::now();
		return *this;
	}

	LogRecordBuilder& LogRecordBuilder::Format(std::string_view format, ...)
	{
		va_list args;
		va_start(args, format);
		mProduct.Message = FormatStringV(format, args);
		va_end(args);

		return *this;
	}

	LogRecord LogRecordBuilder::Finish()
	{
		LogRecord product = std::move(mProduct);
		return product;
	}
}

