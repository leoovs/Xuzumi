add_library(
	Xuzumi_Engine
	STATIC
	"Src/Xuzumi/Core/StringUtils.cpp"

	"Src/Xuzumi/TypeMeta/TypeID.cpp"
	"Src/Xuzumi/TypeMeta/TypeInfo.cpp"

	"Src/Xuzumi/Messaging/EventBus.cpp"
	"Src/Xuzumi/Messaging/EventPublisher.cpp"
	"Src/Xuzumi/Messaging/EventSubscriber.cpp"

	"Src/Xuzumi/IDFramework/HandleGenerator.cpp"

 	"Src/Xuzumi/Debug/DebugSession.cpp"
	"Src/Xuzumi/Debug/Logging.cpp"
	"Src/Xuzumi/Debug/FileLogWriter.cpp"
	"Src/Xuzumi/Debug/ConsoleLogWriter.cpp"
	"Src/Xuzumi/Debug/TimedLogFormatter.cpp"
	"Src/Xuzumi/Debug/Assertion.cpp"
)

target_include_directories(
	Xuzumi_Engine
	PUBLIC
	"Inc"
	PRIVATE
	"Src"
)

set_target_properties(
	Xuzumi_Engine
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)

target_precompile_headers(
	Xuzumi_Engine
	PRIVATE
	"Inc/Xuzumi/Precompiled.hpp"
)
