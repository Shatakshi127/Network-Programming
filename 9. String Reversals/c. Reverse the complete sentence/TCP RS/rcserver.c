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

void reverseString(char *sentence)
{
    int length = strlen(sentence);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = sentence[i];
        sentence[i] = sentence[length - i - 1];
        sentence[length - i - 1] = temp;
    }
}

void func(int connfd)
{
    char sentence[1000];
    read(connfd, sentence, 1000);

    printf("Received sentence: %s\n", sentence);
    printf("Reversing the sentence...\n");

    // Reverse the whole sentence and send it back to the client
    reverseString(sentence);
    write(connfd, sentence, strlen(sentence) + 1);

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

    // Function for reversing the sentence
    func(connfd);

    // After operations, close the socket
    close(sockfd);

    return 0;
}

