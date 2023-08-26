#include "headers.h"

int flag_d = 0;
int flag_f = 0;
int flag_e = 0;

char seek_path[4096];

char seek_homeDir[4096] = {0};

void seekPre(char *input_full, char *seek_home)
{

    char *tok = strtok(input_full, " \n\t\r");
    tok = strtok(NULL, " \n\t\r");
    while (strstr(tok, "-"))
    {
        if (!strcmp(tok, "-d"))
        {
            flag_d = 1;
        }
        else if (!strcmp(tok, "-f"))
        {
            flag_f = 1;
        }
        else if (!strcmp(tok, "-e"))
        {
            flag_e = 1;
        }
        tok = strtok(NULL, " \n\t\r");
    }

    if(flag_d && flag_f){
        printf("No matching flags\n");
        return;
    }

    char *input = tok;
    seek_home = strtok(NULL, " \n\t\r");

    if (!seek_home)
    {
        seek_home = (char *)malloc(4096 * sizeof(char));
        seek_home = getcwd(NULL, 0);
    }

    int count = 0;
    int ForD = 0;

    strcpy(seek_homeDir, seek_home);
    seek(input, seek_home, &count, &ForD);

    if (flag_e && count == 1)
    {
        if (!ForD)
        {
            chdir(seek_home);
            chdir(seek_path);
        }
        else
        {
            char buf[4097] = {0};
            FILE *f = fopen(strcat(seek_home, seek_path + 1), "r");
            while (fread(buf,1, 4096, f))
            {
                printf("%s", buf);
            }
        }
    }
    flag_d = flag_f = flag_e = 0;
    
}
// }

void seek(char *input, char *seek_home, int *count, int *ForD)
{

    DIR *d;
    d = opendir(seek_home);

    if(!d){
        printf("No such directory\n");
        return;
    }
    struct dirent *dir;

    char *temp = (char *)malloc(4096 * sizeof(char));
    strcpy(temp, input);


    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
        {
            continue;
        }

        // printf("Entered reading directory\n");
        // printf("seek_home: %s\n", seek_home);
        // printf("curr: %s\n", dir->d_name);
        if (dir->d_type == 4)
        {
            strcat(seek_home, "/");
            strcat(seek_home, dir->d_name);

            if ((flag_d == 1 || (flag_d == 0 && flag_f == 0)) && !strcmp(input, dir->d_name))
            {

                char *temp = (char *)malloc(4096 * sizeof(char));
                temp[0] = '.';
                strcpy(temp + 1, seek_home + strlen(seek_homeDir));

                printf("%s\n", temp);
                strcpy(seek_path,temp);
                (*count)++;
            }

            seek(input, seek_home, count, ForD);
            seek_home[strlen(seek_home) - strlen(dir->d_name) - 1] = '\0';
        }

        if (dir->d_type == 8)
        {
            char *token = (char *)malloc(4096);
            strcpy(token, dir->d_name);

            if (strstr(token, "."))
            {
                token = strtok(token, ".");
            }

            if ((flag_f == 1 || (flag_d == 0 && flag_f == 0 )) && !strcmp(token, temp))

            {
                strcat(seek_home, "/");
                strcat(seek_home, dir->d_name);

                char *temp = (char *)malloc(4096 * sizeof(char));
                temp[0] = '.';
                strcpy(temp + 1, seek_home + strlen(seek_homeDir));

                printf("%s\n", temp);

                strcpy(seek_path,temp);
                seek_home[strlen(seek_home) - strlen(dir->d_name) - 1] = '\0';
                *ForD = 1;

                (*count)++;
            }
        }
    }
}
