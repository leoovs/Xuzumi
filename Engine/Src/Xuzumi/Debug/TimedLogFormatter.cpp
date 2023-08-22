#include "Xuzumi/Debug/TimedLogFormatter.hpp"

#include "Xuzumi/Core/StringUtils.hpp"

namespace Xuzumi
{
	void TimedLogFormatter::FormatLogData(const LogData& data)
	{	
		const char* levels[]
		{
			"DEBUG",
			"INFO ",
			"WARN ",
			"ERROR",
		};
		
		// NOTE: currently the logging system uses the system_clock to
		// pretty-print the time. However, in the future we might want to switch
		// back to the high_resolution_clock in order to display more precise timings.
		std::time_t time = std::chrono::system_clock::to_time_t(data.Timestamp);
		std::tm* timeInfo = std::localtime(&time);
		
		constexpr std::size_t kTimeReprBufferSize = 128;
		char timeReprBuffer[kTimeReprBufferSize]{};

		std::strftime(
			timeReprBuffer,
			kTimeReprBufferSize, 
			"%H:%M:%S",
			timeInfo
		);

		mFormattedLog = FormatString(
			"[%s] [%s] :: <%s, %d> :: %s :: %s",
			levels[std::size_t(data.Level)],
			timeReprBuffer,
			data.SourceFileName.data(),
			data.LineNumber,
			data.ScopeName.data(),
			data.Message.data()
		);
	}

	std::string_view TimedLogFormatter::GetFormattedLog() const
	{
		return mFormattedLog;
	}
}
