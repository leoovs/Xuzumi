#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <Xuzumi/Xuzumi.hpp>

class Entity
{
public:
	Entity()
	{
		XZ_LOG(Info, "Created");
	}

	Entity(Xuzumi::IDType id)
		: mID(id)
	{
		XZ_LOG(Info, "Created with ID: %zu", id);
	}

	~Entity()
	{
		XZ_LOG(Info, "Destroyed with ID: %zu", mID);
	}

	void Display() const
	{
		XZ_LOG(Info, "Entity{ ID = %zu }", mID);
	}

private:
	Xuzumi::IDType mID = Xuzumi::kInvalidID;
};

#endif // ENTITY_HPP_
