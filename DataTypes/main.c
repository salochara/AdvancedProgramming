//
// Created by Salomón Charabati on 2019-08-12.
//


#include <string.h>
#include <stdio.h>
#include "datatypes.h"

int main()
{
    // This is when the struct is not defined with a typedef
    struct person Gil;
    Gil.age = 32;
    Gil.height = 1.74;

    // For assigning a string
    strncpy(Gil.name,"Gilberto Echevarria",NAME_LENGTH);
    // Or, if you want to add other data
    //sprintf(Gil.name,"Gilberto Excheverria %d &f",Gil.age,Gil.height);

    person_t Salo;
    strncpy(Salo.name,"Salomon Charabati", NAME_LENGTH);
    Salo.age = 23;
    Salo.height = 1.70;

    printPerson_t(Salo);

    return 0;
}





