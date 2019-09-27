//
// Created by Salomón Charabati on 2019-09-26.
//

#include<stdio.h>
#include<string.h>
#include <ctype.h>

int main()
{
    char msg[] = "this should be very interesting, and not too difficult. you can use all lowercase letters for your messages, encryptions and keys";
    char key[] = "prograavanzada";



        int msgLen = strlen(msg), keyLen = strlen(key), i, j;

        char newKey[msgLen], encryptedMsg[msgLen], decryptedMsg[msgLen];

        //generating new key
        for(i = 0, j = 0; i < msgLen; ++i, ++j){
            if(j == keyLen)
                j = 0;

            newKey[i] = key[j];
        }

        newKey[i] = '\0';

        //encryption
        for(i = 0, j= 0; i < msgLen;)
        {
            if(!isalpha(msg[i]))
            {
                encryptedMsg[i] = msg[i];
                encryptedMsg[i] = msg[i];
            }else{
                encryptedMsg[i] = (  (msg[i] - 'a' + newKey[i] - 'a') % 26 )  + 'a';
            }

        }
        encryptedMsg[i] = '\0';

        //decryption
        for(i = 0; i < msgLen; ++i)
            decryptedMsg[i] = (((encryptedMsg[i] - newKey[i]) + 26) % 26) + 'A';

        decryptedMsg[i] = '\0';

        printf("Original Message: %s", msg);
        printf("\nKey: %s", key);
        printf("\nNew Generated Key: %s", newKey);
        printf("\nEncrypted Message: %s", encryptedMsg);
        printf("\nDecrypted Message: %s", decryptedMsg);

        return 0;
    }