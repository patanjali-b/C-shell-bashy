#include "headers.h"
#include "neonate.h"
#include "signal.h"
#include <termios.h>
#include <fcntl.h>

#define MAX_LOADAVG_LEN 256

// Function to set terminal to non-blocking mode
void setNonBlockingMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to check if 'x' key is pressed
int isXKeyPressed() {
    char ch;
    ssize_t key_read = read(STDIN_FILENO, &ch, 1);
    return (key_read == 1 && ch == 'x');
}

void neonate(char *input, char *home) {
    char *token = strtok(input, " \t\n"); // this contains the word neonate
    token = strtok(NULL, " \t\n");        // this contains -n
    token = strtok(NULL, " \t\n");        // this contains the time args
    if (token == NULL) {
        printf("Usage: neonate -n <time>\n");
        return;
    }
    int time_arg = atoi(token);

    // Open the loadavg file to get the PID of the last created process
    int loadavg_fd = open("/proc/loadavg", O_RDONLY);
    if (loadavg_fd == -1) {
        perror("Error opening /proc/loadavg");
        return;
    }

    // Set terminal to non-blocking mode
    setNonBlockingMode();

    char last_pid[MAX_LOADAVG_LEN];

    while (1) {
        // Read the loadavg file to get the last PID
        int loadavg_fd = open("/proc/loadavg", O_RDONLY);
	    if (loadavg_fd == -1) {
		perror("Error opening /proc/loadavg");
		return;
	    }
        ssize_t bytes_read = read(loadavg_fd, last_pid, MAX_LOADAVG_LEN);
        if (bytes_read == -1) {
            perror("Error reading /proc/loadavg");
            break;
        }

        // Print the last PID
        printf("%s", &last_pid[22]);
        fflush(stdout); // Flush the output buffer
        
        // Check if 'x' is pressed
        if (isXKeyPressed()) {
            break;
        }
        
        sleep(time_arg);
    }

    // Restore the terminal settings
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    // Close the loadavg file
    close(loadavg_fd);
}