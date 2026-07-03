cmake_minimum_required(VERSION 3.16)
project(KickItUp VERSION 0.4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED sdl3-image)
pkg_check_modules(SDL3_TTF REQUIRED sdl3-ttf)
pkg_check_modules(MPG123 REQUIRED libmpg123)

set(SOURCES
    sdl3_kick.cpp
    main.cpp
    song.cpp
    select.cpp
    result.cpp
    dead.cpp
    double.cpp
    config.cpp
    input.cpp
    ddutil.cpp
    dsutil.cpp
    modeSlct.cpp
    player.cpp
    game.cpp
    stepdata.cpp
    timing.cpp
    renderer.cpp
    audio.cpp
    animator.cpp
    stage_title.cpp
)

add_executable(kickitup ${SOURCES})

target_include_directories(kickitup PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
    ${SDL3_TTF_INCLUDE_DIRS}
    ${MPG123_INCLUDE_DIRS}
)

target_link_libraries(kickitup PRIVATE
    ${SDL3_LIBRARIES}
    ${SDL3_IMAGE_LIBRARIES}
    ${SDL3_TTF_LIBRARIES}
    ${MPG123_LIBRARIES}
)
