#include "Xuzumi/Debug/ConsoleLogWriter.hpp"

namespace Xuzumi
{
	void ConsoleLogWriter::Write(const LogData& data, ILogFormatter& formatter)
	{
		std::cout << formatter.GetFormattedLog() << std::endl;	
	}
}
