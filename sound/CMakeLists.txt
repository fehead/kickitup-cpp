file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(sound STATIC ${SRC_FILES})

target_link_libraries(sound PRIVATE fmodex)
target_compile_options(sound PRIVATE -Wall -Werror -g)

# target_include_directories(shape PUBLIC ${CMAKE_SOURCE_DIR}/includes)
# target_link_libraries(shape PRIVATE pthread)
