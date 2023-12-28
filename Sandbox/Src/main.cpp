#include <Xuzumi/Xuzumi.hpp>

struct alignas(1024) Resource
{
	Resource()
	{
		XZ_LOG(Info, "Created: %p", this);
	}

	~Resource()
	{
		XZ_LOG(Info, "Destroyed: %p", this);
	}
};

class Factory
{
public:
	Xuzumi::SharedPtr<Resource> Create()
	{
		return Xuzumi::SharedPtr<Resource>(
			new Resource(),
			mGuard.MakeDangleProtectedDeleter(&Factory::Destroy)	
		);
	}

private:
	void Destroy(Resource* resource)
	{
		delete resource;
	}

	Xuzumi::Internal::FactoryExpirationGuard<Factory> mGuard = this;
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
}
