#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // close()
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd, struct sockaddr_in cliaddr)
{
    char buff[MAX];
    int n, len;

    for (;;)
    {
        bzero(buff, MAX);
        len = sizeof(cliaddr);

        // Receive the message from client
        n = recvfrom(sockfd, (char *)buff, MAX,
                     MSG_WAITALL, (SA *)&cliaddr,
                     &len);
        buff[n] = '\0';
        printf("Client: %s\n", buff);

        // If message contains "exit" then server exits
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server exiting...\n");
            break;
        }

        bzero(buff, MAX);
        printf("Server: ");
        n = 0;

        // Read message from server's console
        while ((buff[n++] = getchar()) != '\n')
            ;

        // Send the message to client
        sendto(sockfd, (const char *)buff, strlen(buff),
               MSG_CONFIRM, (const SA *)&cliaddr, len);

        // If message contains "exit" then server exits
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server exiting...\n");
            break;
        }
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Socket creation failed...\n");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Binding failed...\n");
        exit(EXIT_FAILURE);
    }

    printf("Server listening...\n");

    // Function for communication
    func(sockfd, cliaddr);

    // Close the socket
    close(sockfd);
    return 0;
}

