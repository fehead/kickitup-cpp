file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(input STATIC ${SRC_FILES})

target_link_libraries(input PRIVATE SDL2)
target_compile_options(input PRIVATE -Wall -Werror -g)

target_include_directories(input PUBLIC /usr/include/SDL2)
