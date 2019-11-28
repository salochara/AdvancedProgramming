// Created by Salomón Charabati on 27/11/19.
// Game of Life (Threads version) - Program in C

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "pgm_image.h"
#define MAX_STRING_SIZE 50


void usage(char *);
void iterationsOfGameOfLife(pgm_t *, int , char *, int );
void * changePGMImage(void *arg);
int countNeighbors(pgm_t *, int, int);
void strip_ext(char *);
void createThreads(pgm_t * pgm_image, pgm_t * new_pgm_image, int number_of_threads);

int main(int argc, char * argv[])
{
    char * input_file_name;
    char output_file_name[MAX_STRING_SIZE];
    int iterations = 0;
    int number_of_threads;
    pgm_t pgm_image;

    // Checks for the correct arguments for the program to run
    if(argc != 4)
        usage(argv[0]);

    // Stores the arguments passed into corresponding variables
    iterations = atoi(argv[1]);
    input_file_name = argv[2];
    number_of_threads = atoi(argv[3]);

    // Read the contents of a text file and store them in an image structure
    readPGMFile(input_file_name,&pgm_image);

    strncpy(output_file_name,input_file_name, MAX_STRING_SIZE);
    strip_ext(output_file_name);
    // Call the iterations of Game Of Life to be applied and write the data in the image structure into a new PGM file
    iterationsOfGameOfLife(&pgm_image, iterations, output_file_name, number_of_threads);

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
    for (int i = 0; i < number_of_threads ; ++i)
    {
        // Write the file name with 'THR' at the end. i.e. Threads.
        // Also include the number of iteration
        sprintf(file,"%s_THR%d.pgm",output_file_name,i+1);

        // Changing of pointers according to the iteration
        if(i % 2 == 0)
        {
            createThreads(pgm_image,&new_pgm_image,number_of_threads);
            writePGMFile(file,&new_pgm_image);
        }else{
            createThreads(&new_pgm_image,pgm_image,number_of_threads);
            writePGMFile(file,pgm_image);
        }
    }

    // Free the allocated memory
    freeImage(&new_pgm_image.image);
    printf("Freeing memory\n");

    printf("%d iterations done\n",iterations);

}

// Function for creating threads
void createThreads(pgm_t * pgm_image, pgm_t * new_pgm_image, int number_of_threads)
{
    int range;
    int remainder = false;
    int status;

    pthread_t tids[number_of_threads];
    start_stop_t thread_limits[number_of_threads];

    // Set the workload for each thread
    range = pgm_image->image.height / number_of_threads;

    for (int i = 0; i < number_of_threads ; ++i)
    {
        // Initialize the data that will be passed to each thread
        thread_limits[i].pgm_image = pgm_image;
        thread_limits[i].pgm_new_image = new_pgm_image;
        thread_limits[i].start = i * range;
        thread_limits[i].stop = i * range + range - 1;

        // If there is a remainder, add workload to the start of each thread (starting from the second thread)
        if(remainder)
            thread_limits[i].start += 1;

        // If there is a remainder in the range, add one to stop
        if(range < pgm_image->image.height)
        {
            thread_limits[i].stop += 1;
            remainder = true;
        }

        // Now, create each thread
        status = pthread_create(&tids[i],NULL,changePGMImage,&thread_limits[i]);
        // Check for errors when creating threads
        if(status < 0 )
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        // Get the results computed by each thread
        for (int j = 0; j < number_of_threads; ++j)
        {
            pthread_join(tids[j],NULL);
        }
    }
}


// Function for changing the PGM image according to the Game of Life rules
// The struct passed to the function depend on the iteration where the above function is at.
void * changePGMImage(void *arg)
{
    // Cast the argument received
    start_stop_t * thread_data = (start_stop_t *)arg;

    int neighbors;
    int above;
    int left; int right;
    int below;


    /*
                                            Game of Life Rules
            Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            Any live cell with two or three live neighbours lives on to the next generation.
            Any live cell with more than three live neighbours dies, as if by overpopulation.
            Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    */

    // Using pragma omp to parallelize the for loop
    for (int i = 0; i < thread_data->pgm_image->image.height; ++i)
    {
        for (int j = 0; j < thread_data->pgm_image->image.width; ++j)
        {
            neighbors = 0;

            // Get the neighbors from the every pixel.
            // Using modulo to cover the edge pixels from any image
            above = (i - 1 + thread_data->pgm_image->image.height) % thread_data->pgm_image->image.height;
            below = (i+1) % thread_data->pgm_image->image.height;
            left = (j-1 + thread_data->pgm_image->image.width) % thread_data->pgm_image->image.width;
            right = (j+1) % thread_data->pgm_image->image.width;

            if(thread_data->pgm_image->image.pixels[above][left].value == 1)
                neighbors++;
            if (thread_data->pgm_image->image.pixels[above][j].value == 1)
                neighbors++;
            if(thread_data->pgm_image->image.pixels[above][right].value == 1)
                neighbors++;
            if(thread_data->pgm_image->image.pixels[i][left].value == 1)
                neighbors++;
            if(thread_data->pgm_image->image.pixels[i][right].value == 1)
                neighbors++;
            if(thread_data->pgm_image->image.pixels[below][left].value == 1)
                neighbors++;
            if(thread_data->pgm_image->image.pixels[below][j].value == 1)
                neighbors++;
            if(thread_data->pgm_image->image.pixels[below][right].value == 1)
                neighbors++;

            // Any living cell
            if(thread_data->pgm_image->image.pixels[i][j].value == 1)
            {
                if(neighbors < 2)
                {
                    thread_data->pgm_new_image->image.pixels[i][j].value = 0;
                }
                else if(neighbors == 2 || neighbors == 3)
                {
                    thread_data->pgm_new_image->image.pixels[i][j].value = 1;
                }
                else if(neighbors > 3)
                {
                    thread_data->pgm_new_image->image.pixels[i][j].value = 0;
                }
            }
            // Any dead cell
            else
            {
                if(neighbors == 3)
                {
                    thread_data->pgm_new_image->image.pixels[i][j].value = 1;
                }
                else
                {
                    thread_data->pgm_new_image->image.pixels[i][j].value = 0;
                }
            }
        }
    }
    pthread_exit(NULL);
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
    printf("\t%s {number_iterations} {filename_image} {number_of_threads}\n", program);
    exit(EXIT_FAILURE);
}




