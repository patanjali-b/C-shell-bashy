#include "headers.h"
#include <signal.h>
#include "ping.h"

void ping(char *input, char *home_dir, int* no_of_bg_processes, struct background_process *bg_processes, int bg_flag)
{
    // implementing signals
    char *token = strtok(input, " \t\n"); // contains the word ping
    token = strtok(NULL, " \t\n");        // contains the pid of the process
    if(token == NULL){
        printf("Usage: ping <pid> <signal>\n");
        return;
    }
    int pid = atoi(token);
    
    token = strtok(NULL, " \t\n"); // contains the signal number
    if(token == NULL){
        printf("Usage: ping <pid> <signal>\n");
        return;
    }
    int sig = atoi(token);

    int actual_signal = sig%32;

     if (kill(pid, 0) == -1) {
        if (errno == ESRCH) {
            fprintf(stderr, "No such process found\n");
            return;
        } else {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    }

    if (kill(pid, actual_signal) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }

    printf("Sent signal %d to process with pid %d\n", actual_signal, pid);
    printf("Done\n");
    

}