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
  int sds,newsd,n,addrlen,portno,pUsage=10,i;
  long sum=0;
  char buffer[256] = {0}, pUsageChr[10]={0};
  struct sockaddr_in server_addr,client_addr;
  struct rusage usage;

  /*************************Create a Socket****************************/
  sds = socket(AF_INET,SOCK_STREAM,0) ;
  printf("socket value = %d\n",sds);
  if(sds<0)
  {
    perror("failed to open a socket\n");
    exit(1);
  }

  /********************Server Address Structure for bind***************/
  //bzero();
  portno = atoi(argv[1]);
  printf("port = %d\n",portno);
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

  /***********getrusage************************
  //for(i=0;i<10000;i++) sum+=i;
  pUsage  = getrusage(RUSAGE_SELF,&usage);
  sprintf(pUsageChr,"%d",pUsage);
  printf("process usage = %d %s\n",pUsage,pUsageChr);/

  /******************Reading data from client***************************/
  while(1){
      bzero(buffer,256);
      char sub1[11] = {0};
      bzero(sub1, 11);

      n = read(newsd,buffer,255);
      if(n<0)
        printf("Error in reading data\n");

      memcpy( sub1, buffer, 10);
      if(strcmp(sub1, "Protocol 4") == 0){
        printf("You have entered %s\n", buffer);
      }
      else if(strcmp(sub1, "Protocol 5") == 0){
        printf("You have entered %s\n", buffer);
      }
      else if(strcmp(sub1, "Protocol 6") == 0){
        printf("You have entered %s\n", buffer);
      }
      else{
        printf("message = %s\n",buffer);
        // int help = strcmp(buffer, "Protocol 5");
        // printf("The result is: %d\n", help);
      }

  /******************Writing data to client*****************************/
  n = write(newsd,buffer,23);
  //n = write(newsd,pUsageChr,10);
  if(n<0)
    printf("Error in writing data\n");
}

}
  /******************Close the Socket***********************************/
  close(sds);
}