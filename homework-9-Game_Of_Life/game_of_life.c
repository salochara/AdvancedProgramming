// Created by Salomón Charabati
// Game of Life - Program in C
// Assignment #9
// 27-11-2019

#include <stdio.h>
#include <stdlib.h>
#include "pgm_image.h"
#define MAX_STRING_SIZE 50

void usage(char *);
void iterationsOfGameOfLife(pgm_t *pgm_image, int iterations, char *output_file_name, int number_of_threads);
void changePGMImage(pgm_t * pgm_image, pgm_t * new_pgm_image);
int countNeighbors(pgm_t *, int, int);
void strip_ext(char *);

int main(int argc, char * argv[])
{
    char * input_file_name;
    char output_file_name[MAX_STRING_SIZE];
    int iterations = 0;
    pgm_t pgm_image;

    // Checks for the correct arguments for the program to run
    if(argc != 3)
        usage(argv[0]);

    // Stores the arguments passed into corresponding variables
    iterations = atoi(argv[1]);
    input_file_name = argv[2];

    // Read the contents of a text file and store them in an image structure
    readPGMFile(input_file_name,&pgm_image);

    strncpy(output_file_name,input_file_name, MAX_STRING_SIZE);
    strip_ext(output_file_name);
    // Call the iterations of Game Of Life to be applied and write the data in the image structure into a new PGM file
    iterationsOfGameOfLife(&pgm_image, iterations, output_file_name, 0);

    return 0;
}

void iterationsOfGameOfLife(pgm_t *pgm_image, int iterations, char *output_file_name, int number_of_threads)
{
    char file[MAX_STRING_SIZE];

    // Initialize a new_pgm image struct
    pgm_t new_pgm_image;
    new_pgm_image.image.height = pgm_image->image.height;
    new_pgm_image.image.width = pgm_image->image.width;
    new_pgm_image.max_value = pgm_image->max_value;
    sprintf(new_pgm_image.magic_number,"%s",pgm_image->magic_number);

    // Allocate memory for the image in the new_image struct
    allocateImage(&(new_pgm_image.image));

    // For loop for the number of iterations.
    // The pointer to the original and new image will change every
    // other iteration
    for (int i = 0; i < iterations ; ++i)
    {
        // Write the file name with 'NO' at the end. i.e. Normal, with no threads or OMP
        // Also include the number of iteration
        sprintf(file,"%s_NO%d.pgm",output_file_name,i+1);

        // Changing of pointers according to the iteration
        if(i % 2 == 0)
        {
            changePGMImage(pgm_image,&new_pgm_image);
            writePGMFile(file,&new_pgm_image);
        }else{
            changePGMImage(&new_pgm_image,pgm_image);
            writePGMFile(file,pgm_image);
        }
    }

    // Free the allocated memory
    freeImage(&new_pgm_image.image);
    printf("Freeing memory\n");

    printf("%d iterations done\n",iterations);

}
// Function for changing the PGM image according to the Game of Life rules
// The struct passed to the function depend on the iteration where the above function is at.
void changePGMImage(pgm_t * pgm_image, pgm_t * new_pgm_image)
{
    int neighbors = 0;

    /*
                                            Game of Life Rules
            Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            Any live cell with two or three live neighbours lives on to the next generation.
            Any live cell with more than three live neighbours dies, as if by overpopulation.
            Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    */

    for (int i = 0; i < pgm_image->image.height; ++i)
    {
        for (int j = 0; j < pgm_image->image.width; ++j)
        {
            neighbors = countNeighbors(pgm_image, i, j);
            // Any living cell
            if(pgm_image->image.pixels[i][j].value == 1)
            {
                if(neighbors < 2)
                {
                    new_pgm_image->image.pixels[i][j].value = 0;
                }
                else if(neighbors == 2 || neighbors == 3)
                {
                    new_pgm_image->image.pixels[i][j].value = 1;
                }
                else if(neighbors > 3)
                {
                    new_pgm_image->image.pixels[i][j].value = 0;
                }
            }
            // Any dead cell
            else
            {
                if(neighbors == 3)
                {
                    new_pgm_image->image.pixels[i][j].value = 1;
                }
                else
                {
                    new_pgm_image->image.pixels[i][j].value = 0;
                }
            }
        }
    }
}

// Function for counting every pixel-neighbor
int countNeighbors(pgm_t *pgm_image, int i, int j)
{
            int above;
        int left; int right;
            int below;
    int result;

    result = 0;
    // Get the neighbors from the every pixel.
    // Using modulo to cover the edge pixels from any image
    above = (i - 1 + pgm_image->image.height) % pgm_image->image.height;
    below = (i+1) % pgm_image->image.height;
    left = (j-1 + pgm_image->image.width) % pgm_image->image.width;
    right = (j+1) % pgm_image->image.width;

    if(pgm_image->image.pixels[above][left].value == 1)
        result++;
    if (pgm_image->image.pixels[above][j].value == 1)
        result++;
    if(pgm_image->image.pixels[above][right].value == 1)
        result++;
    if(pgm_image->image.pixels[i][left].value == 1)
        result++;
    if(pgm_image->image.pixels[i][right].value == 1)
        result++;
    if(pgm_image->image.pixels[below][left].value == 1)
        result++;
    if(pgm_image->image.pixels[below][j].value == 1)
        result++;
    if(pgm_image->image.pixels[below][right].value == 1)
        result++;

    return result;
}

/*
	Remove extensions from filename
	From: https://stackoverflow.com/questions/43163677/how-do-i-strip-a-file-extension-from-a-string-in-c/43163740
*/
void strip_ext(char *file_name)
{
    char *end = file_name + strlen(file_name);

    while (end > file_name && *end != '.' && *end != '\\' && *end != '/') {
        --end;
    }
    if ((end > file_name && *end == '.') &&
        (*(end - 1) != '\\' && *(end - 1) != '/')) {
        *end = '\0';
    }
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {number_iterations} {filename_image}\n", program);
    exit(EXIT_FAILURE);
}




