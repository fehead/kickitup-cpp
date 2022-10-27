file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(stage STATIC ${SRC_FILES})
target_compile_options(stage PRIVATE -Wall -Werror -g)

target_include_directories(stage PUBLIC /usr/include/SDL2)