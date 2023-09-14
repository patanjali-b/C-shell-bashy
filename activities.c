#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "activities.h"
#include "errno.h"

void activities(char *input, char *home, struct background_process *bg_processes)
{

    for (int i = 0 ; i < counter_bg_processes; i++)
    {
        pid_t pid = bg_processes[i].pid;

        if (kill(pid, 0) == 0) {

            printf("[%d] : %s - %s\n", pid, bg_processes[i].name, "Running");
        }
        else 
        {
            if (errno == ESRCH)
            {
                continue;
            }
            else
            {
                printf("[%d] : %s - %s\n", pid, bg_processes[i].name, "Stopped");

            }
        }

    }



    // for (int i = 0; i < counter_bg_processes; i++)
    // {
    //     pid_t pid = bg_processes[i].pid;
    //     char *command_name = bg_processes[i].name;

        
    //     char state[20];
    //     snprintf(state, sizeof(state), "%s", "Stopped"); 

        
    //     char cmd[1000];
    //     snprintf(cmd, sizeof(cmd), "ps -p %d -o state=", pid);

        
    //     FILE *pipe = popen(cmd, "r");
    //     if (pipe)
    //     {
    //         if (fgets(state, sizeof(state), pipe) != NULL)
    //         {
                
    //             state[strcspn(state, "\n")] = '\0';
    //         }
    //         pclose(pipe);
    //     }

    //     if(strcmp(state, "R") == 0)
    //     {
    //         snprintf(state, sizeof(state), "%s", "Running");
    //     }
    //     else
    //     {
    //         snprintf(state, sizeof(state), "%s", "Stopped");
    //     }

    //     printf("[%d] : %s - %s\n", pid, command_name, state);
    // }
    // char shell_pid_str[32]; // Assuming process IDs can be represented in 32 characters
    // pid_t shell_pid = getpid();
    // snprintf(shell_pid_str, sizeof(shell_pid_str), "%d", shell_pid);

    // char proc_dir_path[256];
    // snprintf(proc_dir_path, sizeof(proc_dir_path), "/proc/%s/task", shell_pid_str);

    // DIR *proc_dir = opendir(proc_dir_path);
    // if (proc_dir == NULL)
    // {
    //     perror("Error opening /proc directory");
    //     return;
    // }

    // struct dirent *entry;
    // while ((entry = readdir(proc_dir)) != NULL)
    // {
    //     if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    //     {
    //         char task_pid_str[32];
    //         snprintf(task_pid_str, sizeof(task_pid_str), "%s", entry->d_name);

    //         char task_cmdline_path[256];
    //         snprintf(task_cmdline_path, sizeof(task_cmdline_path), "/proc/%s/cmdline", task_pid_str);

    //         FILE *cmdline_file = fopen(task_cmdline_path, "r");
    //         if (cmdline_file != NULL)
    //         {
    //             char cmdline[512]; // Assuming command line can be up to 512 characters
    //             if (fgets(cmdline, sizeof(cmdline), cmdline_file) != NULL)
    //             {
    //                 char state = ' '; // Default state: unknown
    //                 char state_path[256];
    //                 snprintf(state_path, sizeof(state_path), "/proc/%s/stat", task_pid_str);

    //                 FILE *stat_file = fopen(state_path, "r");
    //                 if (stat_file != NULL)
    //                 {
    //                     int state_int;
    //                     if (fscanf(stat_file, "%*d %*s %c", &state) != 1)
    //                     {
    //                         state = ' '; // Set to default if reading fails
    //                     }
    //                     fclose(stat_file);
    //                 }

    //                 // Print the information
    //                 printf("[%s] : %s - %s\n", task_pid_str, cmdline, (state == 'R' ? "Running" : "Stopped"));
    //             }

    //             fclose(cmdline_file);
    //         }
    //     }

    //     // for all the processes in the bg_process array, check if they are still running and print them

    //     for (int i = 0; i < counter_bg_processes; i++)
    //     {
    //         int pid = bg_processes[i].pid;
    //         char pid_str[32];
    //         snprintf(pid_str, sizeof(pid_str), "%d", pid);

    //         char proc_dir_path[256];
    //         snprintf(proc_dir_path, sizeof(proc_dir_path), "/proc/%s/task", pid_str);

    //         DIR *proc_dir = opendir(proc_dir_path);
    //         if (proc_dir == NULL)
    //         {
    //             perror("Error opening /proc directory");
    //             return;
    //         }

    //         struct dirent *entry;
    //         while ((entry = readdir(proc_dir)) != NULL)
    //         {
    //             if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
    //             {
    //                 char task_pid_str[32];
    //                 snprintf(task_pid_str, sizeof(task_pid_str), "%s", entry->d_name);

    //                 char task_cmdline_path[256];
    //                 snprintf(task_cmdline_path, sizeof(task_cmdline_path), "/proc/%s/cmdline", task_pid_str);

    //                 FILE *cmdline_file = fopen(task_cmdline_path, "r");
    //                 if (cmdline_file != NULL)
    //                 {
    //                     char cmdline[512]; // Assuming command line can be up to 512 characters
    //                     if (fgets(cmdline, sizeof(cmdline), cmdline_file) != NULL)
    //                     {
    //                         char state = ' '; // Default state: unknown
    //                         char state_path[256];
    //                         snprintf(state_path, sizeof(state_path), "/proc/%s/stat", task_pid_str);

    //                         FILE *stat_file = fopen(state_path, "r");
    //                         if (stat_file != NULL)
    //                         {
    //                             int state_int;
    //                             if (fscanf(stat_file, "%*d %*s %c", &state) != 1)
    //                             {
    //                                 state = ' '; // Set to default if reading fails
    //                             }
    //                             fclose(stat_file);
    //                         }

    //                         // Print the information
    //                         printf("[%s] : %s - %s\n", task_pid_str, cmdline, (state == 'R' ? "Running" : "Stopped"));
    //                     }

    //                     fclose(cmdline_file);
    //                 }
    //             }
    //         }
    //     }
        
        
    // }

    // closedir(proc_dir);
}
