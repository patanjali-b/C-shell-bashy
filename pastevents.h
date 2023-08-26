#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "string.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

void pastevents(char* input, char* home);

#endif