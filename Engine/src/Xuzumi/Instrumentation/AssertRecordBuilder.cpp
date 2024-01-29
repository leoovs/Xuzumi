#include "Xuzumi/Instrumentation/AssertRecordBuilder.hpp"

#include "Xuzumi/Core/FormatString.hpp"

namespace Xuzumi
{
	AssertRecordBuilder& AssertRecordBuilder::Reason(
		std::string_view unmetConditionExpression
	)
	{
		mProduct.UnmetConditionExpression = unmetConditionExpression;
		return *this;
	}

	AssertRecordBuilder& AssertRecordBuilder::AtFile(std::string_view fileName)
	{
		mProduct.FileName = fileName;
		return *this;
	}

	AssertRecordBuilder& AssertRecordBuilder::AtLine(std::int32_t lineNumber)
	{
		mProduct.LineNumber = lineNumber;
		return *this;
	}

	AssertRecordBuilder& AssertRecordBuilder::AtFunc(std::string_view funcName)
	{
		mProduct.FuncName = funcName;
		return *this;
	}

	AssertRecordBuilder& AssertRecordBuilder::Format(std::string_view format, ...)
	{
		va_list args;
		va_start(args, format);
		mProduct.Message = FormatStringV(format, args);
		va_end(args);

		return *this;
	}

	AssertRecordBuilder& AssertRecordBuilder::Format()
	{
		return *this;
	}

	AssertRecord AssertRecordBuilder::Finish()
	{
		AssertRecord result = std::move(mProduct);
		return result;
	}
}

