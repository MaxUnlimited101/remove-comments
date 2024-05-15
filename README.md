
# remove-comments

### How to install:
Basically pull the remove-comments.c file, and complie with your favourite compiler.
**Note:** You have to compile the file with `-std=c++17` in GCC (or `/std:c++17` on MSVC), since I used `<filesystem>`.
Now you have an executable, so you can add it to PATH.

### Usage:
`./remove-comments <line comments file> <block comments file> <file1 | regex> [file2 | regex] [...]`
`Help page: ./remove-comments --help`

### How to create 'comment files': 
'Comment files' contain information about different languages in a specific format.
Example 'line comments file'
`<FILE_EXTENSION_1>:<LINE_COMMENT_DEFINITION_1>`
`<FILE_EXTENSION_2>:<LINE_COMMENT_DEFINITION_2>`
Example 'block comments file':
`<FILE_EXT_1>:<BLOCK_BEGINNING_DEF_1>QQQ<BLOCK_COMMENT_ENDING_DEF_1>`
`<FILE_EXT_2>:<BLOCK_BEGINNING_DEF_2>QQQ<BLOCK_COMMENT_ENDING_DEF_2>`
##### Example for C++: 
Line file:
`cpp://`
Block file: 
`cpp:/*QQQ*/`
Note: `QQQ` string MUST be included, this is necessary for program to work properly!

### Supported languages:
Support for this languages is built in the binary, hence you don't have to specify them in 'comment files'
1. C / C++ / JavaScript / Java /C#
2. Python
3. PHP
4. Go
5. Rust
6. SQL

### General notes:
1. The program does NOT recognise line-break symbol '\\' (sometimes present in C/C++) 
2. In case of Python, the program also removes string literals `'''comment'''` and `"""comment"""`
