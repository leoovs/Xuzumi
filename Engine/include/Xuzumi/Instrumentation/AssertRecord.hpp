#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Core/FuncName.hpp"

namespace Xuzumi
{
	struct AssertRecord
	{
		std::string UnmetConditionExpression;
		std::string FileName;
		std::int32_t LineNumber = 0;
		std::string FuncName;
		std::string Message;
	};
}

