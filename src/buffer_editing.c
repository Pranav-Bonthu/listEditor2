#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>
#include "buffer.h"
#include <sys/file.h> 


const char *filename = "list.txt";


void load_file(BufferLines *buffer, const char *filename){
    FILE *fptr;
    char line[1024];

    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening file for writing");
        return ; // Indicate an error
    }

    while (fgets(line,sizeof(line),fptr) != NULL){
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n'){ // \n represents the end of a line. Single quotes for single character 
            line[len - 1] = '\0'; // \0 is the null character, ends the string 
        }
       edit_buffer_memory(buffer, line); 
    }
    fclose(fptr);
    return;
}

void print_buffer(BufferLines *buffer){
    clear(); // clears the screen before printing using ncurses
    
    for (int i = 0; i < buffer->count; i++){
         mvprintw(i, 0, "%s", buffer->lines[i]); 
         // printf does nto work with ncurses, so mvprintw is used 
    }
}

void insert_char(char *line, char c, int position){
    size_t len = strlen(line);
    for (int i = (int)len; i >= position; i--){ //starts at the position of the added index
        line[i + 1] = line[i]; // shift characters to the right
    }
    line[position] = c; // insert the new character
}


void delete_line(BufferLines *buffer, int line_number){
    if (line_number < 0 || line_number >= buffer->count){
        return; // out of bounds
    }
    free(buffer->lines[line_number]); // free memory of the line to be deleted

    for (int i = line_number; i < buffer->count - 1; i++){
        buffer->lines[i] = buffer->lines[i + 1]; // shift lines up
    }
    buffer->count--; // decrease the count of lines in the buffer
}

void insert_line(BufferLines *buffer, int line_index, const char *new_line){

    if (line_index < 0 || line_index > buffer->count) {
        pthread_mutex_unlock(&buffer->lock);
        return; // out of bounds
    }
    
    // Check if we need to expand capacity
    if (buffer->count >= buffer->capacity) {
        buffer->capacity *= 2;
        char **checker = realloc(buffer->lines, sizeof(char*) * buffer->capacity);
        
        if (checker == NULL) {
            pthread_mutex_unlock(&buffer->lock);
            return;
        }
        buffer->lines = checker;
    }
    
    // Shift all lines from line_index down by one
    for (int i = buffer->count; i > line_index; i--) {
        buffer->lines[i] = buffer->lines[i - 1];
    }
    
    // Allocate memory for the new line at line_index
    buffer->lines[line_index] = malloc(1024);
  
  
    // Copy the new line content
    strcpy(buffer->lines[line_index], new_line);
    
    // Increase the count
    buffer->count = buffer->count + 1;    
}

void delete_char(char *line, int position){
    size_t len = strlen(line);
    if (position < 0 || position >= len){ 
        return; //out of bounds
    } 
    for (size_t i = position; i < len; i++){ //starts at the position of the deleted index
        line[i] = line[i + 1]; // shift characters to the left once a character is delected
    }
}


void edit_buffer(BufferLines *buffer){

    int cursor_row = 0;
    int cursor_col = 0;

    pthread_mutex_lock(&buffer->lock);
    while (1){

        print_buffer(buffer);
        move(cursor_row, cursor_col); // moves the cursor to the specified position

        refresh(); // refreshes the screen to update changes
        int c = getch(); // waits for key press, reutrns int of the pressed key
        //getch();

        if (c == 27){ // ESC key to exit editing mode
            clear();
            mvprintw(1,1,"%s","Do you want to save changes? (y/n)");
            refresh();
            char a = getch();
            if (a == 'y'){
                FILE *fptr = fopen(filename, "w");
                for (int i = 0; i < buffer->count; i++){
                    fprintf(fptr, "%s\n", buffer->lines[i]);
                }   
                fclose(fptr); 
                break; 
                }
            else {
                break;
            }
        }

        switch (c){
        case 259: // left arrow key
            cursor_row = cursor_row - 1;
            if (cursor_row < 0){cursor_row = 0;}
            break;

        case 258: // right arrow key
            int line_len = strlen(buffer->lines[cursor_row]); 
            cursor_row = cursor_row + 1;
            if (cursor_row > line_len ){cursor_row = line_len;}            
            break;      
        case 260: // up arrow key
            cursor_col = cursor_col - 1;
            if (cursor_row == 0){cursor_row = 0;}
            break;
        case 261: // down arrow key
            cursor_col = cursor_col + 1;
            if (cursor_row == buffer->count - 1){cursor_row = buffer->count - 1;}
            break;
        case KEY_BACKSPACE: // backspace key
            if (cursor_col > 0 && cursor_row < buffer->count){
                delete_char(buffer->lines[cursor_row], cursor_col - 1);
                cursor_col = cursor_col - 1;
                  if (strlen(buffer->lines[cursor_row]) == 0){
                    delete_line(buffer,cursor_row ); // delete the line if it becomes empty
                     }
                }
            break;
        case '\n': // enter key
            // Move to the next line
            cursor_row = cursor_row + 1;
            cursor_col = 0;
            if (cursor_row >= buffer->count){
                insert_line(buffer, cursor_row, ""); // insert a new empty line at the end
            }

            break;
        default:
            if (c >= 32 && c <= 126){ // printable characters
                insert_char(buffer->lines[cursor_row], (char)c, cursor_col);
                cursor_col = cursor_col + 1;
            }
            break;
        }
    }
        pthread_mutex_unlock(&buffer->lock);
}        
