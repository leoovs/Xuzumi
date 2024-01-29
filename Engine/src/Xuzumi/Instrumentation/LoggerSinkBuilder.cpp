#include "Xuzumi/Instrumentation/LoggerSinkBuilder.hpp"

namespace Xuzumi
{
	LoggerSink LoggerSinkBuilder::Finish()
	{
		return LoggerSink(std::move(mSinkEmitter), std::move(mSinkFormatter));
	}
}

