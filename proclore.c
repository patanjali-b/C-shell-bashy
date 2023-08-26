#include "headers.h"

void proclore(char *input, char *home)
{
    
    char *token;
    token = strtok(input, " \n\t\r"); // this basically has proclore
    
    while (token != NULL)
    {
        
        token = strtok(NULL, " \n\t\r");
        
       
        if (token == NULL) // I have to print the process of the terminal.
        {
            
            // Printing pid of the terminal, process Status (R/R+/S/S+/Z), Process group, Virtual Memory ,Executable path of process using STDOUT
            pid_t pid = getpid();
            char status;
            char status_path[100];
            snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
            FILE *status_file = fopen(status_path, "r");
            if (status_file)
            {
                fscanf(status_file, "State:\t%c", &status);
                fclose(status_file);
            }
            else
            {
                perror("Error in opening status file");
            }

            // get the status of the process

            status = status == 'R' ? 'R' : status == 'S' ? 'S' : status == 'Z' ? 'Z' : 'R';

            char *status_string;

            // to check if the process is foreground or background

            pid_t pgid = getpgid(pid);
            int foreground = 0;

            if (pgid == tcgetpgrp(STDIN_FILENO))
            {
                status_string = "Foreground";
                foreground = 1;
            }
            else
            {
                status_string = "Background";
            }
            // printing the process group
            printf("pid : %d\n", pid);
            if (foreground)
            {
                printf("Process Status : %c%c\n", status, '+');
            }
            else
            {
                printf("Process Status : %c\n", status);
            }

            // printing the process group
            printf("Process Group : %d\n", pgid);

            // printing the virtual memory
            char mem_path[100];
            snprintf(mem_path, sizeof(mem_path), "/proc/%d/statm", pid);
            FILE *mem_file = fopen(mem_path, "r");
            if (mem_file)
            {
                unsigned long size;
                fscanf(mem_file, "%lu", &size);
                fclose(mem_file);
                printf("Memory : %lu\n", size);
            }
            else
            {
                perror("Error in opening memory file");
            }

            // printing the executable path

            char exec_path[100];
            snprintf(exec_path, sizeof(exec_path), "/proc/%d/exe", pid);
            char exec_path_final[100];
            int exec_path_len = readlink(exec_path, exec_path_final, sizeof(exec_path_final));
            if (exec_path_len == -1)
            {
                perror("Error in reading executable path");
            }
            else
            {
                exec_path_final[exec_path_len] = '\0';
                printf("Executable Path : %s\n", exec_path_final);
            }
        }

        else
        {
            // get the pid of the process and do the same
            pid_t pid = atoi(token);
            char status;
            char status_path[100];
            snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
            FILE *status_file = fopen(status_path, "r");

            if (status_file)
            {
                fscanf(status_file, "State:\t%c", &status);
                fclose(status_file);
            }
            else
            {
                perror("Error in opening status file");
            }

            // get the status of the process

            status = status == 'R' ? 'R' : status == 'S' ? 'S' : status == 'Z' ? 'Z' : 'T';

            char *status_string;

            // to check if the process is foreground or background

            pid_t pgid = getpgid(pid);
            int foreground = 0;

            if (pgid == tcgetpgrp(STDIN_FILENO))
            {
                status_string = "Foreground";
                foreground = 1;
            }
            else
            {
                status_string = "Background";
            }

            // printing the process group
            printf("pid : %d\n", pid);
            if (foreground)
            {
                printf("Process Status : %c%c\n", status, '+');
            }
            else
            {
                printf("Process Status : %c\n", status);
            }

            // printing the process group
            printf("Process Group : %d\n", pgid);

            // printing the virtual memory
            char mem_path[100];
            snprintf(mem_path, sizeof(mem_path), "/proc/%d/statm", pid);
            FILE *mem_file = fopen(mem_path, "r");
            if (mem_file)
            {
                unsigned long size;
                fscanf(mem_file, "%lu", &size);
                fclose(mem_file);
                printf("Memory : %lu\n", size);
            }
            else
            {
                perror("Error in opening memory file");
            }

            // printing the executable path

            char exec_path[100];
            snprintf(exec_path, sizeof(exec_path), "/proc/%d/exe", pid);
            char exec_path_final[100];
            int exec_path_len = readlink(exec_path, exec_path_final, sizeof(exec_path_final));
            if (exec_path_len == -1)
            {
                perror("Error in reading executable path");
            }
            else
            {
                exec_path_final[exec_path_len] = '\0';
                printf("Executable Path : %s\n", exec_path_final);
            }

            break;
        }
    }
}