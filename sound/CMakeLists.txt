file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(sound STATIC ${SRC_FILES})

target_link_libraries(sound PRIVATE fmod)
target_compile_options(sound PRIVATE -Wall -Werror -g)
target_include_directories(sound PUBLIC /usr/include/SDL2 /usr/local/include/fmod)
