#include "headers.h"
#include "fg.h"

void fg(char *input, char *home_dir, int *no_of_bg_processes, struct background_process *bg_processes, int bg_flag)
{
    int flag = 0;
    char *token = strtok(input, " \t\n"); // contains the word fg
    token = strtok(NULL, " \t\n");        // contains the pid of the process
    if (token == NULL)
    {
        printf("Usage: fg <pid>\n");
        return;
    }
    int pid = atoi(token);

    if(waitpid(pid, &flag, 0) == -1)
    {
        printf("No such process found\n");
    }
    
}