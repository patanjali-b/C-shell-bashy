#ifndef __IOREDIRECT_H
#define __IOREDIRECT_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void io_redirect(char *input, char *home_dir, int* no_of_bg_processes, struct background_process *bg_processes, int bg_flag);


#endif