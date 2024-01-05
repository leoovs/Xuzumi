function(set_utf8_as_source_charset TARGET)
	if(MSVC)
		target_compile_options(
			${TARGET}
			PRIVATE
			"/source-charset:utf-8"
		)
	elseif(UNIX OR MINGW OR MSYS)
		target_compile_options(
			${TARGET}
			PRIVATE
			"-finput-charset=UTF-8"
		)
	endif()
endfunction()

function(set_utf8_as_execution_charset TARGET)
	if(MSVC)
		target_compile_options(
			${TARGET}
			PRIVATE
			"/execution-charset:utf-8"
		)
	elseif(UNIX OR MINGW OR MSYS)
		target_compile_options(
			${TARGET}
			PRIVATE
			"-fexec-charset=UTF-8"
		)
	endif()
endfunction()
