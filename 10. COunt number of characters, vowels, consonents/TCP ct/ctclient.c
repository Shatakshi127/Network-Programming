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
    char sentence[1000];
    printf("Enter a sentence: ");
    getchar(); // Consume the newline character left in the buffer
    fgets(sentence, sizeof(sentence), stdin);

    // Send the sentence to the server
    write(sockfd, sentence, strlen(sentence) + 1);

    printf("Waiting for server to count characters, vowels, and consonants...\n");

    // Receive and print the counts from the server
    int charCount, vowelCount, consonantCount;
    read(sockfd, &charCount, sizeof(int));
    read(sockfd, &vowelCount, sizeof(int));
    read(sockfd, &consonantCount, sizeof(int));

    printf("Character count: %d\n", charCount);
    printf("Vowel count: %d\n", vowelCount);
    printf("Consonant count: %d\n", consonantCount);

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

    // function for counting characters, vowels, and consonants
    func(sockfd);

    // close the socket
    close(sockfd);

    return 0;
}

