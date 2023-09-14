#ifndef __ACTIVITIES_H
#define __ACTIVITIES_H
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include "string.h"

void activities(char* input, char* home, struct background_process *bg_processes);

#endif