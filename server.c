
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "utils.h"

#define PORT 8088

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;

    struct sockaddr_in address;

    int opt = 1;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};
    char *hello = "Hello from server";

    printf("Starting server on port %d\n", PORT);


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket file descriptor creation failed.");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Successfully created a socket file descriptor %d.\n",
            server_fd);
    }


    // Forcefully attaching socket to the port 8080
    if (setsockopt(
            server_fd,
            SOL_SOCKET,
            SO_REUSEPORT, // SO_REUSEADDR not working
            &opt,
            sizeof(opt)))
    {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Successfully set socket options.\n");
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(
            server_fd,
            (struct sockaddr *)&address,
            sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Successfully bound socket fd to address & port.\n");
    }

    printf("Start listening in blocking mode.\n");
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // This needs to spawn a new thread
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    else
    {
        char time_buffer[1024];
        long long micros = 0;

        printf("Connection accepted, new socket created %d\n", new_socket);

        for (int i = 0;;i++)
        {
            char response[1024];
            bzero(response, sizeof(response));

            valread = read(new_socket, buffer, 1024);

            current_timestamp(time_buffer, 1024, &micros);

            if (valread == 0) { break; }



            strcpy(response, buffer);
            strcat(response, " to you too");
            response[1023] = '\0';



            printf("[%s] micros=%lld Req#%d Client Said: %s \n",
                time_buffer,
                micros,
                i,
                buffer);

            send(new_socket, response , strlen(response) , 0 );
        }

    }



    return 0;
}