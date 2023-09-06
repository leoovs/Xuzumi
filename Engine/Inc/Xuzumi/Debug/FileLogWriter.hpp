/**
 * @file Xuzumi/Debug/FileLogWriter.hpp
 * 
 * @brief Defines a stock file log writer.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	/**
	 * @brief A stock log writer that outputs log entries to the specified file.
	 * 
	 * @warning The current implementation is not thread-safe, although it is an
	 * explicit requirement.
	 */
	class FileLogWriter : public ILogWriter
	{
	public:
	 	/**
		 * @brief Constructs a `FileLogWriter` object that allocates an output file
		 * stream object. 
		 */
		FileLogWriter(std::string_view fileName);
		
		/**
		 * @brief Destructs a `FileLogWriter` object and closes the output file stream object. 
		 */
		~FileLogWriter() override = default;

		/**
		 * @brief Writes a log entry to the output file stream.
		 * 
		 * @param data A `LogData` object containing logging metadata.
		 * @param formatter A formatter to query a cached string representation of
		 * the @p data from.
		 */
		void Write(const LogData& data, ILogFormatter& formatter) override;

	private:
		std::ofstream mFile;
	};
}
