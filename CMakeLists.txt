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
target_link_libraries(kickitup PUBLIC SDL fmodex)
target_compile_options(kickitup PUBLIC -Wall -Werror -g)

# target_include_directories(kickitup PUBLIC ${CMAKE_SOURCE_DIR})