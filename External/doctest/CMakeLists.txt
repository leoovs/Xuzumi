add_library(
	doctest
	"Src/doctest/Dummy.cpp"
)

target_include_directories(
	doctest
	PUBLIC
	"Inc"
)
