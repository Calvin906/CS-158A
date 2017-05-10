#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//#define portno 1234

void main(int argc,char *argv[])
{
  int sock,newsock,n,addrlen, seq=0, ackRevd=0, expectedAck;
  struct sockaddr_in server_addr,client_addr;
  char buffer[256] = {0};
  char frame[256] = {0};
  char frameStorage[8][350] = {0};
  struct hostent *host;
  
  sock = socket(AF_INET,SOCK_STREAM,0) ; //create a socket
//fcntl(sock, F_SETFL, O_NONBLOCK);
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
      sprintf(frame, "%d", seq);
      strcat(frame, buffer);
      printf("frame %s\n", frame);
      //system(buffer);
      memcpy(frameStorage[seq], frame,strlen(frame));
      n = write(sock,frame,strlen(frame));
      if(n<0)
        printf("Error in writing data\n");
      expectedAck = seq;
      seq++;
      bzero(buffer,256);
      n = read(sock,buffer,256);
      printf("%s",buffer);
      if(n<0)
        printf("Error in reading data\n");
      
      ackRevd = (int)buffer[0];
      printf("seq = %c\n", buffer[0]);	
      printf("ackRevd = %d\n", ackRevd - 48);

      
  }
  
  close(sock);
}
