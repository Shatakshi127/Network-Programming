#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);

    // Send the number to the server
    write(sockfd, &number, sizeof(int));

    printf("Multiplication Table for %d:\n", number);

    // Receive and print the multiplication table from the server
    for (int i = 1; i <= 10; i++)
    {
        int result;
        read(sockfd, &result, sizeof(int));
        printf("%d x %d = %d\n", number, i, result);
    }
    printf("Client Exit...\n");
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

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
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for table
    func(sockfd);

    // close the socket
    close(sockfd);

    return 0;
}

