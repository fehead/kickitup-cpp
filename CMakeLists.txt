cmake_minimum_required(VERSION 3.11)
project(
    kickitup
    VERSION 0.1.0
    DESCRIPTION "kick it up"
    LANGUAGES CXX)

add_subdirectory(data)
add_subdirectory(input)
add_subdirectory(sound)
add_subdirectory(stage)
add_subdirectory(util)
add_subdirectory(video)
add_executable(kickitup Main.cpp GameConfig.cpp KickItUp.cpp)
target_link_libraries(kickitup PRIVATE data input sound stage util video)


# file(GLOB SRC_FILES CONFIGURE_DEPENDS
#     ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/input/*.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/sound/*.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/stage/*.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/video/*.cpp
# )
# add_executable(kickitup ${SRC_FILES})

target_link_libraries(kickitup PUBLIC SDL2 fmod)
target_compile_options(kickitup PUBLIC -Wall -Werror -g)
target_include_directories(kickitup PUBLIC /usr/include/SDL2 /usr/local/include/fmod)
# target_include_directories(kickitup PUBLIC ${CMAKE_SOURCE_DIR} data input sound video util stage)