add_library(
	Xuzumi_Engine
	STATIC
	"src/Xuzumi/Core/StringUtils.cpp"
	
	"src/Xuzumi/String/UtfEncoding.cpp"
	"src/Xuzumi/String/Utf8CharView.cpp"
	"src/Xuzumi/String/Utf16CharView.cpp"
	"src/Xuzumi/String/UtfTextReader.cpp"

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
	
	"src/Xuzumi/Input.Win32/Win32KeyMapping.cpp"
	"src/Xuzumi/Input.Win32/Win32InputDevice.cpp"
	
	"src/Xuzumi/Platform/PlatformService.cpp"
	"src/Xuzumi/Platform.Win32/Win32Debug.cpp"
	"src/Xuzumi/Platform.Win32/Win32PlatformService.cpp"
	"src/Xuzumi/Platform.Win32/Win32WindowFrame.cpp"
	"src/Xuzumi/Platform.Win32/Win32WindowClass.cpp"
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
