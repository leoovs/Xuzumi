add_executable(
	Xuzumi_Sandbox
	"Src/main.cpp"
)

set_target_properties(
	Xuzumi_Sandbox
	PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)

target_link_libraries(
	Xuzumi_Sandbox
	PRIVATE
	Xuzumi_Engine
)
