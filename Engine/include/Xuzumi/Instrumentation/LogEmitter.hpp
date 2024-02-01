#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	class LogEmitter
	{
	public:
		virtual ~LogEmitter() = default;

		virtual std::unique_ptr<LogEmitter> Clone() const = 0;

		virtual void Emit(std::string_view log) = 0;
	};

	class ConsoleLogEmitter : public LogEmitter
	{
	public:
		~ConsoleLogEmitter() override = default;

		std::unique_ptr<LogEmitter> Clone() const override;

		void Emit(std::string_view log) override;
	};

	class FileLogEmitter : public LogEmitter
	{
	public:
		FileLogEmitter(std::string_view fileName);

		~FileLogEmitter() override = default;

		std::unique_ptr<LogEmitter> Clone() const override;

		void Emit(std::string_view log) override;

	private:
		std::shared_ptr<std::ofstream> mLogFile;
		std::shared_ptr<std::mutex> mLogFileMutex;
	};
}

