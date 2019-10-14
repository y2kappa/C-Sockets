#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "utils.h"

#define MAX 1024
#define PORT 8088
#define HOSTNAME "130.211.119.0"
// #define PORT 6123
#define SA struct sockaddr

void func(int sockfd, char time_buffer[])
{
    char buff[MAX];
    int n;


    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");

        n = 0;
        for (;;)
        {
            char chr = getchar();
            if (chr != '\n')
            {
                buff[n++] = chr;
            }
            else
            {
                break;
            }
        }

        write(sockfd, buff, sizeof(buff));

        // now we wait for the server's response
        bzero(buff, sizeof(buff));

        timenow(time_buffer, 1024);
        read(sockfd, buff, sizeof(buff));

        printf("[%s] From Server : %s\n", time_buffer, buff);

        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

void func2(int sockfd, char time_buffer[], int i)
{
    char buff[100];
    sprintf(buff, "hi there %d", i);

    // timenow(time_buffer, 1024);
    current_timestamp(time_buffer, 1024);
    write(sockfd, buff, sizeof(buff));

    // now we wait for the server's response
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));

    printf("[%s] Resp#%d Server Said : %s\n", time_buffer, i, buff);

}

int main()
{

    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully created.. fd: %d\n", sockfd);
        bzero(&servaddr, sizeof(servaddr));
        // bzero() function shall place n zero-valued bytes in the area pointed to by ptr
    }

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    // servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_addr.s_addr = inet_addr("130.211.119.0");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }

    char time_buffer[1024];


    for (int i=0; i < 5; i++)
    {
        func2(sockfd, time_buffer, i);
        // func(sockfd);
    }

    // close the socket
    close(sockfd);
}