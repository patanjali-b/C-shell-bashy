#ifndef __FG_H
#define __FG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void fg(char* input, char *home_dir, int* no_of_bg_processes, struct background_process *bg_processes, int bg_flag);

#endif

