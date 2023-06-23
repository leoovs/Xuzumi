#include <iostream>

#include <Xuzumi/Xuzumi.hpp>

int main()
{
#if XZ_CURRENT_COMPILER(MSVC)
	std::cout << "This is MSVC\n";
#elif XZ_CURRENT_COMPILER(GCC)
	std::cout << "This is GCC\n";
#elif XZ_CURRENT_COMPILER(CLANG)
	std::cout << "This is CLANG\n";
#endif
}
