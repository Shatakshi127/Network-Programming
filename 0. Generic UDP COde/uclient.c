#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h> // close()
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd, struct sockaddr_in servaddr)
{
    char buff[MAX];
    int n;
    socklen_t len;

    for (;;)
    {
        printf("Client: ");
        bzero(buff, MAX);
        n = 0;

        // Read message from client's console
        while ((buff[n++] = getchar()) != '\n')
            ;

        // Send the message to server
        sendto(sockfd, (const char *)buff, strlen(buff),
               MSG_CONFIRM, (const SA *)&servaddr,
               sizeof(servaddr));

        // If message contains "exit" then client exits
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Client exiting...\n");
            break;
        }

        bzero(buff, MAX);
        len = sizeof(servaddr);

        // Receive the message from server
        n = recvfrom(sockfd, (char *)buff, MAX,
                     MSG_WAITALL, (SA *)&servaddr,
                     &len);
        buff[n] = '\0';
        printf("Server: %s\n", buff);

        // If message contains "exit" then client exits
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Client exiting...\n");
            break;
        }
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

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
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    // Function for communication
    func(sockfd, servaddr);

    // Close the socket
    close(sockfd);
    return 0;
}

