/**
 * @file Xuzumi/Debug/TimedLogFormatter.hpp
 * 
 * @brief Defines a stock log formatter.
 */
#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	/**
	 * @brief A stock log formatter.
	 */
	class TimedLogFormatter : public ILogFormatter
	{
	public:
	 	/**
		 * @brief Converts a `LogData` object into a string representation and
		 * caches it. 
		 * 
		 * @param data A `LogData` object containing logging metadata.
		 */
		void FormatLogData(const LogData& data) override;
		
		/**
		 * @brief Retrieves a cached string representation of a log entry.
		 * 
		 * @return An `std::string_view` instance containing cached string
		 * representation of a log entry.
		 */
		std::string_view GetFormattedLog() const override;

	private:
		std::string mFormattedLog;
	};
}
