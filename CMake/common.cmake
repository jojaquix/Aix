
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

function(setupBuildFlags)

	#for complex projects common build flags here  
	add_library(cxx_settings INTERFACE)
	target_compile_features(cxx_settings INTERFACE cxx_std_17)
	target_include_directories(cxx_settings INTERFACE 
		${CMAKE_SOURCE_DIR}
		)


	add_library(c_settings INTERFACE)		
	target_include_directories(c_settings INTERFACE 
		${CMAKE_SOURCE_DIR}
	)

	if(MSVC)
		set(msv_common_defs
			_WIN32_WINNT=0x0A00
			ASIO_STANDALONE
		)
		
		target_compile_definitions(cxx_settings ${msv_common_defs})
		target_compile_definitions(c_settings ${msv_common_defs})
	endif()



endfunction()

setupBuildFlags()