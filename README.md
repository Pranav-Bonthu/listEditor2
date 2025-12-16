# listEditor

This is the list editing app that multiple users can use.

## Getting Started

1. Clone this repository

2. In your terminal, type the following command:
```bash
   cd listEditor/src
```
3. Use the following commands to install necessary dependencies:
```bash
sudo apt-get update
sudo apt-get install libncurses5-dev libncursesw5-dev
```


4. Compile the program using:
```bash
   mkdir build
```
```bash
   cmake ..
```
```bash
   make
```

5. Run the program using:
```bash
   ./list_editor
```

6. To show the multi-user functionality, open another terminal and run the application 
## How the Code Works

### Notable external libraries used:

1. **ncurses.h**: A library that allows for text editing/cursor movement on a screen in the terminal
2. **pthread.h**: A library for creating and managing threads
3. **sys/file.h**: A library that provides system declarations to control file states

### There are 4 main files:

**1. main.c**

This is the main file that utilizes all of the functions to allow the program to run

**2. buffer.h**

A library that contains all of the functions and stores the struct named "BufferLines", which holds the data of the buffer

**3. buffer_functions.c**

A file that contains a variety of functions that help initialize and change the allocated memory of the buffer. The functions are:

- `initial_buffer`: Sets some initial values for the buffer
- `edit_buffer_memory`: Allocates more memory for the buffer when needed
- `free_buffer`: Frees the memory of the buffer after it is no longer being used
- `lock_file`: locks the list so that no one else can write into it
- `unlock_file`: unlocks the file so that others can write into it

**4. buffer_editing.c**

This file contains the functions used to edit the buffer. The functions are:

- `load_file`: loads the file so that it can be read into the buffer
- `print_buffer`: prints the buffer onto the screen (using ncurses)
- `insert_char`: inserts a character/symbol when the user hits a specific key
- `delete_line`: a function that removes an empty line from memory and then shifts all following lines
- `insert_line`: creates a new line for the user to type in
- `delete_char`: when the user presses 'backspace', the symbol before the cursor will be deleted, and all following characters will be shifted down
- `edit_buffer`: takes the input from the user in order to manipulate the buffer. It also allows for the information within the buffer to be written into the list

## Valgrind memory loss report:

<img width="452" height="141" alt="image" src="https://github.com/user-attachments/assets/62f61ef9-3de0-4338-b0e9-1388ffa06fe1" />

