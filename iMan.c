#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 4096

char response[MAX_BUFFER_SIZE];

void remove_html_tags(char *text)
{
    int in_tag = 0;
    char *src = text;
    char *dst = text;

    while (*src)
    {
        if (*src == '<')
        {
            in_tag = 1;
            src++;
            continue;
        }
        else if (*src == '>')
        {
            in_tag = 0;
            src++;
            continue;
        }

        if (!in_tag)
        {
            *dst = *src;
            dst++;
        }
        src++;
    }
    *dst = '\0';
}

void iMan(char *input, char *home)
{
    // Tokenize the input to get the command name
    char *token = strtok(input, " \t\n\r"); // this contains iMan
    token = strtok(NULL, " \t\n\r");        // this contains the command

    if (token == NULL)
    {
        printf("iMan: command not found\n");
        return;
    }

    /*
    1. Do DNS resolution for man.he.net
    2. Open a TCP socket to the IP address
    3. Send a GET request to the website’s server
    4. Read the body of the website
    5. Close socket
    */

    // Doing the DNS resolution using gethostbyname
    struct hostent *host;
    host = gethostbyname("man.he.net");
    if (host == NULL)
    {
        perror("Error in DNS resolution");
        exit(1);
    }

    // Step 2: Open a TCP socket to the IP address
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("Error connecting to the server");
        exit(1);
    }

    // Step 4: Construct the URL with the search term
    char request[4096];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", token);
    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("Error sending request");
        exit(1);
    }

    // Step 5: Read the response (the man page) into response buffer
    int bytes_received;
    while ((bytes_received = recv(sockfd, response, sizeof(response) - 1, 0)) > 0)
    {
        response[bytes_received] = '\0'; // Null-terminate the received data
    }

    if (bytes_received == -1)
    {
        perror("Error receiving response");
    }

    // Step 6: Remove HTML tags and print the response (text content)
    remove_html_tags(response);
    printf("%s\n", response);

    // Step 7: Close the socket
    close(sockfd);
}
