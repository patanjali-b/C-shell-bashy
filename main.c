#include "headers.h"
#include "sys_commands.h"
#include <signal.h>

char semicolon_tokens[1000][1000] = {};
semicolon_counter = 0;
char ampersand_tokens[1000][1000] = {};
ampersand_counter = 0;

pid_t running_process = 0;

int flag1 = 0, flag2 = 0;

struct background_process bg_processes[1000];

void sigintHandler(int sig_num)
{

    if (flag1 == 1)
    {
        printf("Ctrl C pressed\n");
        // int id = getpid();
        flag1 = 0;
        kill(0, SIGINT);
    }
}

void sigtstpHandler(int sig_num)
{
    if(flag2 == 1)
    {
        printf("Ctrl Z pressed\n");
        flag2 = 0;
        kill(0, SIGTSTP);
    }
}



int main()
{

    int no_of_bg_processes = 0;

    // bg_processes = (struct background_process *)malloc(1000 * sizeof(struct background_process));

    char *home = (char *)malloc(1024 * sizeof(char));
    home = getcwd(NULL, 0);

    int no_of_ampersand = 0;

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input

        signal(SIGINT, sigintHandler);
        signal(SIGTSTP, sigtstpHandler);


        if (execution_time_seconds > 2)
        {
            newprompt(home);
        }
        else
        {
            prompt(home);
        }int id = getpid();
    

        // prompt(home);
        execution_time_seconds = 0;
        char input[4096];
        fgets(input, 4096, stdin);

        // if end of file detected return
        if (feof(stdin))
        {
            printf("Logging out\n");
            return 0;
        }

        int flag_semicolon = 0;
        int flag_ampersand = 0;

        for (int i = 0; i < strlen(input); i++)
        {
            if (input[i] == ';')
            {
                flag_semicolon = 1;
                break;
            }
        }

        for (int i = 0; i < strlen(input); i++)
        {
            if (input[i] == '&')
            {
                flag_ampersand = 1;
                no_of_ampersand++;
            }
        }

        if (flag_semicolon == 1)
        {
            char *semicolon_token = strtok(input, ";");

            while (semicolon_token != NULL)
            {
                char *trimmed_token = semicolon_token;
                while (*trimmed_token == ' ' || *trimmed_token == '\n')
                {
                    trimmed_token++;
                }
                int len = strlen(trimmed_token);
                while (len > 0 && (trimmed_token[len - 1] == ' ' || trimmed_token[len - 1] == '\n'))
                {
                    trimmed_token[len - 1] = '\0';
                    len--;
                }

                strcpy(semicolon_tokens[semicolon_counter++], trimmed_token);
                semicolon_token = strtok(NULL, ";");
            }

            for (int i = 0; i < semicolon_counter; i++)
            {
                if (strlen(semicolon_tokens[i]) == 0)
                {
                    continue;
                }
                execution_time_seconds = 0;
                command(semicolon_tokens[i], home, &no_of_bg_processes, bg_processes, 0);
            }
        }
        if (flag_semicolon == 0 && flag_ampersand == 0)
        {

            execution_time_seconds = 0;
            command(input, home, &no_of_bg_processes, bg_processes, 0);
        }

        if (flag_ampersand == 1)
        {
            char *ampersand_token = strtok(input, "&");

            while (ampersand_token != NULL)
            {
                char *trimmed_token = ampersand_token;
                while (*trimmed_token == ' ' || *trimmed_token == '\n')
                {
                    trimmed_token++;
                }
                int len = strlen(trimmed_token);
                while (len > 0 && (trimmed_token[len - 1] == ' ' || trimmed_token[len - 1] == '\n'))
                {
                    trimmed_token[len - 1] = '\0';
                    len--;
                }

                strcpy(ampersand_tokens[ampersand_counter++], trimmed_token);
                ampersand_token = strtok(NULL, "&");
            }

            for (int i = 0; i < ampersand_counter; i++)
            {
                if (strlen(ampersand_tokens[i]) == 0)
                {
                    continue;
                }
                execution_time_seconds = 0;
                // the command before the & is a background process
                if (i <= no_of_ampersand - 1)
                {
                    command(ampersand_tokens[i], home, &no_of_bg_processes, bg_processes, 1);
                }
                else
                {
                    command(ampersand_tokens[i], home, &no_of_bg_processes, bg_processes, 0);
                }
            }
        }

        // clear the semi colon tokens
        for (int i = 0; i < 1000; i++)
        {
            semicolon_tokens[i][0] = '\0';
        }
        semicolon_counter = 0;

        // clear the ampersand tokens
        for (int i = 0; i < 1000; i++)
        {
            ampersand_tokens[i][0] = '\0';
        }
        ampersand_counter = 0;
    }
}
