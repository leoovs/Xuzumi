add_executable(
	Xuzumi_Tests
	"Src/main.cpp"
	"Src/StringUtilsTests.cpp"
	"Src/IsCompatibleTests.cpp"
	"Src/DelegateTests.cpp"
	"Src/HandleGeneratorTests.cpp"
	"Src/SharedPtrTests.cpp"
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
