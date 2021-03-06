#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>

//#define portno 1234

void main(int argc,char *argv[])
{
  int sock,newsock,n,addrlen;
  struct sockaddr_in server_addr,client_addr;
  char buffer[256] = {0};
  struct hostent *host; //This data type is used to represent an entry in the hosts database
  
  sock = socket(AF_INET,SOCK_STREAM,0) ; //create a socket
  if(sock<0)
  {
    perror("failed to open a socket\n");
    exit(1);
  }

  host = gethostbyname(argv[1]);
  
  server_addr.sin_family = AF_INET;     
  server_addr.sin_port = htons(atoi(argv[2]));   
  server_addr.sin_addr = *((struct in_addr *)host->h_addr);
  //bcopy((char *)host->h_addr,(char *)&server_addr.sin_addr.s_addr,host->h_length);

  if(connect(sock,(struct sockaddr *)(&server_addr),sizeof(server_addr))<0)
  {
    perror("failed to connect\n");
    exit(1);
  }
  while(1)
  {
      bzero(buffer,256);
      printf("Enter message TCP: ");
      fgets(buffer,256,stdin);
      n = write(sock,buffer,strlen(buffer));
      if(n<0)
        printf("Error in writing data\n");

      bzero(buffer,256);
      n = read(sock,buffer,256);
      printf("Server received: %s",buffer);
      if(n<0)
        printf("Error in reading data\n");

  }
  
  close(sock);
}
