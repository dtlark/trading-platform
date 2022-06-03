#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 8888
	
int main(int argc, char *argv[]) {

	int sockfd;
	int addrlen, new_socket, client_socket[30], max_clients = 30, activity, i, valread, sd;
	int max_sd;
	struct sockaddr_in address;
	char buffer[256]; //data
	fd_set readfds;
	
	//init array to 0
	for (i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
	}
		
	if(sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

    int opt = 1;    //option enabled
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY); //converts the unsigned integer hostlong from host byte order to network byte order
	address.sin_port = htons(PORT);
		
	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	addrlen = sizeof(address);
	puts("Waiting for clients...");
		
	while(true) {
		FD_ZERO(&readfds);
	
		FD_SET(sockfd, &readfds);
		max_sd = sockfd;
			
		for ( i = 0 ; i < max_clients ; i++) {
			sd = client_socket[i];

			if(sd > 0)
				FD_SET( sd , &readfds);
				
			if(sd > max_sd)
				max_sd = sd;
		}
	
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR)) {
			printf("select");
		}

		if (FD_ISSET(sockfd, &readfds)) {

			if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
				perror("accept");
				exit(EXIT_FAILURE);
			}
            char *message = "ECHO Daemon v1.0 \r\n";
			if(send(new_socket, message, strlen(message), 0) != strlen(message)) {
				perror("send");
			}
				
			puts("Message sent");
				
			for (i = 0; i < max_clients; i++) {
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
						
					break;
				}
			}
		}
			
		for (i = 0; i < max_clients; i++) {
			sd = client_socket[i];
			if (FD_ISSET( sd , &readfds)) {
				if ((valread = read( sd , buffer, 1024)) == 0) {

					getpeername(sd , (struct sockaddr*)&address , \(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
					close( sd );
					client_socket[i] = 0;
				}
				else {
					buffer[valread] = '\0';
					send(sd , buffer , strlen(buffer) , 0 );
				}
			}
		}
	}
		
	return 0;
}
