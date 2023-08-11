#include <Xuzumi/Xuzumi.hpp>

#include "App.hpp"

int main()
{
	Xuzumi::DebugSession::Get().ConfigureLogger(
		[](Xuzumi::LoggerConfigurator configurator)
		{
			configurator.SetFormatter<Xuzumi::TimedLogFormatter>();
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

	XZ_LOG(Info, "Hello, %s", "World!");

	Xuzumi::EventBus events;
	App app((Xuzumi::ObserverPtr<Xuzumi::EventBus>(&events)));
}
