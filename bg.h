#ifndef __BG_H
#define __BG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void command_background(char *input, char *home);

#endif