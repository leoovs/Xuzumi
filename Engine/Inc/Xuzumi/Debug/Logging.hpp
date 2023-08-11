#ifndef XUZUMI_DEBUG_LOGGING_HPP_
#define XUZUMI_DEBUG_LOGGING_HPP_

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/ObserverPtr.hpp"
#include "Xuzumi/TypeMeta/TypeID.hpp"

namespace Xuzumi
{
	enum class LogLevel
	{
		Debug,
		Info,
		Warning,
		Error,
		Count_
	};

	struct LogData
	{
		LogLevel Level = LogLevel::Debug;
		std::chrono::system_clock::time_point Timestamp;
		std::string SourceFileName;
		std::uint32_t LineNumber = -1;
		std::string Message;
		std::string ScopeName;
	
		LogData() = default;

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

	class ILogFormatter
	{
	public:
		virtual ~ILogFormatter() = default;
	
		virtual void FormatLogData(const LogData& data) = 0;
		virtual std::string_view GetFormattedLog() const = 0;
	};

	class ILogWriter
	{
	public:
		virtual ~ILogWriter() = default;

		virtual void Write(
			const LogData& data,
			ILogFormatter& formatter
		) = 0;
	};

	class Logger
	{
	public:
		void Log(const LogData& data);

		void SetFormatter(std::unique_ptr<ILogFormatter> formatter);
		void AddWriter(std::unique_ptr<ILogWriter> writer);

	private:
		std::unique_ptr<ILogFormatter> mFormatter;
		std::vector<std::unique_ptr<ILogWriter>> mWriters;
	};

	class LoggerConfigurator
	{
	public:
		explicit LoggerConfigurator(ObserverPtr<Logger> configuredLogger);

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

#endif // XUZUMI_DEBUG_LOGGING_HPP_
