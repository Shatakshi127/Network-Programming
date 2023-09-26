//TCP CHAT:CLient

#include <sys/types.h>

#include <sys/socket.h>

#include <netdb.h>

#include <stdio.h>

#include<string.h>

#include<arpa/inet.h>

#include<unistd.h>

#include<netinet/in.h>

int main(int argc,char **argv)

{

 int sockfd,n;

 char sendline[100];

 char recvline[100];

 struct sockaddr_in servaddr;

 sockfd=socket(AF_INET,SOCK_STREAM,0);

 bzero(&servaddr,sizeof(servaddr));

 servaddr.sin_family=AF_INET;

 servaddr.sin_port=htons(22000);

 servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

 //inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));

 connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

while(1){

 printf("Enter message:");

 fgets(sendline,sizeof(sendline),stdin); /*stdin = 0 , for standard input */

 send(sockfd,sendline,strlen(sendline),0);

 if(strcmp(sendline,"exit\n")==0){

 printf("Chat ended by client");

 break;

 }

 bzero(sendline,sizeof(sendline));

 recv(sockfd,recvline,sizeof(recvline),0);

 printf("Message send by server: %s",recvline);

 if(strcmp(recvline,"exit\n")==0){

 printf("Chat ended by server");

 break;

 }

}

close(sockfd);

return 0;

}
