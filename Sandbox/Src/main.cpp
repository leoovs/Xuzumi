#include <Xuzumi/Xuzumi.hpp>
#include <thread>
#include <memory>

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

void worker(Xuzumi::SharedPtr<Entity> e)
{
	std::cout << e.As<Person>()->Name << std::endl;
	std::cout << e.UseCount() << std::endl;
}

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

	auto e = Xuzumi::MakeShared<Person>("Vasya");
	
	std::thread t1(worker, e);
	std::thread t2(worker, e);

	std::cout << e.UseCount() << std::endl;

	t1.detach();
	t2.detach();

	std::this_thread::sleep_for(std::chrono::seconds(3));
}
