#include <stdio.h>
#include <string.h>

int main()
{
    char *original = "Hello ;  world!";

    char *token = strtok(original, ";");
    printf("reached here\n");

    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, ";");
    }

    return 0;
}

