#include "Xuzumi/Instrumentation/LogEmitter.hpp"

namespace Xuzumi
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// ConsoleLogEmitter
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	std::unique_ptr<LogEmitter> ConsoleLogEmitter::Clone() const
	{
		return std::make_unique<ConsoleLogEmitter>(*this);
	}

	void ConsoleLogEmitter::Emit(std::string_view log)
	{
		std::cout << log << '\n';
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// FileLogEmitter
	//
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	std::unique_ptr<LogEmitter> FileLogEmitter::Clone() const
	{
		return std::make_unique<FileLogEmitter>(*this);
	}

	FileLogEmitter::FileLogEmitter(std::string_view fileName)
		: mLogFile(std::make_shared<std::ofstream>(fileName.data()))
		, mLogFileMutex(std::make_shared<std::mutex>())
	{}

	void FileLogEmitter::Emit(std::string_view log)
	{
		std::scoped_lock logFileLock(*mLogFileMutex);

		if (!mLogFile->is_open())
		{
			return;
		}

		*mLogFile << log << '\n';

		// REVIEW: possible performance issues?
		mLogFile->flush();
	}
}

