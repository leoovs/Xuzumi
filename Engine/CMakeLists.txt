add_library(
	Xuzumi_Engine
	STATIC
	"Src/Xuzumi/TypeInfo/TypeID.cpp"

	"Src/Xuzumi/Messaging/EventBus.cpp"
	"Src/Xuzumi/Messaging/EventPublisher.cpp"
	"Src/Xuzumi/Messaging/EventSubscriber.cpp"

	"Src/Xuzumi/IDFramework/HandleGenerator.cpp"
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
