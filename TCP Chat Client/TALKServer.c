#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// This functions entire purpose is to display exactly
// what went wrong, and abort the program.
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	 // Variables needed for TCP Socket
     int sockfd, newsockfd, portno; // File descriptor values, and port number
     socklen_t clilen; // Size of client address
     char buffer[256]; // Data buffer
     struct sockaddr_in serv_addr, cli_addr; // This struct contains the addresses

     int n;

	// Checks for proper arguments
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

	 // Creation of the socket. The inputs for socket() are all protocols
	// Error message in case it fails. Very unlikely.
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

	 // Set our buffer to 0's
     bzero((char *) &serv_addr, sizeof(serv_addr));

	// Define the server information (the port number and address)
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

	// Bind the socket to the address and wait for connections
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

	// When a connection comes, it gets accepted and read from.
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

	// Send back confirmation to the client, and terminate.
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}
