file(GLOB Effects_SRC "*.hpp" "*.cpp")
list(REMOVE_ITEM Effects_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DisplayImage.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(rendering)
add_subdirectory(fadecandy)

add_library(Effects ${Effects_SRC})
add_executable(DisplayImage DisplayImage.cpp)
target_link_libraries(DisplayImage Effects Rendering fadecandy ${SDL2_LIBRARY} ${OpenCV_LIBS} ${OPENGLES3_LIRARIES} ${EGL_LIBRARIES} ${LIBFTDI_LIBRARIES})
install(TARGETS DisplayImage RUNTIME DESTINATION ${BIN_DIR})
