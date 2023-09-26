//Client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8080
#define MAX 80

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP
    servaddr.sin_port = htons(PORT);

   
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    }

    printf("Connected to the server...\n");

    char buffer[MAX];
    bzero(buffer, sizeof(buffer));

    
    read(sockfd, buffer, sizeof(buffer));
    printf("Server response: %s\n", buffer);

   
    close(sockfd);

    return 0;
}
