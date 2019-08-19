//
// Created by Salomón Charabati on 2019-08-12.
//


#ifndef PERSON_T
#define PERSON_T

#include <string.h>
#include <stdio.h>
#define NAME_LENGTH 60
// structs are like classes, but they do not handle functions. Only attributes.
typedef struct person{
    char name [NAME_LENGTH];
    int age;
    float height;
    float weight;
} person_t; // type defined with typedef

// Typedef is used to rename something, create an alias
typedef unsigned char byte;

void printPerson_t(person_t anyone);
//void printPerson(struct person anyone);

#endif

