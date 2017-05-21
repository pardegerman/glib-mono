cmake_minimum_required(VERSION 3.5)
project(glib-mono-host)

set(CMAKE_LOCAL_ROOT ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_LOCAL_ROOT}/Modules")
enable_language(CSharp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB glib-2.0)
pkg_check_modules(GIO gio-2.0)
pkg_check_modules(MONO mono-2)

include_directories(${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS} ${MONO_INCLUDE_DIRS})
add_executable(glib-mono-host src/main.c)
target_link_libraries(glib-mono-host ${GLIB_LIBRARIES} ${GIO_LIBRARIES} ${MONO_LIBRARIES})

add_library(libmodule src/Module.cs)
set_target_properties(libmodule PROPERTIES LINKER_LANGUAGE CSharp)
