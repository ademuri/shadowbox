cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0028 NEW)
project(shadowbox)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${shadowbox_SOURCE_DIR}/cmake")

# Set an output directory for our binaries
set(BIN_DIR ${shadowbox_SOURCE_DIR}/bin)

# Set appropriate warning levels for clang, gcc & msvc.
# Also set debug/optimization flags depending on the build type.
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Find dependencies
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc imgcodecs)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

find_package(OpenGLES3 REQUIRED)
include_directories(${OPENGLES3_INCLUDE_DIR} ${EGL_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)

# Configure tests
enable_testing()

add_test(MyTests test/FpsTest)
