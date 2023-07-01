#include <Xuzumi/Precompiled.hpp>
#include <Xuzumi/Ownership/SharedFactory.hpp>

#include <cassert>

#include <iostream>

class Factory
{
public:
	std::shared_ptr<void> Create()
	{
		return std::shared_ptr<void>
		{
			CreateResource(),
			m_Share.MakeDeleter(&Factory::DestroyResource)
		};
	}

private:
	void* CreateResource()
	{
		std::cout << "Created\n";
		return new int{ 105 };
	}

	void DestroyResource(void* ptr)
	{
		delete reinterpret_cast<int*>(ptr);
		std::cout << "Destoryed\n";
	}

	Xuzumi::SharedFactory<Factory> m_Share{ this };
};

int main()
{
	auto f = std::make_shared<Factory>();

	std::shared_ptr<void> res{ f->Create() };
}
