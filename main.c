#include "headers.h"

char semicolon_tokens[1000][1000] = {};
semicolon_counter = 0;
char ampersand_tokens[1000][1000] = {};
ampersand_counter = 0;

int main()
{
    char *home = (char *)malloc(1024 * sizeof(char));
    home = getcwd(NULL, 0);

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input

        if (execution_time_seconds > 2)
        {
            newprompt(home);
        }
        else
        {
            prompt(home);
        }

        // prompt(home);
        execution_time_seconds = 0;
        char input[4096];
        fgets(input, 4096, stdin);

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
                command(semicolon_tokens[i], home);
            }
        }
        if (flag_semicolon == 0)
        {

            execution_time_seconds = 0;
            command(input, home);
        }

        // clear the semi colon tokens
        for (int i = 0; i < 1000; i++)
        {
            strcpy(semicolon_tokens[i], "");
        }

        
    }
}
