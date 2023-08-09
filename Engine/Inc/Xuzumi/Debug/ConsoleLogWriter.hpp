#ifndef XUZUMI_DEBUG_CONSOLELOGWRITER_HPP_
#define XUZUMI_DEBUG_CONSOLELOGWRITER_HPP_

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	class ConsoleLogWriter : public ILogWriter
	{
	public:
		void Write(const LogData& data, ILogFormatter& formatter) override;
	};
}

#endif // XUZUMI_DEBUG_CONSOLELOGWRITER_HPP_
