#include <Xuzumi/Xuzumi.hpp>
#include <thread>
#include <memory>

struct Resource
{
	Resource()
	{
		XZ_LOG(Info, "Created");
	}

	~Resource()
	{
		XZ_LOG(Info, "Destroyed");
	}
};

class Factory
{
public:
	Xuzumi::SharedPtr<Resource> Create()
	{
		return Xuzumi::SharedPtr<Resource>(
			new Resource(),
			mGuard.MakeDangleProtectedDeleter<Resource>(
				&Factory::Destroy
			)	
		);
	}

private:
	void Destroy(Resource* resource)
	{
		delete resource;
	}

	// TODO: maybe rename to `ResourceExpirationGuard`
	Xuzumi::Internal::ResourceDeleterGuard<Factory> mGuard = this;
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

	XZ_LOG(Info, "Compiled with %s", Xuzumi::GetCompilerName().data());

	Xuzumi::SharedPtr<Factory> factory(new Factory());
	Xuzumi::SharedPtr<Resource> res;
	res = factory->Create();
}
