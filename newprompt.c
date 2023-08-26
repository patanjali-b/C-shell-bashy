#include "headers.h"

void newprompt(char *home)
{
    char username[1024];
    char systemname[1024];
    getlogin_r(username, 1024);
    gethostname(systemname, 1024);

    char cwd[1024];
    getcwd(cwd, 1024);

    char *curr_dir = cwd;
   
    
    if (strcmp(curr_dir, home) == 0)
    {
        strcpy(cwd, "~");
    }
    else if (strstr(curr_dir, home) != NULL)
    {
        
        char *temp = (char *)malloc(1024 * sizeof(char));
        temp[0] = '~';
        strcpy(temp + 1, curr_dir + strlen(home));
        strcpy(cwd, temp);
    }

    char *last_event = history[counter_pastevents-1];
    char *token = strtok(last_event, " \n\t\r");
    
    printf("%s<%s@%s:%s%s %s : %ds>%s\n", GREEN, username, systemname, BLUE, cwd, token,execution_time_seconds,  WHITE);
}