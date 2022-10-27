file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(util STATIC ${SRC_FILES})

target_link_libraries(util PRIVATE SDL2)
target_compile_options(util PRIVATE -Wall -Werror -g)

target_include_directories(util PUBLIC /usr/include/SDL2)