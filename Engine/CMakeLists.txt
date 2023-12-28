add_library(
	Xuzumi_Engine
	STATIC
	"src/Xuzumi/Core/StringUtils.cpp"

	"src/Xuzumi/TypeMeta/TypeID.cpp"
	"src/Xuzumi/TypeMeta/TypeInfo.cpp"

	"src/Xuzumi/Messaging/EventBus.cpp"
	"src/Xuzumi/Messaging/EventPublisher.cpp"
	"src/Xuzumi/Messaging/EventSubscriber.cpp"

	"src/Xuzumi/IDFramework/HandleGenerator.cpp"

	"src/Xuzumi/Memory/Referencer.cpp"
	
 	"src/Xuzumi/Debug/DebugSession.cpp"
	"src/Xuzumi/Debug/Logging.cpp"
	"src/Xuzumi/Debug/FileLogWriter.cpp"
	"src/Xuzumi/Debug/ConsoleLogWriter.cpp"
	"src/Xuzumi/Debug/TimedLogFormatter.cpp"
	"src/Xuzumi/Debug/Assertion.cpp"
	
	"src/Xuzumi/Platform/PlatformService.cpp"
	"src/Xuzumi/Platform.Win32/PlatformServiceWin32.cpp"
 )

target_include_directories(
	Xuzumi_Engine
	PUBLIC
	"include"
	PRIVATE
	"src"
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
	"include/Xuzumi/Precompiled.hpp"
)
