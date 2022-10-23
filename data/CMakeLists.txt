file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(data STATIC ${SRC_FILES})

target_link_libraries(data PRIVATE SDL2)

target_include_directories(data PUBLIC /usr/include/SDL2 /usr/local/include/fmod)
