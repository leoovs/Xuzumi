#include "Xuzumi/Core/StringUtils.hpp"

namespace Xuzumi
{
	std::string FormatString(std::string_view format, ...)
	{
		va_list args;
		va_start(args, format);
		std::string result = FormatString(format, args);
		va_end(args);

		return result;
	}

	std::string FormatString(std::string_view format, va_list args)
	{
		va_list argsForBufferSizeQuery;
		va_copy(argsForBufferSizeQuery, args);
	
		constexpr std::size_t kOptimalBufferSize = 256;
		char optimalCaseBuffer[kOptimalBufferSize]{};
		std::unique_ptr<char[]> worstCaseBuffer;
		char* buffer = optimalCaseBuffer;

		auto requiredSize = std::size_t(
			vsnprintf(
				nullptr,
				0,
				format.data(),
				argsForBufferSizeQuery
			)
		);
		va_end(argsForBufferSizeQuery);

		requiredSize += 1; // Include the null terminator.

		if (requiredSize > kOptimalBufferSize)
		{
			worstCaseBuffer = std::make_unique<char[]>(requiredSize);
			buffer = worstCaseBuffer.get();
		}

		vsnprintf(buffer, requiredSize, format.data(), args);
	
		return std::string(buffer);
	}
}
