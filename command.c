#include "headers.h"

char history[15][1000] = {};
counter_pastevents = 0;
int pastevents_flag = 0;

void command(char *input, char *home)
{

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
                    command(history[index - 1], home);

                }
                else
                {
                    printf("Invalid index\n");
                }
            }
        }
        else if(strcmp(command_ip ,"seek") == 0)
        {
            seekPre(input_copy, home); 
        }
        else
        {
            syscommands(input_copy, home);
        }

        while (token != NULL)
        {

            token = strtok(NULL, " ");
        }
    }
}