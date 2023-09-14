#include "headers.h"
#include "sys_commands.h"
#include "command.h"
#include "pipe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

void pipe_func(char *input, char *home_dir, int *no_of_bg_processes, struct background_process *bg_processes, int bg_flag)
{
    // Split the input into individual commands using the pipe symbol as the delimiter
    char *commands[MAX_COMMAND_LENGTH];
    int num_commands = 0;
    char *token = strtok(input, "|");
    while (token != NULL)
    {
        commands[num_commands] = token;
        num_commands++;
        token = strtok(NULL, "|");
    }

    // Check for invalid use of pipe
    if (num_commands < 2)
    {
        printf("Invalid use of pipe\n");
        return;
    }

    // Create pipes for communication between commands
    int pipes[num_commands - 1][2];
    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Execute commands sequentially from left to right
    for (int i = 0; i < num_commands; i++)
    {
        // Fork a child process
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) // Child process
        {
            // Close unnecessary pipe ends
            if (i > 0)
            {
                close(pipes[i - 1][1]); // Close write end of previous pipe
                dup2(pipes[i - 1][0], STDIN_FILENO); // Redirect stdin from the previous pipe
                close(pipes[i - 1][0]); // Close read end of previous pipe
            }
            if (i < num_commands - 1)
            {
                close(pipes[i][0]); // Close read end of current pipe
                dup2(pipes[i][1], STDOUT_FILENO); // Redirect stdout to the current pipe
                close(pipes[i][1]); // Close write end of current pipe
            }

            // Execute the command
            command(commands[i], home_dir, no_of_bg_processes, bg_processes, bg_flag);

            // Exit the child process
            exit(EXIT_SUCCESS);
        }
        else // Parent process
        {
            // Close pipe ends in the parent
            if (i > 0)
            {
                close(pipes[i - 1][0]); // Close read end of previous pipe
                close(pipes[i - 1][1]); // Close write end of previous pipe
            }
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++)
    {
        wait(NULL);
    }
}
