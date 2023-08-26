#ifndef __SYSCOMMANDS_H
#define __SYSCOMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "time.h"
#include <sys/time.h>

void syscommands(char *input, char *home_dir);

#endif