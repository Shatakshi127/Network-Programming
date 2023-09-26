//TCP CHAT:Server
#include <sys/types.h>

#include <sys/socket.h>

#include <netdb.h>

#include <stdio.h>

#include<string.h>

#include<unistd.h>

#include<arpa/inet.h>

#include<netinet/in.h>

#include<stdlib.h>

int main()

{

 char str[100];

 int listen_fd, comm_fd;

 struct sockaddr_in servaddr;

 listen_fd = socket(AF_INET, SOCK_STREAM, 0);

 if(listen_fd==-1){

 printf("Socket CReateion failed\n");

 exit(0);

 }

 else{

 printf("Socket Successfully created\n");

 }

 bzero( &servaddr, sizeof(servaddr));

 servaddr.sin_family = AF_INET;

 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

 servaddr.sin_port = htons(22000);

 int b=bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));// if same port number given to 2 different server then bind system gives error

 if(b!=0){

 printf("Socket bind failed\n");

 exit(0);

 }

 else{

 printf("Socket successfully binded\n");

 }

 listen(listen_fd, 10);

 char str2[100];

 comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

 while(1)

 {

 bzero( str, sizeof(str));

 recv(comm_fd,str,sizeof(str),0);

 if(strcmp(str,"exit\n")==0){

 printf("Chat ended by client\n");

 break;

 }

 printf("Message send by client: %s",str);

 printf("Enter message to send to client");

 fgets(str2,sizeof(str2),stdin);

 send(comm_fd,str2,strlen(str2),0);

 if(strcmp(str2,"exit\n")==0){

 printf("Chat ended by server\n");

 break;

 }

 }

 close(listen_fd);

 close(comm_fd);

 return 0;

}
