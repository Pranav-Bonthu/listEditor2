add_library(buffer_lib buffer_functions.c buffer_editing.c buffer.h)
add_executable(list_editor main.c)
target_link_libraries(list_editor PRIVATE buffer_lib)

target_link_libraries(buffer_lib PRIVATE ${CURSES_LIBRARIES} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
