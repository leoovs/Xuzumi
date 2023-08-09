#include "Xuzumi/Debug/TimestampLogFormatter.hpp"

#include "Xuzumi/Core/StringUtils.hpp"

namespace Xuzumi
{
	void TimestampLogFormatter::FormatLogData(const LogData& data)
	{
		std::chrono::nanoseconds dur = data.Timestamp.time_since_epoch();
		auto nanosSinceStart = dur.count();
	
		const char* levels[]
		{
			"DEBUG",
			"INFO ",
			"WARN ",
			"ERROR",
		};

		// TODO: actually display timestamp, which is currenly missing.

		mFormattedLog = FormatString(
			"[%s] :: <%s, %d> :: %s :: %s",
			levels[std::size_t(data.Level)],
			data.SourceFileName.data(),
			data.LineNumber,
			data.ScopeName.data(),
			data.Message.data()
		);
	}

	std::string_view TimestampLogFormatter::GetFormattedLog() const
	{
		return mFormattedLog;
	}
}
