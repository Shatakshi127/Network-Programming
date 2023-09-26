#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define SA struct sockaddr

void performOperation(int connfd, int num1, int num2, int choice)
{
    int result;

    switch (choice)
    {
    case 1:
        result = num1 + num2;
        break;
    case 2:
        result = num1 - num2;
        break;
    case 3:
        result = num1 * num2;
        break;
    case 4:
        if (num2 != 0)
            result = num1 / num2;
        else
            result = -1; // Indicate division by zero
        break;
    default:
        result = 0;
        break;
    }

    // Send the result to the client
    write(connfd, &result, sizeof(int));
}

void func(int connfd)
{
    int num1, num2, choice;

    // Receive numbers and choice from the client
    read(connfd, &num1, sizeof(int));
    read(connfd, &num2, sizeof(int));
    read(connfd, &choice, sizeof(int));

    printf("Received numbers: %d, %d\n", num1, num2);
    printf("Performing operation based on choice...\n");

    // Perform the requested operation and send the result back to the client
    performOperation(connfd, num1, num2, choice);

    printf("Server Exit...\n");
}

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Function for operations
    func(connfd);

    // After operations, close the socket
    close(sockfd);

    return 0;
}

