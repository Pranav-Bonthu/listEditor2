#ifndef BUFFER_EDITING_H //creates a library for functions
#define BUFFER_EDITING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "buffer_functions.h"

void load_file(BufferLines *buffer, const char *filename);
void print_buffer(BufferLines *buffer);
void edit_buffer(BufferLines *buffer);
void insert_char(char *line, char c, int position);
void delete_char(char *line, int position);
void delete_line(BufferLines *buffer, int line_number);
void insert_line(BufferLines *buffer, int line_index, const char *new_line);

#endif
