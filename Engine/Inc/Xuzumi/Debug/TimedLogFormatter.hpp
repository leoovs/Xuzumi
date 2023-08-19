#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	class TimedLogFormatter : public ILogFormatter
	{
	public:
		void FormatLogData(const LogData& data) override;
		std::string_view GetFormattedLog() const override;

	private:
		std::string mFormattedLog;
	};
}
