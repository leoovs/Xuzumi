#include "Xuzumi/Debug/FileLogWriter.hpp"

namespace Xuzumi
{
	FileLogWriter::FileLogWriter(std::string_view fileName)
		: mFile(fileName.data())
	{
	}
	
	void FileLogWriter::Write(const LogData &data, ILogFormatter &formatter)
	{
		if (mFile.is_open())
		{
			mFile << formatter.GetFormattedLog() << '\n';
		}
	}
}
