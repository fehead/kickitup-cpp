file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(video STATIC ${SRC_FILES})

target_link_libraries(video PRIVATE SDL2)
target_compile_options(video PRIVATE -Wall -Werror -g)

target_include_directories(video PUBLIC /usr/include/SDL2 /usr/local/include/fmod)

# target_link_libraries(shape PRIVATE pthread)
