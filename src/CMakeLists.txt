find_package(Curses REQUIRED)
cmake_minimum_required(VERSION 3.10)
project(ListEditor LANGUAGES C CXX)

add_library(buffer_lib buffer_functions.c buffer_editing.c buffer_editing.h buffer_functions.h)
add_executable(list_editor main.c)
target_link_libraries(list_editor PRIVATE buffer_lib)

target_link_libraries(buffer_lib PRIVATE ${CURSES_LIBRARIES} PRIVATE ${CMAKE_THREAD_LIBS_INIT})