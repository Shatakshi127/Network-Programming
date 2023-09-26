#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>  // Include for tolower() and isalpha()

#define PORT 8080
#define SA struct sockaddr

int isVowel(char c)
{
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

void countCharacters(char *sentence, int *charCount, int *vowelCount, int *consonantCount)
{
    *charCount = strlen(sentence) - 1; // Excluding the newline character
    *vowelCount = 0;
    *consonantCount = 0;

    for (int i = 0; i < *charCount; i++)
    {
        if (isalpha(sentence[i]))
        {
            if (isVowel(sentence[i]))
                (*vowelCount)++;
            else
                (*consonantCount)++;
        }
    }
}

void func(int connfd)
{
    char sentence[1000];
    read(connfd, sentence, 1000);

    printf("Received sentence: %s\n", sentence);
    printf("Counting characters, vowels, and consonants...\n");

    // Count characters, vowels, and consonants and send the counts back to the client
    int charCount, vowelCount, consonantCount;
    countCharacters(sentence, &charCount, &vowelCount, &consonantCount);
    write(connfd, &charCount, sizeof(int));
    write(connfd, &vowelCount, sizeof(int));
    write(connfd, &consonantCount, sizeof(int));

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

    // Function for counting characters, vowels, and consonants
    func(connfd);

    // After operations, close the socket
    close(sockfd);

    return 0;
}

