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

void Observe(Xuzumi::WeakPtr<Entity> e)
{
	if (e.Expired())
	{
		std::cout << "Weak reference expired\n";
		return;
	}

	std::cout << e.Lock().As<Person>()->Name << std::endl;
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

	Xuzumi::WeakPtr<Person> p;
	std::cout << "WeakPtr is uninitialized\n";
	Observe(p);

	{
		auto sp = Xuzumi::MakeShared<Person>("Lyosha");
		p = sp;
		std::cout << "WeakPtr initialized with SharedPtr\n";
		Observe(p);
	}

	std::cout << "SharedPtr has been destructed\n";
	Observe(p);
}
