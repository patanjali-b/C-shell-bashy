#include "headers.h"
#include "sys_commands.h"
#include "command.h"

char history[15][1000] = {};
counter_pastevents = 0;
int pastevents_flag = 0;

void command(char *input, char *home, int *no_of_bg_processes, struct background_process bg_processes[], int bg_flag)
{

    // intializing the background process array

    int flag_pipe = 0;

    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '|')
        {
            flag_pipe = 1;
            break;
        }
    }

    int flag_redirection = 0;

    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '>' || input[i] == '<')
        {
            flag_redirection = 1;
            break;
        }
    }

    if (flag_pipe == 1)
    {
        pipe_func(input, home, no_of_bg_processes, bg_processes, bg_flag);
        return;
    }

    if (flag_redirection == 1)
    {
        io_redirect(input, home, no_of_bg_processes, bg_processes, bg_flag);
        return;
    }

    char *input_copy = (char *)malloc(4096 * sizeof(char));
    strcpy(input_copy, input);

    char *cwd = (char *)malloc(1024 * sizeof(char));
    getcwd(cwd, 1024);

    int flag_semicolon = 0;

    char *token = strtok(input, " \n\t\r");
    char *command_ip = token;

    // accounting for ; and &

    if (flag_semicolon == 0)
    {

        if (strcmp(token, "pastevents") != 0)
        {
            strcpy(history[counter_pastevents], input_copy);
            counter_pastevents++;
            if (counter_pastevents == 15)
            {
                counter_pastevents = 0;
            }
        }

        if (strcmp(command_ip, "warp") == 0)
        {
            warp_func(input_copy, home, prev_dir);
        }

        else if (strcmp(command_ip, "peek") == 0)
        {
            peek(input_copy, home);
        }

        else if (strcmp(command_ip, "proclore") == 0)
        {
            proclore(input_copy, home);
        }

        else if (strcmp(command_ip, "pastevents") == 0)
        {
            // pastevents(input_copy, home);
            token = strtok(NULL, " \n\t\r");
            if (token == NULL && counter_pastevents > 0)
            {
                // check if the string is empty, if it is not then print it
                for (int i = 0; i < 15; i++)
                {
                    if (strcmp(history[i], "") != 0)
                    {
                        printf("%d. %s", i + 1, history[i]);
                    }
                    else
                    {
                        break;
                    }
                }
            }

            else if (strcmp(token, "purge") == 0)
            {
                for (int i = 0; i < 15; i++)
                {
                    strcpy(history[i], "");
                }
                counter_pastevents = 0;
            }

            else if (strcmp(token, "execute") == 0)
            {
                token = strtok(NULL, " \n\t\r");
                int index = atoi(token);
                if (index >= 0 && index < 15)
                {
                    command(history[index - 1], home, &no_of_bg_processes, bg_processes, 0);
                }
                else
                {
                    printf("Invalid index\n");
                }
            }
        }
        else if (strcmp(command_ip, "seek") == 0)
        {
            seekPre(input_copy, home);
        }
        else if (strcmp(command_ip, "iMan") == 0)
        {
            iMan(input_copy, home);
        }
        else if (strcmp(command_ip, "activities") == 0)
        {
            activities(input_copy, home, bg_processes);
        }
        else if (strcmp(command_ip, "ping") == 0)
        {
            ping(input_copy, home, &no_of_bg_processes, bg_processes, bg_flag);
        }
        else if (strcmp(command_ip, "fg") == 0)
        {
            fg(input_copy, home, &no_of_bg_processes, bg_processes, bg_flag);
        }
        else if (strcmp(command_ip, "bg") == 0)
        {
            bg(input_copy, home);
        }
        else if (strcmp(command_ip, "neonate") == 0)
        {
            neonate(input_copy, home);
        }
        else
        {

            syscommands(input_copy, home, &no_of_bg_processes, bg_processes, bg_flag);
        }

        while (token != NULL)
        {

            token = strtok(NULL, " ");
        }
    }

    return;
}