include(TranslationCharset)

add_executable(
	Xuzumi_Sandbox
	"src/main.cpp"
	"src/App.cpp"
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

set_utf8_as_source_charset(Xuzumi_Sandbox)
set_utf8_as_execution_charset(Xuzumi_Sandbox)

