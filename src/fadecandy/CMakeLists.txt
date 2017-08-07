file(GLOB fadecandy_SRC "*.h")

add_library(fadecandy ${fadecandy_SRC})
set_target_properties(fadecandy PROPERTIES LINKER_LANGUAGE CXX)
