#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // close()
#include <stdbool.h>
#include <ctype.h> // Include for tolower() and isalpha()

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

typedef struct
{
    int characters;
    int vowels;
    int consonants;
} AnalysisResult;

bool is_vowel(char c)
{
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

AnalysisResult analyze_string(const char *str)
{
    AnalysisResult result = {0, 0, 0};

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]))
        {
            result.characters++;
            if (is_vowel(str[i]))
                result.vowels++;
            else
                result.consonants++;
        }
    }

    return result;
}

void func(int sockfd, struct sockaddr_in cliaddr)
{
    int n, len;
    char buff[MAX];

    for (;;)
    {
        bzero(buff, MAX);
        len = sizeof(cliaddr);

        // Receive the string from client
        n = recvfrom(sockfd, (char *)buff, MAX,
                     MSG_WAITALL, (SA *)&cliaddr,
                     &len);
        buff[n] = '\0';

        printf("Received string from client: %s\n", buff);

        // Analyze the string
        AnalysisResult result = analyze_string(buff);

        // Convert the result to string
        snprintf(buff, sizeof(buff), "Characters: %d, Vowels: %d, Consonants: %d", result.characters, result.vowels, result.consonants);

        // Send the result back to the client
        sendto(sockfd, (const char *)buff, strlen(buff),
               MSG_CONFIRM, (const SA *)&cliaddr, len);
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

