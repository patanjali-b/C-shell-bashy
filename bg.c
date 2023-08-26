#include "headers.h"

void handle_sigchld(int sig)
{
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("pid %d exited\n", pid);
    }
}

void command_background(char *input, char* home)
{
     signal(SIGCHLD, handle_sigchld); 
    // using the command function
    int is_background = 0;
    pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            char *args[1000];
            char *token = strtok(input, " \n");
            int i = 0;
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " \n");
            }
            args[i] = NULL;
            int is_background = 0;
            if (strcmp(args[i - 1], "&") == 0) {
                is_background = 1;
                args[i - 1] = NULL;
            }
            if (is_background) {
                setpgid(0, 0);
            }
            if (execvp(args[0], args) < 0) {
                perror("Exec failed");
                exit(1);
            }
            
        } else {
            // Parent process
            if (!is_background) {
                wait(NULL);
            }
        }

}