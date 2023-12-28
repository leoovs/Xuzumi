/**
 * @file Xuzumi/Debug/ConsoleLogWriter.hpp 
 * 
 * @brief Defines a stock console log writer. 
 */
#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	/**
	 * @brief A stock console log writer.
	 * 
	 * @warning The current implementation is not thread-safe, although it is an
	 * explicit requirement.
	 */
	class ConsoleLogWriter : public ILogWriter
	{
	public:
	 	/**
		 * @brief Displays a log entry in the console.
		 * 
		 * @param data A const reference to a `LogData` object containing logging
		 * metadata.
		 * @param formatter A formatter to query a cached string representation of
		 * the @p data from. 
		 */
		void Write(const LogData& data, ILogFormatter& formatter) override;
	};
}
