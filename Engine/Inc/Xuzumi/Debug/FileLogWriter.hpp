#ifndef XUZUMI_DEBUG_FILELOGWRITER_HPP_
#define XUZUMI_DEBUG_FILELOGWRITER_HPP_

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	class FileLogWriter : public ILogWriter
	{
	public:
		FileLogWriter(std::string_view fileName);
		~FileLogWriter() override = default;

		void Write(const LogData& data, ILogFormatter& formatter) override;

	private:
		std::ofstream mFile;
	};
}

#endif // XUZUMI_DEBUG_FILELOGWRITER_HPP_
