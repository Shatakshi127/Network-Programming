#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // close()
#include <stdbool.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

bool is_prime(int num)
{
    if (num <= 1)
        return false;

    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return false;
    }

    return true;
}

void func(int sockfd, struct sockaddr_in cliaddr)
{
    int n, len;
    char buff[MAX];

    for (;;)
    {
        bzero(buff, MAX);
        len = sizeof(cliaddr);

        // Receive the number from client
        n = recvfrom(sockfd, (char *)buff, MAX,
                     MSG_WAITALL, (SA *)&cliaddr,
                     &len);
        buff[n] = '\0';

        int num = atoi(buff);
        printf("Received number from client: %d\n", num);

        // Check if the number is prime
        bool isPrime = is_prime(num);

        // Convert the result to string
        snprintf(buff, sizeof(buff), "%s", isPrime ? "Prime" : "Not Prime");

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

