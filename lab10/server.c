#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <time.h>

int main() {
      printf("Server running");
  int sockfd, newsockfd, portno, clilen,fd;
   char filename[10];
      char filedata[256];
      char filedatadump[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
    
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5005;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
  
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }      

   listen(sockfd,5);

   clilen = sizeof(cli_addr);

   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }

   char filenames[] = "a1, b2, c3";

   n = write(newsockfd,filenames,(char*)sizeof(filenames)-1);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
 
   bzero(filename,10);
   n = read( newsockfd,filename,9 );

   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   printf("Request Recv: filename %s\n",filename);

   FILE *ptr_file;
   ptr_file =fopen(filename,"rb");
   if (ptr_file){
       fscanf(ptr_file,"%[^\n]",filedata);
       bzero(filedata,10);
       getc(ptr_file);
       fclose(ptr_file);
   }
   strcat(filedata, filedatadump);
   

   n = write(newsockfd,filedata,(char*)sizeof(filedata)-1);
   
   if (n < 0) {
      perror("ERROR writing filedata");
      exit(1);
   }
        
   return 0;
}
