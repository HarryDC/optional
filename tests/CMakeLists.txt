
include_directories(
)

SET(LOCAL_HEADERS
	../include/optional
)

SET(LOCAL_SOURCES
	optional_tests.cpp
)


add_executable(OptionalTests ${LOCAL_HEADERS} ${LOCAL_SOURCES})
target_link_libraries(OptionalTests PRIVATE gtest gmock gmock_main)

if (MSVC)
	# Restrict MSVC feature set for better standards conformance
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
endif()

add_test(AllTestsInMain OptionalTests)
