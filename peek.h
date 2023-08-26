#ifndef __PEEK_H
#define __PEEK_H
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include "string.h"

void peek(char* input, char* home);

#endif