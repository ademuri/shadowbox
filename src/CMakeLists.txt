file(GLOB Effects_SRC "*.hpp" "*.cpp")
list(REMOVE_ITEM Effects_SRC "DisplayImage.cpp")

add_library(Effects ${Effects_SRC})
add_executable(DisplayImage DisplayImage.cpp)
target_link_libraries(DisplayImage Effects ${SDL2_LIBRARY} ${OpenCV_LIBS} ${OPENGLES3_LIRARIES} ${EGL_LIBRARIES})
install(TARGETS DisplayImage RUNTIME DESTINATION ${BIN_DIR})
