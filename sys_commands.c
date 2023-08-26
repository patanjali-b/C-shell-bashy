#include "headers.h"

execution_time_seconds = 0;
return_for_execvp = 100;

void syscommands(char *input, char *home)
{
    char *token = strtok(input, " \n\t\r");
    int counter = 0;
    int flag = 0;
    int ret_for_execvp = 100;
    int last_but_one = 0;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    while (token != NULL)
    {

        int childID = fork();

        if (childID == 0)
        {
            char *args[100];
            int i = 0;
            while (token != NULL)
            {

                args[i] = token;

                i++;
                token = strtok(NULL, " \n\t\r");

                if (token != NULL)
                {
                    if (strcmp(token, "&") == 0)
                    {
                        // Background process
                        // printf("Background process\n");

                        args[i] = NULL;

                        int childID2 = fork();
                        
                        if(childID2 == 0)
                        {
                            setpgid(0, 0);
                            // how to print the status of the child process?
                            return_for_execvp = execvp(args[0], args);
                            perror("execvp");

                        }
                        else
                        {
                            // wait(NULL);
                            printf("pid: %d\n", childID2);
                            exit(0);
                        }

                        token = NULL;
                    }
                }
            }

            args[i] = NULL;

            return_for_execvp = execvp(args[0], args);

            flag = 1;
            break;
        }
        else
        {
            wait(NULL);

            token = strtok(NULL, " \n\t\r");
        }
        break;
    }

    gettimeofday(&end_time, NULL);

    execution_time_seconds = end_time.tv_sec - start_time.tv_sec;

    // printf("Execution time: %d\n", execution_time_seconds);

    if (return_for_execvp == -1)
    {
        printf("Command not found\n");
    }
}