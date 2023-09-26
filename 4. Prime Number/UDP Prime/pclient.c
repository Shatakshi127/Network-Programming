#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h> // close()
#include <stdbool.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

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

    // Input the number from user
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    // Send the number to server
    char buff[MAX];
    snprintf(buff, sizeof(buff), "%d", num);
    sendto(sockfd, (const char *)buff, strlen(buff),
           MSG_CONFIRM, (const SA *)&servaddr,
           sizeof(servaddr));

    printf("Number sent to server: %d\n", num);

    // Receive the result from server
    bzero(buff, MAX);
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, (char *)buff, MAX,
                     MSG_WAITALL, (SA *)&servaddr,
                     &len);
    buff[n] = '\0';

    // Display the result
    printf("Result from server: %s\n", buff);

    // Close the socket
    close(sockfd);
    return 0;
}

