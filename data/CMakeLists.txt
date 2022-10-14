file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(data STATIC ${SRC_FILES})

target_link_libraries(data PRIVATE SDL)

# target_include_directories(shape PUBLIC ${CMAKE_SOURCE_DIR}/includes)
# target_compile_options(shape PRIVATE -Wall -Werror -g)
# target_link_libraries(shape PRIVATE pthread)
