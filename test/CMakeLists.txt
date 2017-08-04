find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${shadowbox_SOURCE_DIR}/src)
add_executable(FpsTest fps_test.cpp)
add_executable(EmptyDetectorTest empty_detector_test.cpp)
target_link_libraries(FpsTest GTest::GTest GTest::Main Effects Rendering ${OpenCV_LIBS} ${SDL2_LIBRARY} ${OPENGLES3_LIRARIES} ${EGL_LIBRARIES})
target_link_libraries(EmptyDetectorTest GTest::GTest GTest::Main Effects ${GMOCK_LIBRARIES} ${OpenCV_LIBS} ${LIBFTDI_LIBRARIES})

# Assets
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
