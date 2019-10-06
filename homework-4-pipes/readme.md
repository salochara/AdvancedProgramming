# **Vigenere Cypher**

Write a program that creates processes using fork and communicates between them using pipes.

The program will be able to encode or decode messages using the Vigenère cipher (Links to an external site.). The messages and the key should be all in lowercase letters. Use only the English alphabet. Characters other than letters will be left the same (but they also "consume" a letter from the key string). You can use the following link to test your program: https://cryptii.com/pipes/vigenere-cipher (Links to an external site.)

The examples provided here example_encodings.zip were encoded with the key: prograavanzada

The main process will wait for user input. The users can choose to encode or decode a message, then provide the name of the file that contains the message. Finally the user provides the encryption key.

A fork is used to create a child process that will take care of the task of encoding or decoding. When the child finishes it will inform its parent process, write a new file with the prefix "encoded_" or "decoded_" and send back the name of the new file to its parent.

You can use any of the Inter Process Communication (IPC) methods seen so far (pipe, popen, fifo, passing arguments), as long as you have two way communication between the parent and the child processes.

You should write a clean and correct program, with comments explaining your code. Add your name inside all your files in a comment. You must upload a .zip file containing only the source code of the program (no binaries nor project files). The name of the file must be composed of the assignment number, a dash and your name, using underscores instead of spaces. Remember not to use any non-English characters (accents, ñ, etc.). Example: 04-Gilberto_Echeverria.zip