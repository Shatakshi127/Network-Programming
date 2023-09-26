//Server
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

int fibonacci(int n)
{
    if (n <= 1)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void generateFibonacci(int connfd, int number)
{
    int a = 0, b = 1;
    int nextFibonacci;

    // Send the first two numbers of the series
    write(connfd, &a, sizeof(int));
    write(connfd, &b, sizeof(int));

    // Generate and send the rest of the series
    for (int i = 2; i <= number; i++)
    {
        nextFibonacci = a + b;
        write(connfd, &nextFibonacci, sizeof(int));
        a = b;
        b = nextFibonacci;
    }
}

void func(int connfd)
{
    int number;
    read(connfd, &number, sizeof(int));

    printf("From client: %d\t To client : ", number);

    printf("Generating Fibonacci Series up to %d\n", number);

    // Generate and send the Fibonacci series to the client
    generateFibonacci(connfd, number);

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

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    close(sockfd);

    return 0;
}

