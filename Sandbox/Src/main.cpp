#include <iostream>

#include <Xuzumi/TypeInfo/TypeName.hpp>

using namespace Xuzumi;

int main()
{
	std::cout << GetTypeName(std::cout) << std::endl;
}
