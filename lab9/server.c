#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <time.h>

int main() {
      printf("Server running");
  int sockfd, newsockfd, portno, clilen;
   char username[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
    
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;
   
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
 
   bzero(username,256);
   n = read( newsockfd,username,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
   
   printf("Request: username %s\n",username);

   time_t current_time;
struct tm * time_info;
char timeString[9];  // space for "HH:MM:SS\0"

time(&current_time);
time_info = localtime(&current_time);

strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
puts(timeString);
   
   n = write(newsockfd,timeString,10);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
      
   return 0;
}
