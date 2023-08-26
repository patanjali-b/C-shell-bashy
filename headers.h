#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>

#include "prompt.h"
#include "command.h"
#include "warp.h"
#include "colours.h"
#include "peek.h"
#include "dirent.h"
#include "proclore.h"
#include "pastevents.h"
#include "sys_commands.h"
#include "newprompt.h"
#include "seek.h"
#include "bg.h"

static char prev_dir[1024] = "";
extern char history[15][1000];
extern int counter_pastevents;
extern int execution_time_seconds;
extern int return_for_execvp;
extern char semicolon_tokens[1000][1000];
extern int semicolon_counter;
extern char ampersand_tokens[1000][1000];
extern int ampersand_counter;


#endif