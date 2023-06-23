add_library(
	Xuzumi_Engine
	"Src/Xuzumi/Dummy.cpp"
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
