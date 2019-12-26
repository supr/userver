set (SOURCES
	main.cc
)

add_executable(userver ${SOURCES})
target_include_directories(userver PRIVATE ${LIBURING_INCLUDE_DIRS})
target_link_libraries(userver ${LIBURING_LIBRARIES})
set_target_properties(userver PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
