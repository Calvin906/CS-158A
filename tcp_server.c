#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/resource.h>
#include <strings.h>

//#define portno 1234

void main(int argc,char *argv[])
{
  int sds,newsd,n,addrlen,portno,i;
  char buffer[256] = {0};
  struct sockaddr_in server_addr,client_addr; //using those structs from netinet/in.h

  /*************************Create a Socket****************************/
  //socket(int domain, int type, int protocol)
  sds = socket(AF_INET,SOCK_STREAM,0) ;
  //printing the socket value, mainly for debugging
  printf("socket value = %d\n",sds);
  //If socket value is less then 0, the socket could not be opened
  if(sds<0)
  {
    perror("failed to open a socket\n");
    exit(1);
  }

  /********************Server Address Structure for bind***************/
  //Taking port number from the commad line as an argument
  portno = atoi(argv[1]);
  //printing the port number, mainly for debugging
  printf("port = %d\n",portno);
  //setting the fields for the server address struct
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portno);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  /*********************binding ipaddr and portno***********************/
  if(bind(sds,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
  { 
    perror("Failed to bind\n");
    exit(1);
  }
  
  /**********************Server in Listening mode***********************/
  listen(sds,5);

  while(1)
  {
  /*******************accept from client********************************/
  addrlen = sizeof(client_addr);
  newsd = accept(sds,(struct sockaddr *)&client_addr,&addrlen);
  printf("newsd = %d\n",newsd);
  if(newsd < 0) 
  {
    perror("Failed to accept from client\n");
    exit(1);
  }
  
  /******************Reading data from client***************************/ 
  while(1){
      bzero(buffer,256);
  
      n = read(newsd,buffer,255);
      printf("message = %s\n",buffer);
      if(n<0)
        printf("Error in reading data\n");
    

  /******************Writing data to client*****************************/ 
  n = write(newsd,buffer,23);
  if(n<0)
    printf("Error in writing data\n");  
}
  
}
  /******************Close the Socket***********************************/
  close(sds);
}

