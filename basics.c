//
// Created by Salomón Charabati on 2019-08-12.
//

#include <stdio.h>

int main()
{
    char name [50];
    int age;
    char gender;

    printf("What's your name?\t");
    scanf("%s",name); //name is already a pointer. No need for "&", like in the following example
    printf("Hello, %s\n",name);

    printf("How old are you? \t");
    scanf("%d",&age);
    printf("You're %d years old\n",age);

    printf("Enter your gender: \t");
    scanf("%c",&gender);
    printf("Your gender is: %c\n",gender);



    return 0;
}