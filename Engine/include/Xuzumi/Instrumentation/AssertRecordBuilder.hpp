#pragma once

#include "Xuzumi/Core/FuncName.hpp"
#include "Xuzumi/Instrumentation/AssertRecord.hpp"

#define XZ_INTERNAL_MAKE_ASSERT_RECORD(unmetCondition, ...) \
	(::Xuzumi::AssertRecordBuilder() \
		.Reason(#unmetCondition) \
		.AtFile(__FILE__) \
		.AtLine(__LINE__) \
		.AtFunc(XZ_FUNCNAME) \
		.Format(__VA_ARGS__) \
		.Finish())

namespace Xuzumi
{
	class AssertRecordBuilder
	{
	public:
		AssertRecordBuilder& Reason(std::string_view unmetConditionExpression);
		AssertRecordBuilder& AtFile(std::string_view fileName);
		AssertRecordBuilder& AtLine(std::int32_t LineNumber);
		AssertRecordBuilder& AtFunc(std::string_view FuncName);
		AssertRecordBuilder& Format(std::string_view format, ...);
		AssertRecordBuilder& Format();

		AssertRecord Finish();

	private:
		AssertRecord mProduct;
	};
}

