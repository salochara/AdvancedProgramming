#include "datatypes.h"
/*void printPerson(struct person anyone) // when not using typedef, these is the definition of the parameters
{

}*/
void printPerson_t(person_t anyone)
{
    printf("Size of the structure %ld \n", sizeof(anyone)); // prints the sum of the data reserved for the person struct
    printf("%s \n%f \n%d \n",anyone.name,anyone.height,anyone.age);
}





