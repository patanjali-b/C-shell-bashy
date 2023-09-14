#include "headers.h"
#include "command.h"
#include "sys_commands.h"
#include "io_redirection.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 1024

void io_redirect(char *input, char *home_dir, int *no_of_bg_processes, struct background_process *bg_processes, int bg_flag)
{
    char *temp_input = input;
    char *args[MAX_COMMAND_LENGTH];
    int arg_count = 0;
    char *token = strtok(temp_input, " ");

    while (token != NULL)
    {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    args[arg_count] = NULL;

    // Check for input/output redirection
    int input_fd = 0;  // Default to stdin
    int output_fd = 1; // Default to stdout
    int append = 0;    // Flag for > or >>

    for (int i = 0; i < arg_count; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            int len = strlen(args[i + 1]);
            if (args[i + 1][len - 1] == '\n')
            {
                args[i + 1][len - 1] = '\0';
            }
            // Input redirection
            input_fd = open(args[i + 1], O_RDONLY);
            if (input_fd == -1)
            {
                perror("No such input file found!");
                return;
            }
            args[i] = NULL; // Remove "<" from arguments
        }
        else if (strcmp(args[i], ">") == 0)
        {
            int len = strlen(args[i + 1]);
            if (args[i + 1][len - 1] == '\n')
            {
                args[i + 1][len - 1] = '\0';
            }
            // Output redirection (overwrite)
            output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1)
            {
                perror("Failed to create output file!");
                return;
            }
            args[i] = NULL; // Remove ">" from arguments
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            int len = strlen(args[i + 1]);
            if (args[i + 1][len - 1] == '\n')
            {
                args[i + 1][len - 1] = '\0';
            }
            // Output redirection (append)
            output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd == -1)
            {
                perror("Failed to create output file!");
                return;
            }
            append = 1;
            args[i] = NULL; // Remove ">>" from arguments
        }
    }

    // Fork and execute the temp_input
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork failed");
        return;
    }
    else if (pid == 0)
    {
        // Child process
        if (input_fd != 0)
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != 1)
        {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Execute the command
        // execvp(args[0], args);

        // attach all the elements of args to a string until a null is encountered
        char cmd[MAX_COMMAND_LENGTH];
        strcpy(cmd, args[0]);
        for (int i = 0; i < arg_count; i++)
        {
            if(args[i] == NULL)
            {
                break;
            }
            else if(i == 0)
            {
                strcat(cmd, " ");
            }
            else
            {
                strcat(cmd, args[i]);
                strcat(cmd, " ");
            }


            // printf("--> %s", cmd);
        }

        int ret;
        ret = execvp(args[0], args);

        if(ret == -1)
        command(cmd, home_dir, no_of_bg_processes, bg_processes, bg_flag);

        

        // If execvp fails, report the error and exit
        
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        wait(NULL);
    }
}
