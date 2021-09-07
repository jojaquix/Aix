# CMakeList.txt: archivo del proyecto de CMake de nivel superior, establezca la configuración global
# e incluya los subproyectos aquí.
#
cmake_minimum_required (VERSION 3.8)


function(generateDeps)
	include(FetchContent)
	FetchContent_Declare(asio
	  GIT_REPOSITORY git@github.com:chriskohlhoff/asio.git
	  GIT_TAG asio-1-19-2
	  CONFIGURE_COMMAND ""
	  BUILD_COMMAND ""
	  )

	FetchContent_GetProperties(asio)
	if(NOT asio_POPULATED)
	  FetchContent_Populate(asio)
	endif()

	add_library(asio INTERFACE)
	target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
endfunction()

function(Main)
	project ("Aix")
	set(CMAKE_CXX_STANDARD 17)


	generateDeps()

	# Incluya los subproyectos.
	add_subdirectory ("AixClient")
	add_subdirectory ("UTests")

endfunction()



Main()