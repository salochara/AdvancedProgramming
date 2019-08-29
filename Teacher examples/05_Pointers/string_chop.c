#include <stdio.h>

int main()
{
    char string[15] = "Hello world";
    char * ptr = string;

    printf("Array: %s\n", string);
    printf("Pointer: %s\n", ptr);
    ptr = &string[2];
    printf("Pointer: %c\n", *ptr);
    printf("Pointer: %c\n", *(ptr+4));
    printf("Pointer: %c\n", *(ptr+4)-3);
    ptr += 4;
    return 0;
}
