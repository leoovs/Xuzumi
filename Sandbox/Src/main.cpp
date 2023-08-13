#include <Xuzumi/Xuzumi.hpp>

#include "App.hpp"

struct Entity
{
	Entity()
	{
		XZ_LOG(Info, "Created: %p", this);
	}

	~Entity()
	{
		XZ_LOG(Info, "Destroyed: %p", this);
	}
};

class Person : public Entity
{
public:
	~Person()
	{
		XZ_LOG(Info, "Destroyed: %p", this);
	}
};

struct RESOURCE_DESC
{
	std::size_t ByteWidth = 1;
	bool Mappable = true;

	const void* Subresource = nullptr;
	std::size_t SubresourceSize = 0;
};

class IResource
{
public:
	virtual ~IResource() = default;
};

class IDevice
{
public:
	virtual ~IDevice() = default;

	virtual Xuzumi::SharedPtr<IResource> CreateResource(
		const RESOURCE_DESC& desc
	) = 0;

	virtual void* Map(Xuzumi::SharedPtr<IResource> resource) = 0;
	virtual void Unmap(Xuzumi::SharedPtr<IResource> resource) = 0;
};

Xuzumi::SharedPtr<IDevice> CreateDevice();

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

	std::shared_ptr<Entity> e(new Person());
}

class DummyResource : public IResource
{
public:
	DummyResource(
		const RESOURCE_DESC& desc,
		void* memory
	)
		: mDesc(desc)
		, mMemory(memory)
	{
		XZ_LOG(Info, "%p created", this);

		if (desc.Subresource && desc.SubresourceSize)
		{
			auto srcBegin = reinterpret_cast<const char*>(desc.Subresource);
			auto srcEnd = srcBegin + desc.SubresourceSize;

			std::copy(srcBegin, srcEnd, reinterpret_cast<char*>(mMemory));
		}
	}
	
	~DummyResource() override
	{
		XZ_LOG(Info, "%p destroyed", this);
	}

	void* GetMemory() const
	{
		return mMemory;
	}

private:
	RESOURCE_DESC mDesc;
	void* mMemory = nullptr;
};

class DummyDevice : public IDevice
{
public:
	DummyDevice()
	{
		XZ_LOG(Info, "%p created", this);
	}
	
	~DummyDevice() override
	{
		XZ_LOG(Info, "%p destroyed", this);
	}

	Xuzumi::SharedPtr<IResource> CreateResource(
		const RESOURCE_DESC& desc
	) override
	{
		std::ptrdiff_t iRes = FindAvailableResource();
		if (-1 == iRes)
		{
			return nullptr;
		}

		auto memory = new char[desc.ByteWidth]{};
		auto res = new (GetResourceMemory(iRes)) DummyResource(desc, memory);
	
		return Xuzumi::SharedPtr<IResource>(
			res,
			[iRes, memory, this](DummyResource* res)
			{
				res->~DummyResource();
				delete[] memory;

				mResourceIsAllocated[iRes] = false;
			}
		);
	}

	void* Map(Xuzumi::SharedPtr<IResource> resource) override
	{
		if (auto dummy = resource.As<DummyResource>())
		{
			return dummy->GetMemory();
		}
		return nullptr;
	}

	void Unmap(Xuzumi::SharedPtr<IResource> resource) override
	{
		// Does nothing...
	}

private:
	std::ptrdiff_t FindAvailableResource()
	{
		for (std::ptrdiff_t iRes = 0; iRes < kResourceCap; iRes++)
		{
			if (!mResourceIsAllocated[iRes])
			{
				mResourceIsAllocated[iRes] = true;
				return iRes;	
			}
		}

		return -1;
	}
	
	void* GetResourceMemory(std::ptrdiff_t iRes)
	{
		std::ptrdiff_t resourceOffset = iRes * sizeof(DummyResource);
		return mResourcePool + resourceOffset;
	}

	inline static constexpr std::size_t kResourceCap = 32; 
	char mResourcePool[sizeof(DummyResource) * kResourceCap]{};
	bool mResourceIsAllocated[kResourceCap]{};
};

Xuzumi::SharedPtr<IDevice> CreateDevice()
{
	return Xuzumi::SharedPtr<IDevice>(new DummyDevice());
}
