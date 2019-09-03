# Advanced Programming
**Homework 2**


### Homework description:

Write a program to multiply two matrices, of any size, in C and print the resulting matrix.

An explanation of the algorithm to multiply matrices can be found here: Matrix_Multiplication (Links to an external site.). There are many other resources on line where you can get more information. For example at Math is fun (Links to an external site.). You can also test your results using this website: https://matrix.reshish.com/multiplication.php (Links to an external site.)

The program should receive the names of the files with the matrices as arguments. The input of the matrices should be done by reading plain text files with the format provided in the files included. The output should be written into  file with the same format. You must validate that the input matrices can be multiplied. You can use these files to test: Matrix_A.txtPreview the document Matrix_B.txt Preview the documentMatrix_C.txtPreview the document Matrix_D.txtPreview the document

Your program should use getopt (Links to an external site.) to parse the options and read the names of the input and output files. The command to run the program should look like this:

`
./multiplication -1 {filename_matrix_1} -2 {filename_matrix_2} -r {filename_matrix_result}`

So the valid options are -1, -2 and -r. All requiring an additional string afterwards. Your program will try to compute the multiplication of matrix 1 by matrix 2.

The basis of this program will later be used for other assignments, so make sure it is easily modified.

You should write a clean and correct program, with comments explaining your code. You must upload a .zip file containing only the source code of the program (no binaries nor project files). The name of the file must be composed of the assignment number, a dash and your name, using underscores instead of spaces. Remember not to use any non-English characters (accents, ñ, etc.). Example: 02-Gilberto_Echeverria.zip