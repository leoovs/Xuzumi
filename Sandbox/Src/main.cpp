#include <Xuzumi/Xuzumi.hpp>
#include <thread>

struct Entity
{
	Entity()
	{
		XZ_LOG(Info, "Created parent");
	}

	~Entity()
	{
		XZ_LOG(Info, "Destroyed parent");
	}
};

struct Person : Entity
{
	std::string Name;

	Person(std::string_view name = std::string_view())
		: Name(name)
	{
		XZ_LOG(Info, "Created child: %s", Name.data());
	}

	~Person()
	{
		XZ_LOG(Info, "Destroyed child: %s", Name.data());
	}
};

int main()
{
	Xuzumi::DebugSession::Get().ConfigureLogger(
		[](Xuzumi::LoggerConfigurator configurator)
		{
			configurator.SetFormatter<Xuzumi::TimedLogFormatter>();
			configurator.AddWriter<Xuzumi::FileLogWriter>("Xuzumi.log");
			configurator.AddWriter<Xuzumi::ConsoleLogWriter>();
		}
	);

	Xuzumi::DebugSession::Get().ConfigureAssertion(
		[](Xuzumi::AssertionConfigurator configurator)
		{
			configurator.SetLoggerAsHandler();
		}
	);

	Xuzumi::SharedPtr<Entity> entity;
	entity = Xuzumi::MakeShared<Person>("Hello");
	 
	auto hack = entity.AsUnsafe<std::string>();
	std::cout
		<< hack.UseCount()
		<< " - " << *hack
		<< " (" << hack->size() << ')'
		<< std::endl;
}
