add_executable(
	Xuzumi_Tests
	"Src/main.cpp"
	"Src/TestCore/TestDelegate.cpp"
	"Src/TestCore/TestStringUtils.cpp"
	"Src/TestCore/TestTemplates/TestIsCompatible.cpp"

	"Src/TestIDFramework/TestHandleGenerator.cpp"

	"Src/TestMemory/TestSharedPtr.cpp"
	"Src/TestMemory/TestWeakPtr.cpp"
	"Src/TestMemory/TestObserverPtr.cpp"
	"Src/TestMemory/TestControlBlock.cpp"
	"Src/TestMemory/TestReferencer.cpp"
	"Src/TestMemory/TestResourceDeleterGuard.cpp"

	"Src/TestMessaging/TestEvent.cpp"
	"Src/TestMessaging/TestEventBus.cpp"
	"Src/TestMessaging/TestEventPublisher.cpp"
	"Src/TestMessaging/TestEventSubscriber.cpp"

	"Src/TestTypeMeta/TestTypeID.cpp"
)

set_target_properties(
	Xuzumi_Tests
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(
	Xuzumi_Tests
	PRIVATE
	doctest
	Xuzumi_Engine
)

target_include_directories(
	Xuzumi_Tests
	PRIVATE
	"$<TARGET_PROPERTY:Xuzumi_Engine,SOURCE_DIR>/Src"
)
