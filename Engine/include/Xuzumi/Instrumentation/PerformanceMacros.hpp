#pragma once

#include "Xuzumi/Core/FuncName.hpp"
#include "Xuzumi/Instrumentation/PerformanceCapture.hpp"
#include "Xuzumi/Instrumentation/PerformanceProfiler.hpp"
#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"

#define XZ_INTERNAL_PROFILE_JOIN_NAMES(a, b) a ## b
#define XZ_INTERNAL_PROFILE_TAG(name, line) \
	XZ_INTERNAL_PROFILE_JOIN_NAMES(name, line)

#define XZ_PROFILE_BLOCK(blockName) ::Xuzumi::PerformanceCapture \
	XZ_INTERNAL_PROFILE_TAG(XuzumiImplDetail_PerformanceCapture_UniqueTag, __LINE__) \
	(::Xuzumi::InstrumentationSession::Get().GetPerformanceProfiler(), blockName)

#define XZ_PROFILE_FUNCTION() XZ_PROFILE_BLOCK(XZ_FUNCNAME)

