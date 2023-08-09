#include <Xuzumi/Debug/DebugSession.hpp>
#include <Xuzumi/Debug/FileLogWriter.hpp>
#include <Xuzumi/Debug/ConsoleLogWriter.hpp>
#include <Xuzumi/Debug/TimestampLogFormatter.hpp>

int main()
{
	Xuzumi::DebugSession::Get().ConfigureLogger(
		[](Xuzumi::LoggerConfigurator configurator)
		{
			configurator.SetFormatter<Xuzumi::TimestampLogFormatter>();
			configurator.AddWriter<Xuzumi::ConsoleLogWriter>();
			configurator.AddWriter<Xuzumi::FileLogWriter>("Xuzumi.log");
		}
	);

	Xuzumi::DebugSession::Get().ConfigureAssertion(
		[](Xuzumi::AssertionConfigurator configurator)
		{
			configurator.SetLoggerAsHandler();
		}
	);

	XZ_LOG(Info, "Testing logging system");

	XZ_ASSERT(5 == 5.0000001f, "%s )))", "Message");
	XZ_ASSERT(5 == 5.00001f, "%s )))", "Message");
}
