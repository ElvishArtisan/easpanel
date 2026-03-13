cmake_minimum_required(VERSION 3.16)

project(easpanel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Network)
find_package(PkgConfig REQUIRED)

qt_standard_project_setup()
pkg_check_modules(CURL libcurl)
pkg_check_modules(RIVWEBCAPI rivwebcapi)

add_compile_definitions(VERSION="1.0.0rc0")
add_compile_definitions(ARCH="x86_64")

add_subdirectory(src/easp)
add_subdirectory(src/tests)

add_subdirectory(icons)
add_subdirectory(xdg)
