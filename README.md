
# remove-comments

### How to install:
Basically pull the remove-comments.c file, and complie with your favourite compiler.
**Note:** You have to compile the file with `-std=c++17` in GCC (or `/std:c++17` on MSVC), since I used `<filesystem>`.
Now you have an executable, so you can add it to PATH.

### Usage:
`Usage: ./remove-comments <line comments file> <block comments file> <file1 | regex> [file2 | regex] [...]`
`Help page: ./remove-comments --help`

### General notes:
1. The program does NOT recognise line-break symbol '\\' (sometimes present in C/C++) 
2. 