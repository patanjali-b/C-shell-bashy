#include "headers.h"
#include "sys_commands.h"

execution_time_seconds = 0;
return_for_execvp = 100;


void handle_sigchld(int sig)
{
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("pid %d exited succesfully\n", pid);
    }
}

void syscommands(char *input, char *home, int *no_of_bg_processes, struct background_process *bg_processes, int bg_flag)
{
    char ch;
    // check if there is an & in the input
    int flag_ampersand = 0;
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '&')
        {
            flag_ampersand = 1;
            break;
        }
    }

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
                        printf("no of bg processes: %d\n", *no_of_bg_processes);

                        bg_processes[*no_of_bg_processes].pid = childID;
                        printf("no of bg cv: %d\n", *no_of_bg_processes);

                        strcpy(bg_processes[*no_of_bg_processes].name, args[0]);
                        // copy the arguments into the args of the bg_processes
                        for (int j = 0; j < i; j++)
                        {
                            strcpy(bg_processes[*no_of_bg_processes].args[j], args[j]);
                        }
                        
                        // make the last argument NULL
                        bg_processes[*no_of_bg_processes].args[i][0] = '\0';
                        (*no_of_bg_processes)++;

                        printf("no of bg processes: %d\n", *no_of_bg_processes);
                        printf("First commands is %s\n", bg_processes[0].name);

                        args[i] = NULL;

                        int childID2 = fork();
                        
                        
                        if(childID2 == 0)
                        {
                            setpgid(0, 0);
                            // how to print the status of the child process?
                            signal(SIGCHLD, handle_sigchld);
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