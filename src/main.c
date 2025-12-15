#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h> //ui cotnrol in terminal
#include <pthread.h> //mutex and threads
#include "buffer_editing.h" //my functions 
#include "buffer_functions.h" //my functions 

#include <unistd.h>    // unlink()


int main() {
    BufferLines buffer;
    int lock = lock_file("list.txt"); 
    if (lock < 0 ){
        printf("File is busy\n");
        return 1;
    }

    initial_buffer(&buffer);
    load_file(&buffer, "list.txt");

    initscr(); // start ncurses mode
    cbreak(); //makes typed chaaracters immediately available to the program
    noecho();  // don't echo input characters
    keypad(stdscr, TRUE); // enable arrow keys

    print_buffer(&buffer);
    refresh(); 

    edit_buffer(&buffer);
    endwin(); // end ncurses mode

    free_buffer(&buffer);

    unlock_file(lock); // unlock the file after editing
    unlink("list.txt.lock"); // remove the lock file


    return 0;
}