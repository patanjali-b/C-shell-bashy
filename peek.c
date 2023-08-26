#include "headers.h"

int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

void peek(char *input, char *home)
{

    char *token = strtok(input, " \n\t\r"); // this has the string peek basically.
    int counter = 0;

    char cwd[1024] = "";
    getcwd(cwd, 1024);

    char temp_cwd[1024] = "";
    getcwd(temp_cwd, 1024);

    while (token != NULL)
    {
        token = strtok(NULL, " \n\t\r");
        counter++;

 // This token basically has the second argument of the peek command.

 /*
 It can be either NULL, -l, -a, -la, -al, or a directory name. 
 */

// If its NULL.
        if (token == NULL && counter == 1)
        {
            // print all the files in the current directory.
            DIR *d;
            struct dirent *file;
            d = opendir(cwd);
            int num_of_files = 0;
            if (d)
            {
                while ((file = readdir(d)) != NULL)
                {
                    num_of_files++;
                }
            }

            rewinddir(d);

            char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files
            for (int i = 0; i < num_of_files; i++)
            {
                files[i] = (char *)malloc(1024 * sizeof(char));
            }
            int file_index = 0;
            while ((file = readdir(d)) != NULL)
            {
                strcpy(files[file_index++], file->d_name);
            }
            closedir(d);

            qsort(files, num_of_files, sizeof(char *), compare);

            for (int i = 0; i < num_of_files; i++)
            {
                // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                struct stat file_stat;
                char file_path[1024];
                int folder_path_len = strlen(cwd);
                strcpy(file_path, cwd);
                if (cwd[folder_path_len - 1] != '/')
                {
                    strcat(file_path, "/");
                }
                strcat(file_path, files[i]);
             // Do not print hidden files.
                if (files[i][0] == '.')
                {
                    continue;
                }
                if (stat(file_path, &file_stat) == 0)
                {
                    if (S_ISREG(file_stat.st_mode))
                    {
                        printf("%s\n", files[i]);
                    }
                    else if (S_ISDIR(file_stat.st_mode))
                    {
                        printf("\033[1;34m%s\033[0m\n", files[i]);
                    }
                    else
                    {
                        printf("\033[1;32m%s\033[0m\n", files[i]);
                    }
                            
                }
                else
                {
                    perror("stat");
                }
                
                free(files[i]);
            }
            printf("%d\n", num_of_files);

            free(files);
        }

// If not NULL, then either -l, -a, -la, -al, or a directory name.
        
        if (token != NULL)
        {
            if (strcmp(token, "-l") == 0)
            {
                token = strtok(NULL, " \n\t\r");

// Second argument is -l and third argument is NULL.

                if (token == NULL)
                {
                    DIR *d;
                    struct dirent *file;
                    d = opendir(cwd);
                    int num_of_files = 0;
                    if (d)
                    {
                        while ((file = readdir(d)) != NULL)
                        {
                            num_of_files++;
                        }
                    }

                    rewinddir(d);

                    char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                    for (int i = 0; i < num_of_files; i++)
                    {
                        files[i] = (char *)malloc(1024 * sizeof(char));
                    }
                    int file_index = 0;
                    while ((file = readdir(d)) != NULL)
                    {
                        strcpy(files[file_index++], file->d_name);
                    }
                    closedir(d);

                    qsort(files, num_of_files, sizeof(char *), compare);

                    // I should print all the details of the files as in the ls -l command

                    for (int i = 0; i < num_of_files; i++)
                    {
                        if(files[i][0] == '.')
                        {
                            continue;
                        }
                        struct stat file_stat;
                        char file_path[1024];
                        int folder_path_len = strlen(cwd);
                        strcpy(file_path, cwd);
                        if (cwd[folder_path_len - 1] != '/')
                        {
                            strcat(file_path, "/");
                        }
                        strcat(file_path, files[i]);
                        if (stat(file_path, &file_stat) == 0)
                        {

                            print_permissions(file_stat.st_mode);
                            printf(" %ld ", file_stat.st_nlink);
                            struct passwd *owner_info = getpwuid(file_stat.st_uid);
                            if (owner_info != NULL)
                            {
                                printf(" %s ", owner_info->pw_name);
                                printf(" %s ", owner_info->pw_name);
                            }
                            else
                            {
                                perror("getpwuid");
                            }
                            printf(" %ld ", file_stat.st_blksize);
                            struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                            char mod_time_str[20];
                            strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                            int start = 0;
                            int length = 12;

                            char *substring = (char *)malloc((length + 1) * sizeof(char));
                            if (substring != NULL)
                            {
                                strncpy(substring, mod_time_str + start, length);
                                substring[length] = '\0';

                                printf("%s ", substring);

                                // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                                if (S_ISREG(file_stat.st_mode))
                                {
                                    printf("%s\n", files[i]);
                                }
                                else if (S_ISDIR(file_stat.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", files[i]);
                                }
                                else if (S_IXUSR & file_stat.st_mode)
                                {
                                    printf("\033[1;32m%s\033[0m\n", files[i]);
                                }

                                free(substring);
                            }
                            else
                            {
                                perror("malloc");
                            }
                        }
                        else
                        {
                            perror("localtime");
                        }
                    }
                }

// Second argument is -l and third argument is -a.

                else if (strcmp(token, "-a") == 0)
                {
                    // print all the files in the directory specified by the token
                    token = strtok(NULL, " \n\t\r");

                    DIR *d;
                    struct dirent *file;
                    if (token != NULL)
                    {
                        d = opendir(token);
                        int num_of_files = 0;
                        if (d)
                        {
                            printf("Directory exists\n");
                            while ((file = readdir(d)) != NULL)
                            {
                                num_of_files++;
                            }
                        }

                        rewinddir(d);

                        char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                        for (int i = 0; i < num_of_files; i++)
                        {
                            files[i] = (char *)malloc(1024 * sizeof(char));
                        }
                        int file_index = 0;
                        while ((file = readdir(d)) != NULL)
                        {
                            strcpy(files[file_index++], file->d_name);
                        }
                        closedir(d);

                        qsort(files, num_of_files, sizeof(char *), compare);

                        // I should print all the details of the files as in the ls -l command
                        for (int i = 0; i < num_of_files; i++)
                        {
                            struct stat file_stat;
                            char file_path[1024];
                            int folder_path_len = strlen(token);
                            strcpy(file_path, token);
                            if (token[folder_path_len - 1] != '/')
                            {
                                strcat(file_path, "/");
                            }
                            strcat(file_path, files[i]);

                            if (stat(file_path, &file_stat) == 0)
                            {

                                print_permissions(file_stat.st_mode);
                                printf(" %ld ", file_stat.st_nlink);
                                struct passwd *owner_info = getpwuid(file_stat.st_uid);
                                if (owner_info != NULL)
                                {
                                    printf(" %s ", owner_info->pw_name);
                                    printf(" %s ", owner_info->pw_name);
                                }
                                else
                                {
                                    perror("getpwuid");
                                }
                                printf(" %ld ", file_stat.st_blksize);

                                struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                                char mod_time_str[20];
                                strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                                int start = 0;
                                int length = 12;

                                char *substring = (char *)malloc((length + 1) * sizeof(char));
                                if (substring != NULL)
                                {
                                    strncpy(substring, mod_time_str + start, length);
                                    substring[length] = '\0';

                                    printf("%s %s\n", substring, files[i]);

                                    free(substring);
                                }
                                else
                                {
                                    perror("malloc");
                                }
                            }
                            else
                            {
                                perror("localtime");
                            }
                        }
                        free(files);
                    }

                    else
                    {
                        d = opendir(temp_cwd);
                        // print all the details of the current directory
                        int num_of_files = 0;
                        if (d)
                        {
                            while ((file = readdir(d)) != NULL)
                            {
                                num_of_files++;
                            }
                        }

                        rewinddir(d);

                        char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                        for (int i = 0; i < num_of_files; i++)
                        {
                            files[i] = (char *)malloc(1024 * sizeof(char));
                        }

                        int file_index = 0;
                        while ((file = readdir(d)) != NULL)
                        {
                            strcpy(files[file_index++], file->d_name);
                        }
                        closedir(d);

                        qsort(files, num_of_files, sizeof(char *), compare);

                        // I should print all the details of the files as in the ls -l command

                        for (int i = 0; i < num_of_files; i++)
                        {
                            struct stat file_stat;
                            char file_path[1024];
                            int folder_path_len = strlen(temp_cwd);
                            strcpy(file_path, temp_cwd);
                            if (temp_cwd[folder_path_len - 1] != '/')
                            {
                                strcat(file_path, "/");
                            }
                            strcat(file_path, files[i]);
                            if (stat(file_path, &file_stat) == 0)
                            {

                                print_permissions(file_stat.st_mode);
                                printf(" %ld ", file_stat.st_nlink);
                                struct passwd *owner_info = getpwuid(file_stat.st_uid);
                                if (owner_info != NULL)
                                {
                                    printf(" %s ", owner_info->pw_name);
                                    printf(" %s ", owner_info->pw_name);
                                }
                                else
                                {
                                    perror("getpwuid");
                                }
                                printf(" %ld ", file_stat.st_blksize);
                                struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                                char mod_time_str[20];
                                strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                                int start = 0;
                                int length = 12;

                                char *substring = (char *)malloc((length + 1) * sizeof(char));
                                if (substring != NULL)
                                {
                                    strncpy(substring, mod_time_str + start, length);
                                    substring[length] = '\0';

                                    printf("%s ", substring );

                                    // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                                    if (S_ISREG(file_stat.st_mode))
                                    {
                                        printf("%s\n", files[i]);
                                    }
                                    else if (S_ISDIR(file_stat.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", files[i]);
                                    }
                                    else if (S_IXUSR & file_stat.st_mode)
                                    {
                                        printf("\033[1;32m%s\033[0m\n", files[i]);
                                    }

                                    free(substring);
                                }
                                else
                                {
                                    perror("malloc");
                                }
                            }
                            else
                            {
                                perror("localtime");
                            }
                        }
                        free(files);
                    }
                }

// Second argument is -l and third argument is a directory.
                else
                {
                    // print all the files in the directory specified by the token
                    DIR *d;
                    struct dirent *file;
                    d = opendir(token);
                    int num_of_files = 0;
                    if (d)
                    {
                        while ((file = readdir(d)) != NULL)
                        {
                            num_of_files++;
                        }
                    }

                    rewinddir(d);

                    char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                    for (int i = 0; i < num_of_files; i++)
                    {
                        files[i] = (char *)malloc(1024 * sizeof(char));
                    }
                    int file_index = 0;
                    while ((file = readdir(d)) != NULL)
                    {
                        strcpy(files[file_index++], file->d_name);
                    }
                    closedir(d);

                    qsort(files, num_of_files, sizeof(char *), compare);

                    // I should print all the details of the files as in the ls -l command
                    for (int i = 0; i < num_of_files; i++)
                    {
                        struct stat file_stat;
                        char file_path[1024];
                        int folder_path_len = strlen(token);
                        strcpy(file_path, token);
                        if (token[folder_path_len - 1] != '/')
                        {
                            strcat(file_path, "/");
                        }
                        strcat(file_path, files[i]);

                        // if the file name starts with a dot, then it is a hidden file and it should not be printed
                        if (files[i][0] == '.')
                        {
                            continue;
                        }

                        if (stat(file_path, &file_stat) == 0)
                        {

                            print_permissions(file_stat.st_mode);
                            printf(" %ld ", file_stat.st_nlink);
                            struct passwd *owner_info = getpwuid(file_stat.st_uid);
                            if (owner_info != NULL)
                            {
                                printf(" %s ", owner_info->pw_name);
                                printf(" %s ", owner_info->pw_name);
                            }
                            else
                            {
                                perror("getpwuid");
                            }
                            printf(" %ld ", file_stat.st_blksize);

                            struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                            char mod_time_str[20];
                            strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                            int start = 0;
                            int length = 12;

                            char *substring = (char *)malloc((length + 1) * sizeof(char));
                            if (substring != NULL)
                            {
                                strncpy(substring, mod_time_str + start, length);
                                substring[length] = '\0';

                                printf("%s %s\n", substring, files[i]);

                                free(substring);
                            }
                            else
                            {
                                perror("malloc");
                            }
                        }
                        else
                        {
                            perror("localtime");
                        }
                    }
                    free(files);
                }
            }

            /*
            Finished handling -l command completely! Starting off -a
            */

            else if (strcmp(token, "-a") == 0)
            {
                token = strtok(NULL, " \n\t\r");
// Second argument -a third argument is NULL.
                if (token == NULL)
                {
                    DIR *d;
                    struct dirent *file;
                    d = opendir(cwd);
                    int num_of_files = 0;
                    if (d)
                    {
                        while ((file = readdir(d)) != NULL)
                        {
                            num_of_files++;
                        }
                    }

                    rewinddir(d);

                    char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                    for (int i = 0; i < num_of_files; i++)
                    {
                        files[i] = (char *)malloc(1024 * sizeof(char));
                    }
                    int file_index = 0;
                    while ((file = readdir(d)) != NULL)
                    {
                        strcpy(files[file_index++], file->d_name);
                    }
                    closedir(d);

                    qsort(files, num_of_files, sizeof(char *), compare);

                    // I should print all the details of the files as in the ls -l command
                    for (int i = 0; i < num_of_files; i++)
                    {
                        // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                        struct stat file_stat;
                        char file_path[1024];
                        int folder_path_len = strlen(cwd);
                        strcpy(file_path, cwd);
                        if (cwd[folder_path_len - 1] != '/')
                        {
                            strcat(file_path, "/");
                        }
                        strcat(file_path, files[i]);

                        if (stat(file_path, &file_stat) == 0)
                        {
                            if (S_ISREG(file_stat.st_mode))
                            {
                                printf("%s\n", files[i]);
                            }
                            else if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("\033[1;34m%s\033[0m\n", files[i]);
                            }
                            else if (S_IXUSR & file_stat.st_mode)
                            {
                                printf("\033[1;32m%s\033[0m\n", files[i]);
                            }
                        }
                        else
                        {
                            perror("stat");
                        }
                        free(files[i]);
                    }
                    printf("%d\n", num_of_files);

                    free(files);
                }
// Second argument is -a and third argument is -l.
                else if (strcmp(token, "-l") == 0)
                {

                    // print all the files in the directory specified by the token
                    token = strtok(NULL, " \n\t\r");
                    if (token != NULL)
                    {
                        DIR *d;
                        struct dirent *file;
                        d = opendir(token);
                        int num_of_files = 0;
                        if (d)
                        {
                            while ((file = readdir(d)) != NULL)
                            {
                                num_of_files++;
                            }
                        }

                        rewinddir(d);

                        char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                        for (int i = 0; i < num_of_files; i++)
                        {
                            files[i] = (char *)malloc(1024 * sizeof(char));
                        }
                        int file_index = 0;
                        while ((file = readdir(d)) != NULL)
                        {
                            strcpy(files[file_index++], file->d_name);
                        }
                        closedir(d);

                        qsort(files, num_of_files, sizeof(char *), compare);

                        // I should print all the details of the files as in the ls -l command
                        for (int i = 0; i < num_of_files; i++)
                        {
                            struct stat file_stat;
                            char file_path[1024];
                            int folder_path_len = strlen(token);
                            strcpy(file_path, token);
                            if (token[folder_path_len - 1] != '/')
                            {
                                strcat(file_path, "/");
                            }
                            strcat(file_path, files[i]);
                            if (stat(file_path, &file_stat) == 0)
                            {

                                print_permissions(file_stat.st_mode);
                                printf(" %ld ", file_stat.st_nlink);
                                struct passwd *owner_info = getpwuid(file_stat.st_uid);
                                if (owner_info != NULL)
                                {
                                    printf(" %s ", owner_info->pw_name);
                                    printf(" %s ", owner_info->pw_name);
                                }
                                else
                                {
                                    perror("getpwuid");
                                }
                                printf(" %ld ", file_stat.st_blksize);

                                struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                                char mod_time_str[20];
                                strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                                int start = 0;
                                int length = 12;

                                char *substring = (char *)malloc((length + 1) * sizeof(char));
                                if (substring != NULL)
                                {
                                    strncpy(substring, mod_time_str + start, length);
                                    substring[length] = '\0';

                                    printf("%s ", substring);
                                    //
                                    // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                                    if (S_ISREG(file_stat.st_mode))
                                    {
                                        printf("%s\n", files[i]);
                                    }
                                    else if (S_ISDIR(file_stat.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", files[i]);
                                    }
                                    else if (S_IXUSR & file_stat.st_mode)
                                    {
                                        printf("\033[1;32m%s\033[0m\n", files[i]);
                                    }

                                    free(substring);
                                }
                                else
                                {
                                    perror("malloc");
                                }
                            }
                            else
                            {
                                perror("localtime");
                            }
                        }
                        free(files);
                    }
// Second argument is -l and third argument is directory.
                    else
                    {
                        DIR *d;
                        struct dirent *file;
                        d = opendir(temp_cwd);
                        // print all the details of the current directory
                        int num_of_files = 0;
                        if (d)
                        {
                            while ((file = readdir(d)) != NULL)
                            {
                                num_of_files++;
                            }
                        }

                        rewinddir(d);

                        char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                        for (int i = 0; i < num_of_files; i++)
                        {
                            files[i] = (char *)malloc(1024 * sizeof(char));
                        }

                        int file_index = 0;
                        while ((file = readdir(d)) != NULL)
                        {
                            strcpy(files[file_index++], file->d_name);
                        }
                        closedir(d);

                        qsort(files, num_of_files, sizeof(char *), compare);

                        // I should print all the details of the files as in the ls -l command

                        for (int i = 0; i < num_of_files; i++)
                        {
                            struct stat file_stat;
                            char file_path[1024];
                            int folder_path_len = strlen(temp_cwd);
                            strcpy(file_path, temp_cwd);
                            if (temp_cwd[folder_path_len - 1] != '/')
                            {
                                strcat(file_path, "/");
                            }
                            strcat(file_path, files[i]);
                            if (stat(file_path, &file_stat) == 0)
                            {

                                print_permissions(file_stat.st_mode);
                                printf(" %ld ", file_stat.st_nlink);
                                struct passwd *owner_info = getpwuid(file_stat.st_uid);
                                if (owner_info != NULL)
                                {
                                    printf(" %s ", owner_info->pw_name);
                                    printf(" %s ", owner_info->pw_name);
                                }
                                else
                                {
                                    perror("getpwuid");
                                }
                                printf(" %ld ", file_stat.st_blksize);
                                struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                                char mod_time_str[20];
                                strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                                int start = 0;
                                int length = 12;

                                char *substring = (char *)malloc((length + 1) * sizeof(char));
                                if (substring != NULL)
                                {
                                    strncpy(substring, mod_time_str + start, length);
                                    substring[length] = '\0';

                                    printf("%s ", substring);
                                    // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                                    if (S_ISREG(file_stat.st_mode))
                                    {
                                        printf("%s\n", files[i]);
                                    }
                                    else if (S_ISDIR(file_stat.st_mode))
                                    {
                                        printf("\033[1;34m%s\033[0m\n", files[i]);
                                    }
                                    else if (S_IXUSR & file_stat.st_mode)
                                    {
                                        printf("\033[1;32m%s\033[0m\n", files[i]);
                                    }

                                    free(substring);
                                }
                                else
                                {
                                    perror("malloc");
                                }
                            }
                            else
                            {
                                perror("localtime");
                            }
                        }
                        free(files);
                    }
                }

                else
                {
                    // print all the files in the directory specified by the token
                    DIR *d;
                    struct dirent *file;
                    d = opendir(token);
                    int num_of_files = 0;
                    if (d)
                    {
                        while ((file = readdir(d)) != NULL)
                        {
                            num_of_files++;
                        }
                    }

                    rewinddir(d);

                    char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                    for (int i = 0; i < num_of_files; i++)
                    {
                        files[i] = (char *)malloc(1024 * sizeof(char));
                    }
                    int file_index = 0;
                    while ((file = readdir(d)) != NULL)
                    {
                        strcpy(files[file_index++], file->d_name);
                    }
                    closedir(d);

                    qsort(files, num_of_files, sizeof(char *), compare);

                    // I should print all the details of the files as in the ls -l command
                    for (int i = 0; i < num_of_files; i++)
                    {
                        // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                        struct stat file_stat;
                        char file_path[1024];
                        int folder_path_len = strlen(token);
                        strcpy(file_path, token);
                        if (token[folder_path_len - 1] != '/')
                        {
                            strcat(file_path, "/");
                        }
                        strcat(file_path, files[i]);

                        if (stat(file_path, &file_stat) == 0)
                        {
                            if (S_ISREG(file_stat.st_mode))
                            {
                                printf("%s\n", files[i]);
                            }
                            else if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("\033[1;34m%s\033[0m\n", files[i]);
                            }
                            else if (S_IXUSR & file_stat.st_mode)
                            {
                                printf("\033[1;32m%s\033[0m\n", files[i]);
                            }
                        }
                        else
                        {
                            perror("stat");
                        }
                        free(files[i]);
                    }
                }
            }

            else if ((strcmp(token, "-la") == 0) || (strcmp(token, "-al") == 0))
            {
                token = strtok(NULL, " \n\t\r");
                if (token == NULL)
                {
                    DIR *d;
                    struct dirent *file;
                    d = opendir(temp_cwd);
                    // print all the details of the current directory
                    int num_of_files = 0;
                    if (d)
                    {
                        while ((file = readdir(d)) != NULL)
                        {
                            num_of_files++;
                        }
                    }

                    rewinddir(d);

                    char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                    for (int i = 0; i < num_of_files; i++)
                    {
                        files[i] = (char *)malloc(1024 * sizeof(char));
                    }

                    int file_index = 0;
                    while ((file = readdir(d)) != NULL)
                    {
                        strcpy(files[file_index++], file->d_name);
                    }
                    closedir(d);

                    qsort(files, num_of_files, sizeof(char *), compare);

                    // I should print all the details of the files as in the ls -l command

                    for (int i = 0; i < num_of_files; i++)
                    {
                        struct stat file_stat;
                        char file_path[1024];
                        int folder_path_len = strlen(temp_cwd);
                        strcpy(file_path, temp_cwd);
                        if (temp_cwd[folder_path_len - 1] != '/')
                        {
                            strcat(file_path, "/");
                        }
                        strcat(file_path, files[i]);
                        if (stat(file_path, &file_stat) == 0)
                        {

                            print_permissions(file_stat.st_mode);
                            printf(" %ld ", file_stat.st_nlink);
                            struct passwd *owner_info = getpwuid(file_stat.st_uid);
                            if (owner_info != NULL)
                            {
                                printf(" %s ", owner_info->pw_name);
                                printf(" %s ", owner_info->pw_name);
                            }
                            else
                            {
                                perror("getpwuid");
                            }
                            printf(" %ld ", file_stat.st_blksize);
                            struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                            char mod_time_str[20];
                            strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                            int start = 0;
                            int length = 12;

                            char *substring = (char *)malloc((length + 1) * sizeof(char));
                            if (substring != NULL)
                            {
                                strncpy(substring, mod_time_str + start, length);
                                substring[length] = '\0';

                                printf("%s ", substring);
                                // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                                if (S_ISREG(file_stat.st_mode))
                                {
                                    printf("%s\n", files[i]);
                                }
                                else if (S_ISDIR(file_stat.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", files[i]);
                                }
                                else if (S_IXUSR & file_stat.st_mode)
                                {
                                    printf("\033[1;32m%s\033[0m\n", files[i]);
                                }

                                free(substring);
                            }
                            else
                            {
                                perror("malloc");
                            }
                        }
                        else
                        {
                            perror("localtime");
                        }
                    }
                }

                else if (token != NULL)
                {
                    DIR *d;
                    struct dirent *file;
                    d = opendir(token);
                    int num_of_files = 0;
                    if (d)
                    {
                        printf("Directory exists\n");
                        while ((file = readdir(d)) != NULL)
                        {
                            num_of_files++;
                        }
                    }

                    rewinddir(d);

                    char **files = (char **)malloc(num_of_files * sizeof(char *)); // basically to store the list of pointers of the names of the files

                    for (int i = 0; i < num_of_files; i++)
                    {
                        files[i] = (char *)malloc(1024 * sizeof(char));
                    }
                    int file_index = 0;
                    while ((file = readdir(d)) != NULL)
                    {
                        strcpy(files[file_index++], file->d_name);
                    }
                    closedir(d);

                    qsort(files, num_of_files, sizeof(char *), compare);

                    // I should print all the details of the files as in the ls -l command
                    for (int i = 0; i < num_of_files; i++)
                    {
                        struct stat file_stat;
                        char file_path[1024];
                        int folder_path_len = strlen(token);
                        strcpy(file_path, token);
                        if (token[folder_path_len - 1] != '/')
                        {
                            strcat(file_path, "/");
                        }
                        strcat(file_path, files[i]);

                        if (stat(file_path, &file_stat) == 0)
                        {

                            print_permissions(file_stat.st_mode);
                            printf(" %ld ", file_stat.st_nlink);
                            struct passwd *owner_info = getpwuid(file_stat.st_uid);
                            if (owner_info != NULL)
                            {
                                printf(" %s ", owner_info->pw_name);
                                printf(" %s ", owner_info->pw_name);
                            }
                            else
                            {
                                perror("getpwuid");
                            }
                            printf(" %ld ", file_stat.st_blksize);

                            struct tm *mod_time_info = localtime(&file_stat.st_mtime);
                            char mod_time_str[20];
                            strftime(mod_time_str, sizeof(mod_time_str), "%b %d %H:%M", mod_time_info);

                            int start = 0;
                            int length = 12;

                            char *substring = (char *)malloc((length + 1) * sizeof(char));
                            if (substring != NULL)
                            {
                                strncpy(substring, mod_time_str + start, length);
                                substring[length] = '\0';

                                printf("%s ", substring);
                                // if it is a regular file, print in white, if it s a directory, print in blue, if it is an excutable file, print in green
                                if (S_ISREG(file_stat.st_mode))
                                {
                                    printf("%s\n", files[i]);
                                }
                                else if (S_ISDIR(file_stat.st_mode))
                                {
                                    printf("\033[1;34m%s\033[0m\n", files[i]);
                                }
                                else if (S_IXUSR & file_stat.st_mode)
                                {
                                    printf("\033[1;32m%s\033[0m\n", files[i]);
                                }

                                free(substring);
                            }
                            else
                            {
                                perror("malloc");
                            }
                        }
                        else
                        {
                            perror("localtime");
                        }
                    }
                    free(files);
                }
            }
        }
    }
}