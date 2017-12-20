# Steganography
A bitmap image steganography encoding tool.
Reads in a bitmap image and a string to be concealed, and then encodes the string into an outputted bitmap image.

## Requirements
Platform: Linux (Ubuntu)  
Written in C (standard ANSI C11)  
Compiled using GCC.  
No external libraries used.  

## Build
To build this project:
  1. Clone the project.
  2. Navigate to the project directory.
  3. Run the following from the command line: 
  ```
  $ gcc -std=c11 main.c
  ```

## Usage
After building the project, run the following from the command line:
```
$ ./a.out {input_file} {output_file} {secret_message}
```
Command line arguments:  
`{input_file}` is a path to a bitmap image with a 32 bit pixel size.   
`{output_file}` is a path to an output file, which may or may not exist. If it exists, it will be overwritten.  
`{secret_message}` is a text string* containing a secret message.  

*If there are spaces in the message, surround the whole message with double quotes.  
