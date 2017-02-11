add_executable(DisplayImage DisplayImage.cpp)
target_link_libraries(DisplayImage ${SDL2_LIBRARY} ${OpenCV_LIBS})
install(TARGETS DisplayImage RUNTIME DESTINATION ${BIN_DIR})
