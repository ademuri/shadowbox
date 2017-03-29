find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${shadowbox_SOURCE_DIR}/src)
add_executable(FpsTest fps_test.cpp)
target_link_libraries(FpsTest GTest::GTest GTest::Main Effects ${OpenCV_LIBS} ${SDL2_LIBRARY} ${OPENGLES3_LIRARIES} ${EGL_LIBRARIES})

# Assets
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
