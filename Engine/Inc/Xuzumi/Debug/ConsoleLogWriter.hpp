#pragma once

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
