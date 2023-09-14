#include "headers.h"
#include "bg.h"
#include "signal.h"



void bg(char *input, char *home)
{
    char *command = strtok(input, " \t\n"); // this contains the word bg
    command = strtok(NULL, " \t\n");        // this contains the pid of the process
    if (command == NULL)
    {
        printf("Usage: bg <pid>\n");
        return;
    }
    int pid = atoi(command);

    // check if pid exists and ask it to continue running
    if(kill(pid, 0) == -1)
    {
        printf("No such process found\n");
        return;
    }
    kill(pid, SIGCONT);

}