#include "Xuzumi/Instrumentation/LogFormatter.hpp"

#include "Xuzumi/Core/FormatString.hpp"

namespace Xuzumi
{
	std::unique_ptr<LogFormatter> MinimalLogFormatter::Clone() const
	{
		return std::make_unique<MinimalLogFormatter>(*this);
	}

	std::string MinimalLogFormatter::FormatRecord(const LogRecord& record) const
	{
		constexpr std::string_view cLevel[]
		{
			"TRACE",
			"INFO ",
			"WARN ",
			"ERROR",
		};

		std::filesystem::path sourceFilePath(record.FileName);
		std::string fileName = sourceFilePath.filename().string();

		return FormatString(
			"[%s] %s `%s` (%s, %d)",
			cLevel[std::size_t(record.Level)].data(),
			record.Message.data(),
			record.FuncName.data(),
			fileName.data(),
			record.LineNumber
		);
	}

	std::unique_ptr<LogFormatter> VerboseLogFormatter::Clone() const
	{
		return std::make_unique<VerboseLogFormatter>(*this);
	}

	std::string
	VerboseLogFormatter::FormatRecord(const LogRecord& record) const
	{
		constexpr std::string_view cLevel[]
		{
			"Trace",
			"Info ",
			"Warn ",
			"Error",
		};

		// TODO: add timestampt entry.
		return FormatString(
			"[%s] %s (%s, %d, `%s`)",
			cLevel[std::size_t(record.Level)].data(),
			record.Message.data(),
			record.FileName.data(),
			record.LineNumber,
			record.FuncName.data()
		);
	}
}

