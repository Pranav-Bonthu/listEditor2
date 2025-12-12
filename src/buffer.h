#ifndef BUFFER_H //creates a library for functions
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>


// setting up the buffer
//the buffer will be edited by the users, once editing is complete,
// the buffer will be written in list.txt
//This will help to avoid errors while multiple users are editing the file
typedef struct {
    char **lines; // a pointer to an array of line pointers 
    int count; // the amount of lines in the buffer
    int capacity;// the amount of lines pointers allocated
    pthread_mutex_t lock; //only one thread can edit the buffer 
} BufferLines;

// Function declarations 
void initial_buffer(BufferLines *buffer);
void free_buffer(BufferLines *buffer);
void edit_buffer_memory(BufferLines *buffer, const char *new_line);
void load_file(BufferLines *buffer, const char *filename);
void print_buffer(BufferLines *buffer);
void edit_buffer(BufferLines *buffer);
void insert_char(char *line, char c, int position);
void delete_char(char *line, int position);
void delete_line(BufferLines *buffer, int line_number);
void insert_line(BufferLines *buffer, int line_index, const char *new_line);
int lock_file(const char *filename);
void unlock_file(int fd);



#endif
