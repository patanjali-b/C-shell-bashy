#include "headers.h"



void warp_func(char *input, char *home, char *prev_dir)
{
   
    char *token = strtok(input, " \n\t\r"); // this has the string warp basically.
    int counter = 0;

    char temp_prev_dir[1024] = "";

    

    while (token != NULL)
    {

        int ret_for_chdir;
        token = strtok(NULL, " \n\t\r");
        
        counter++;

        if (token != NULL)
        {
            // printf("%s\n", token);
            // token[strlen(token)-1] = '\0';
            // printf("%ld\n", strlen(token));
        }

        if(token == NULL && counter == 1)
        {
            ret_for_chdir = chdir(home);
        }

        
        if (token != NULL)
        {
            if (strcmp(token, "~") == 0)
            {
                getcwd(prev_dir, 1024);
                
                ret_for_chdir = chdir(home);
            }

            else if(strcmp(token, "..") == 0)
            {
                getcwd(prev_dir, 1024);
                ret_for_chdir = chdir("..");
            }

            else if(strcmp(token, "-") == 0)
            {
                getcwd(temp_prev_dir, 1024);
                
                ret_for_chdir = chdir(prev_dir);
                strcpy(prev_dir, temp_prev_dir);

                
            }

            else
            { 
                
                getcwd(prev_dir, 1024);
                ret_for_chdir = chdir(token);
                char *temp = getcwd(NULL, 0);
                           
            }
        }
    }


    

   
}