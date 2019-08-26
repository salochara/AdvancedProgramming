#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    int option, input, output;

    printf("This program has %d arguments \n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("\t%d: %s \n",i,argv[i]);
    }
    // getopt; for parsing the arguments passed to the programs
    while((option = getopt(argc,argv,"i:o:n:yc::")) != -1) // : are a string mandatory after the flag, :: are optional)
    {
        switch(option)
        {
            case 'i':
                input = atoi(optarg);
                break;
            case 'o':
                input = atoi(optarg);
                break;
            case '?': // getopt returns a question mark if it's any other than what it's declared
                printf("Unknown option %c \n", option);
                break;
        }
    }
    printf("Remaining arguments: \n");
    for (int j = optind; j < argc; ++j) { //optind= index from argv where it's working, index where the unknown flags start
        printf("\targv[i] = %s \n", argv[j]);
    }

    return 0;
}