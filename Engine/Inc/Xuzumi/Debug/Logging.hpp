/**
 * @file Xuzumi/Debug/Logging.hpp  
 * 
 * @brief Defines classes and structures related to the logging facility.
 */

#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Memory/ObserverPtr.hpp"
#include "Xuzumi/TypeMeta/TypeID.hpp"

namespace Xuzumi
{
	/**
	 * @brief Enumerates logging levels. 
	 */
	enum class LogLevel
	{
		Debug,
		Info,
		Warning,
		Error,
		Count_
	};

	/**
	 * @brief Structure containing logging metadata. 
	 */
	struct LogData
	{
		/**
		 * @brief A log level.
		 */
		LogLevel Level = LogLevel::Debug;

		/**
		 * @brief A time point when a log entry occurred.
		 */
		std::chrono::system_clock::time_point Timestamp;

		/**
		 * @brief The name of the source file where a log entry occurred.
		 */
		std::string SourceFileName;
		
		/**
		 * @brief The line in the source file where a log entry occurred. 
		 */
		std::uint32_t LineNumber = -1;

		/**
		 * @brief A descriptive log message.
		 * 
		 * Ideally, provides context information and task execution status. 
		 */
		std::string Message;

		/**
		 * @brief The name of the function or method where a log entry occurred. 
		 */
		std::string ScopeName;
	
		/**
		 * @brief Default constructor.
		 * 
		 * Constructs a `LogData` object that contains meaningless logging metadata.
		 */
		LogData() = default;

		/**
		 * @brief Constructs a `LogData` object with provided information.
		 * 
		 * @param level A log level.
		 * @param timestamp A time point when a log entry occurred.
		 * @param sourceFileName The name of the source file where a log entry
		 * occurred.
		 * @param lineNumber The line number in the source file where a log entry
		 * occurred.
		 * @param message A descriptive message providing context information and
		 * task execution status.
		 * @param scopeName The name of the function or method where a log entry
		 * occurred.
		 */
		LogData(
			LogLevel level,
			std::chrono::system_clock::time_point timestamp,
			std::string_view sourceFileName,
			std::uint32_t lineNumber,
			std::string_view message,
			std::string_view scopeName
		)
			: Level(level)
			, Timestamp(timestamp)
			, SourceFileName(sourceFileName)
			, LineNumber(lineNumber)
			, Message(message)
			, ScopeName(scopeName)
		{
		}
	};

	/**
	 * @brief Logging metadata formatting interface.
	 * 
	 * Defines an interface for creating and caching string representation of the
	 * `LogData` structure object.
	 * Thread-safety is the only requirement for the `ILogFormatter`
	 * implementation.
	 */
	class ILogFormatter
	{
	public:
		/**
		 * @brief Virtual destructor enforces safe RAII. 
		 */
		virtual ~ILogFormatter() = default;

		/**
		 * @brief Creates and caches string representation of @p data.
		 * 
		 * @param data An object of type `LogData`. 
		 */
		virtual void FormatLogData(const LogData& data) = 0;

		/**
		 * @brief Retrieves cached string representation of logging metadata.
		 * 
		 * @return Cached string representation of `LogData`. 
		 */
		virtual std::string_view GetFormattedLog() const = 0;
	};

	/**
	 * @brief Logging metadata writing interface.
	 * 
	 * Defines an interface for writing logging metadata to an
	 * implementation-defined entity (file, console, socket, ...). 
	 * Thread-safety is the only requirement for the `ILogWriter` implementation.
	 */
	class ILogWriter
	{
	public:
	 	/**
		 * @brief Virtual destructor enforces safe RAII. 
		 */
		virtual ~ILogWriter() = default;

		/**
		 * @brief Writes @p data that may be formatted with @p formatter.
		 * 
		 * @param data Const reference to an object of type `LogData`.
		 * @param formatter Formatter to query cached string representation from.
		 */
		virtual void Write(
			const LogData& data,
			ILogFormatter& formatter
		) = 0;
	};

	/**
	 * @brief Directs the logging facility.
	 * 
	 * `Logger` is responsible dispatching logging data to a formatter and
	 * writers.
	 * Typically, users should not access objects of this class directly. Logging
	 * facility must be utilized via dedicated macros: `XZ_LOG` and others. If
	 * any modification to the `Logger` is required, then the `LoggerConfigurator`
	 * class should be used instead.
	 * 
	 * @see XZ_LOG
	 * @see Xuzumi::LoggerConfigurator 
	 */
	class Logger
	{
	public:
	 	/**
		 * @brief Log the provided logging metadata.
		 */
		void Log(const LogData& data);

		/**
		 * @brief Sets a custom formatter instance.
		 * 
		 * @param formatter A custom formatter instance. 
		 */
		void SetFormatter(std::unique_ptr<ILogFormatter> formatter);
		
		/**
		 * @brief Add a new writer to the logger.
		 * 
		 * Added writer instances cannot be removed. Besides, there is no check
		 * whether a writer has already been added provided. 
		 * 
		 * @param writer A writer instance.
		 */
		void AddWriter(std::unique_ptr<ILogWriter> writer);

	private:
		std::unique_ptr<ILogFormatter> mFormatter;
		std::vector<std::unique_ptr<ILogWriter>> mWriters;
	};

	/**
	 * @brief Provides utility methods to make the `Logger` configuration more
	 * clean and reduces some boilerplate code. 
	 */
	class LoggerConfigurator
	{
	public:
		/**
		 * @brief Constructs a `LoggerConfigurator` object that references a
		 * `Logger` object.
		 * 
		 * Internally, the `Logger` object is reference via a raw pointer. Instances
		 * of this class should not be constructed directly by user or used
		 * globally. In order to localize usage of this class instances, consider
		 * using the `DebugSession` class functionality.
		 * 
		 * @param configuredLogger A pointer to the `Logger` object that is to be
		 * configured.
		 * 
		 * @see Xuzumi::DebugSession::ConfigureLogger
		*/
		explicit LoggerConfigurator(ObserverPtr<Logger> configuredLogger);

		/**
		 * @brief Sets a new formatter to the configured logger.
		 * 
		 * @tparam FormatterT A type of the formatter. The @p FormatterT must
		 * implement the `ILogFormatter` interface.
		 * @tparam ArgsT Types of arguments that are passed to the @p FormatterT
		 * constructor.
		 * 
		 * @param args Arguments that are passed to  the @p FormatterT constructor.
		 */
		template<typename FormatterT, typename... ArgsT>
		FormatterT& SetFormatter(ArgsT&&... args)
		{
			auto formatter = std::make_unique<FormatterT>(
				std::forward<ArgsT>(args)...
			);
			FormatterT& formatterRef = *formatter;
		
			mConfiguredLogger->SetFormatter(std::move(formatter));
			
			return formatterRef;
		}


		/**
		 * @brief Adds a new writer to the configured logger.
		 * 
		 * @tparam WriterT A type of the writer. The @p WriterT must implement the
		 * `ILogWriter` interface.
		 * @tparam Types of the arguments that are passed to the @p WriterT
		 * constructor.
		 * 
		 * @param args Arguments that are passed to the @p WriterT constructor. 
		 */
		template<typename WriterT, typename... ArgsT>
		WriterT& AddWriter(ArgsT&&... args)
		{
			auto writer = std::make_unique<WriterT>(std::forward<ArgsT>(args)...);
			WriterT& writerRef = *writer;

			mConfiguredLogger->AddWriter(std::move(writer));

			return writerRef;
		}

	private:
		ObserverPtr<Logger> mConfiguredLogger;
	};
}
