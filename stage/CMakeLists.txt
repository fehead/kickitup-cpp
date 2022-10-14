file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(stage STATIC ${SRC_FILES})

# target_link_libraries(stage PRIVATE fmodex)

# target_include_directories(shape PUBLIC ${CMAKE_SOURCE_DIR}/includes)
# target_compile_options(shape PRIVATE -Wall -Werror -g)
# target_link_libraries(shape PRIVATE pthread)
