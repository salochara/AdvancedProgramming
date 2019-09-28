//
// Created by Salomón Charabati on 2019-09-26.
//

#include<stdio.h>
#include<string.h>
#include <ctype.h>

int main()
{
    //  iywy shjuyc ee
    char msg[] = "iywy shjuyc ee";
    char key[] = "prograavanzada";
    char encryptedMsg[50];
    char decryptedMsg[50];

    int messageLength = strlen(msg);
    int keyLength = strlen(key);

    for (int i = 0, j = 0; i < messageLength ; ++i, ++j)
    {
        if(j == keyLength)
        {
            j = 0;
        }

        if(!isalpha(msg[i]))
        {
            printf("Copying %c\n", msg[i]);
            encryptedMsg[i] = msg[i];
            decryptedMsg[i] = msg[i]; // for decrypt
            continue;
        }

        if(isalpha(msg[i]))
        {
            printf("Changing %c for %c\n", msg[i], key[j]);
            //encryptedMsg[i] = (  (msg[i] - 'a' + key[j] - 'a') % 26 )  + 'a'; // for encrypt
            int letter = (( msg[i] -'a')  - (key[j] -'a') + 26 ) % 26; // for decryot
            decryptedMsg[i] = letter + 'a';
        }
    }




    printf("Original Message: %s", msg);
    printf("\nKey: %s", key);
    //printf("\nEncrypted Message: %s", encryptedMsg);
    printf("\nDecrypted Message: %s", decryptedMsg);

    return 0;
}