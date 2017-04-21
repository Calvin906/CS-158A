#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/resource.h>
#include <strings.h>
#include <string.h>

#define max(x,y) (x>y)?x:y

//#define portno 1234

void main(int argc,char *argv[])
{
  int sds,sock,newsd,n,addrlen,portno,pUsage=10,i,sockmax=0;
  long sum=0;
  char buffer[256] = {0}, pUsageChr[10]={0};
  struct sockaddr_in server_addr1,client_addr1,server_addr2,client_addr2,from;
  struct rusage usage;

  fd_set set;
  struct timeval timeout; 


  /*************************Create a Socket****************************/
  sds = socket(AF_INET,SOCK_STREAM,0) ;
  printf("socket value1 = %d\n",sds);
  if(sds<0)
  {
    perror("failed to open a socket\n");
    exit(1);
  }

  /********************Server Address Structure for bind***************/
  //bzero();
  portno = atoi(argv[1]);
  printf("port = %d\n",portno);
  server_addr1.sin_family = AF_INET;
  server_addr1.sin_port = htons(portno);
  server_addr1.sin_addr.s_addr = INADDR_ANY;

  /*********************binding ipaddr and portno***********************/
  if(bind(sds,(struct sockaddr *)&server_addr1,sizeof(server_addr1)) < 0) 
  { 
    perror("Failed to bind\n");
    exit(1);
  }
  
  /**********************Server in Listening mode***********************/
  listen(sds,5);

/****************************UDP server***********************************/

/*************************Create a Socket****************************/
  sock = socket(AF_INET,SOCK_DGRAM,0) ;
  printf("socket value2 = %d\n",sock);
  if(sock<0)
  {
    perror("failed to open a socket\n");
    exit(1);
  }

  /********************Server Address Structure for bind***************/
  //bzero();
  portno = atoi(argv[1]);
  printf("port = %d\n",portno);
  server_addr2.sin_family = AF_INET;
  server_addr2.sin_port = htons(portno);
  server_addr2.sin_addr.s_addr = INADDR_ANY;

  /********************binding ipaddr and portno***********************/
  if(bind(sock,(struct sockaddr *)&server_addr2,sizeof(server_addr2))< 0) 
  { 
    perror("Failed to bind\n");
    exit(1);
  }

  FD_ZERO(&set);
  //FD_SET(sock,&set);
  //FD_SET(sds,&set);

  sockmax = (max(sock,sds)) + 1;
  printf("sockmax = %d\n",sockmax);

  while(1)
  {
   FD_SET(sock,&set);
   FD_SET(sds,&set);
   if(select(sockmax,&set,NULL,NULL,NULL)<0)
   {
    perror("Select failed");
    exit(1);
   }
   
    for(i=0;i<=sockmax;i++)
    {

     /***********getrusage************************/
      pUsage  = getrusage(RUSAGE_SELF,&usage);
      sprintf(pUsageChr,"\nCPU time used:%ld\nMax Resident size:%ld\nPage reclaims:%ld\nPage faults:%ld\nBlock ip ops:%ld\nBlock op ops:%ld\nIPC msgs sent:%ld\nIPC msgs rcvd:%ld\nSig rcvd:%ld\nVoluntary context swtch:%ld\nInvoluntary context swtch:%ld\n",usage.ru_utime.tv_usec,usage.ru_maxrss,usage.ru_minflt,usage.ru_majflt,usage.ru_inblock,usage.ru_oublock,usage.ru_msgsnd,usage.ru_msgrcv,usage.ru_nsignals,usage.ru_nvcsw,usage.ru_nivcsw);

      //printf("process usage = %s\n",pUsageChr);    

    if(FD_ISSET(i,&set))
    {
     if(i==sds)
     {
      //printf("cnt = %d\n",cnt);
      /*******************accept from client********************************/
      addrlen = sizeof(client_addr2);
      newsd = accept(sds,(struct sockaddr *)&client_addr2,&addrlen);
      if(newsd < 0) 
      {
        perror("Failed to accept from client\n");
        exit(1);
      }
 
 
      /******************Reading data from client***************************/ 
      bzero(buffer,256);
       
      n = read(newsd,buffer,255);
      printf("message = %s\n",buffer);
      if(n<0)
        printf("Error in reading data\n");
      
      n = write(newsd,buffer,strlen(buffer));
      if(n<0)
        printf("Error in writing data\n"); 
       
      

      /******************Writing data to client*****************************/ 
      bzero(buffer,256); 
      n = read(newsd,buffer,256);
      printf("message = %s\n",buffer);
      if(n<0)
        printf("Error in reading data\n");
      
      bzero(buffer,256);
      n = write(newsd,pUsageChr,strlen(pUsageChr)+1);
      if(n<0)
        printf("Error in writing data\n"); 

      FD_CLR(sds,&set);
 
     }else if(i==sock)
      {
        bzero(buffer,256);
        /*******************Recv from client*********************************/
        addrlen = sizeof(struct sockaddr_in);
        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from,&addrlen);
        if(n<0)
          perror("Failed to read\n");
        write(1,"\nRecvd datagram: ",17);
        write(1,buffer,n);  
        
        bzero(buffer,256);
        n = sendto(sock,buffer,strlen(buffer)+1,0,(struct sockaddr *)&from ,addrlen);
        if(n<0)
        perror("Failed to send\n");

        bzero(buffer,256);
        /*******************Send to client*********************************/
        n = recvfrom(sock,buffer,256,0,(struct sockaddr *)&from,&addrlen);
        if(n<0)
          perror("Failed to read\n");
        write(1,"\nRecvd datagram: ",17);
        write(1,buffer,n); 
      
        bzero(buffer,256);
        n = sendto(sock,pUsageChr,strlen(pUsageChr)+1,0,(struct sockaddr *)&from ,addrlen);
        if(n<0)
        perror("Failed to send\n");  
 
        FD_CLR(sock,&set); 
      }
   }
   }
   

  }//while end

}//main end
 
