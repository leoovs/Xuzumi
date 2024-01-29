#pragma once

#include "Xuzumi/Instrumentation/LogRecord.hpp"

namespace Xuzumi
{
	class LogFormatter
	{
	public:
		virtual ~LogFormatter() = default;

		virtual std::unique_ptr<LogFormatter> Clone() const = 0;

		virtual std::string FormatRecord(const LogRecord& record) const = 0;
	};

	class MinimalLogFormatter : public LogFormatter
	{
	public:
		~MinimalLogFormatter() override = default;

		std::unique_ptr<LogFormatter> Clone() const override;

		std::string FormatRecord(const LogRecord& record) const override;
	};

	class VerboseLogFormatter : public LogFormatter
	{
	public:
		~VerboseLogFormatter() override = default;

		std::unique_ptr<LogFormatter> Clone() const override;

		std::string FormatRecord(const LogRecord& record) const override;
	};
}

