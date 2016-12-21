#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>




int main(int argc, char *argv[]) {
   int sockfd, portno, n, nfd;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char filenames[256];
   char filename[10];
   char filedata[256];

   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
   
   portno = atoi(argv[2]);;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	
   server = gethostbyname(argv[1]);
   
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }

   bzero(filenames,256);
   n = read(sockfd, filenames, 255);
 
   if (n < 0) {
      perror("ERROR getting response from socket");
      exit(1);
   }
   printf("FIles available are\n");
   puts(filenames);
 
   printf("\nPlease enter the a filename: ");
   bzero(filename,10);
   fgets(filename,9,stdin);
   
   n = write(sockfd, filename, strlen(filename));
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   
   n = read( sockfd,filedata,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   printf("File data sent from server is %s\n", filedata);

   return 0;
}
