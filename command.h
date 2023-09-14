#ifndef __COMMAND_H
#define __COMMAND_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void command(char *input, char *home_dir, int* no_of_bg_processes, struct background_process *bg_processes, int bg_flag);

#endif