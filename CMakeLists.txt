cmake_minimum_required(VERSION 3.15)
include(ExternalProject)
include(FindPkgConfig)

project(userver C CXX)

execute_process (COMMAND ${CMAKE_C_COMPILER} -dumpmachine OUTPUT_VARIABLE HOST_TRIPLE_DEFAULT OUTPUT_STRIP_TRAILING_WHITESPACE)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

option (USE_COLOR "Colorize log output" ON)
option (USE_SYSTEM_LIBURING "Use system liburing" ON)

set (HOST_TRIPLE "${HOST_TRIPLE_DEFAULT}" CACHE STRING "Override default host triple")
if (HOST_TRIPLE)
	set (HOST_TRIPLE_ARG "--host=${HOST_TRIPLE}")
endif()

message (STATUS "Building for ${HOST_TRIPLE}")

find_program (AUTOCONF autoconf)
if (NOT AUTOCONF)
	message (FATAL_ERROR "Missing autoconf. Install autoconf package and try again.")
endif()

find_program (AUTOMAKE automake)
if (NOT AUTOMAKE)
	message (FATAL_ERROR "Missing automake. Install automake package and try again.")
endif()

if (USE_SYSTEM_LIBURING)
	find_library(LIBURING_LIBRARIES liburing)
	pkg_search_module(LIBURING liburing)
	if (NOT LIBURING_LIBRARIES)
		message (WARNING "liburing not found. Building from source.")
		set (USE_SYSTEM_LIBURING OFF)
	endif()
endif()

if (NOT USE_SYSTEM_LIBURING)
	set (LIBURING_PREFIX "${CMAKE_BINARY_DIR}/liburing-prefix")
	ExternalProject_Add(
		liburing_external
		GIT_REPOSITORY "https://git.kernel.dk/cgit/liburing"
		GIT_TAG liburing-0.3
		PREFIX ${LIBURING_PREFIX}

		UPDATE_COMMAND ""
		CONFIGURE_COMMAND ${LIBURING_PREFIX}/src/liburing_external/configure --prefix=${LIBURING_PREFIX}
		BUILD_IN_SOURCE ON
	)

	add_library (liburing STATIC IMPORTED)
	set_property (TARGET liburing PROPERTY IMPORTED_LOCATION "${LIBURING_PREFIX}/lib/liburing.a")
	add_dependencies (liburing liburing_external)

	set (LIBURING_INCLUDE_DIRS "${LIBURING_PREFIX}/include")
	set (LIBURING_LIBRARIES liburing)
	set (LIBURING_BUILTIN liburing)
endif()

if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	execute_process( COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
		${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
	)
endif()

add_subdirectory (src)

