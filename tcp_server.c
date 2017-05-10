#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/resource.h>
#include <strings.h>
#include <time.h>

//#define portno 1234

void protocol_4(char buffer[], int n, char newsd[]);

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
        protocol_4(buffer, n, newsd);
      } else if(strcmp(sub1, "Protocol 5") == 0){
        printf("You have entered %s\n", buffer);
        n = write(newsd,buffer,23);
        if(n<0){
          printf("Error in writing data\n");
        }
        int isProtocol5 = 1;
        while(isProtocol5){
          //re-read from newsd so clear buffer
          bzero(buffer, 256);
          n = read(newsd,buffer,255);
          if(n<0){
            printf("Error in reading data\n");
          }

          //check if Exit was entered
          char sub2[5] = {0};
          memcpy( sub2, buffer, 4);
          if(strcmp(sub2, "Exit") == 0){
            isProtocol5 = 0;
          }
          else{
            //send ack
            n = write(newsd, "Ack for 5", 23);
            if(n<0){
              printf("Error in writing data\n");
            }

          }
          n = write(newsd, "Ack\n", 23);
          if(n<0)
            printf("Error in writing data\n");
        }
      } else{
        printf("message = %s\n",buffer);
        // int help = strcmp(buffer, "Protocol 5");
        // printf("The result is: %d\n", help);
        n = write(newsd,buffer,23);
        if(n<0)
          printf("Error in writing data\n");
      }

  /******************Writing data to client*****************************/

  //n = write(newsd,pUsageChr,10);



}

}
  /******************Close the Socket***********************************/
  close(sds);
}

void protocol_4(char buffer[], int n, char newsd[]){
  printf("You have entered %s\n", buffer);
        int isProtocol4 = 1;
        while(isProtocol4) {
          int bufLen = strlen(buffer);
          char frame[bufLen];
          memcpy(frame, buffer, 10);
          if ((frame[bufLen-1] == '0') == 0)
          {
            //printf("eee\n");
            buffer[bufLen-1] = '1';
            printf("message = %s \n",buffer);
            unsigned int retTime = time(0) + 3;
            while(time(0) < retTime);
            n = write(newsd, "Ack Received", 23);
          }else {
            n = write(newsd, buffer, 23);
            printf("message = %s \n",buffer);
          }
        
          if(n<0)
            printf("Error in writing data\n");
          char sub2[5] = {0};
          bzero(sub2, 5);
          memcpy( sub2, buffer, 4);
          if (strcmp(sub2, "Exit") == 0)
          {
            isProtocol4 = 0;
            n = write(newsd, "You have exited Protocol 4. \n", 33);
          if(n<0)
            printf("Error in writing data\n");
          printf("You have exited Protocol 4. \n");
            break;
          }

        bzero(buffer,256);
        char sub1[11] = {0};
        bzero(sub1, 11);
        n = read(newsd,buffer,255);
        if(n<0)
        printf("Error in reading data\n");

        }
}

