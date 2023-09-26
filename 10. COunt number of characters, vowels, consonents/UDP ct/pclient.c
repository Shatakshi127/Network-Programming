#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h> // close()
#include <ctype.h>  // Include for tolower()

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

    // Input the string from user
    char buff[MAX];
    printf("Enter a string: ");
    getchar(); // Consume newline left by previous input
    fgets(buff, sizeof(buff), stdin);

    // Send the string to server
    sendto(sockfd, (const char *)buff, strlen(buff),
           MSG_CONFIRM, (const SA *)&servaddr,
           sizeof(servaddr));

    printf("String sent to server: %s", buff);

    // Receive the result from server
    bzero(buff, MAX);
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, (char *)buff, MAX,
                     MSG_WAITALL, (SA *)&servaddr,
                     &len);
    buff[n] = '\0';

    // Display the result
    printf("Analysis from server: %s\n", buff);

    // Close the socket
    close(sockfd);
    return 0;
}

