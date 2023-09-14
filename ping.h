#ifndef __PING_H
#define __PING_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

void ping(char *input, char *home_dir, int* no_of_bg_processes, struct background_process *bg_processes, int bg_flag);


#endif