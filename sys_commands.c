#include "headers.h"
#include "sys_commands.h"

execution_time_seconds = 0;
return_for_execvp = 100;
counter_bg_processes = 0;

void handle_sigchld(int sig)
{
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, 0)) > 0)
    {
        printf("pid %d exited succesfully\n", pid);
    }
}

void syscommands(char *input, char *home, int *no_of_bg_processes, struct background_process bg_processes[], int bg_flag)
{

    char *command = strtok(input, " \t\n");
    char *args[100];
    int i = 0;

    while (command != NULL)
    {
        args[i] = command;
        command = strtok(NULL, " \t\n");
        i++;
    }
    args[i] = NULL;

    if (bg_flag == 1) // BACKGROUND PROCESS
    {
        int pid = fork();

        if (pid == 0)
        {
            printf("[%d]\n", getpid());
            
            if (execvp(args[0], args) < 0)
            {
                perror("Error");
                exit(0);
            }
        }
        else
        {
            int status;
            bg_processes[counter_bg_processes].pid = pid;
            strcpy(bg_processes[counter_bg_processes].name, args[0]);
            // copy the arguments into args
            for (int i = 0; i < 100; i++)
            {
                if (args[i] == NULL)
                {
                    break;
                }
                strcpy(bg_processes[counter_bg_processes].args[i], args[i]);
            }
            counter_bg_processes++;
            signal(SIGCHLD, handle_sigchld);
        }
    }

    else // FOREGROUND PROCESS
    {
        int pid = fork();
        if (pid == 0)
        {
            if (execvp(args[0], args) < 0)
            {
                perror("Error");
                exit(0);
            }
        }
        else
        {
            int status;

            struct timeval start_time, end_time;
            gettimeofday(&start_time, NULL);

            waitpid(pid, &status, WUNTRACED);

            gettimeofday(&end_time, NULL);
            execution_time_seconds = end_time.tv_sec - start_time.tv_sec;
        }
    }
}