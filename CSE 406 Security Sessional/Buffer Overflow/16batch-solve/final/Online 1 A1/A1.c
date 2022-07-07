
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int foo(char *str)
{
    int arr[40];
    char buffer[669];

    /* The following statement has a buffer overflow problem */ 
    strcpy(buffer, str);

    return 1;
}

int main(int argc, char **argv)
{
    char str[889];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 889, badfile);
    foo(str);

    printf("Try Again\n");
    return 1;
}

