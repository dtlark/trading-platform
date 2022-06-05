#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define PORT 8888

int main(int argc, char* argv[]) {

	cout << "1: Read" << endl;
	cout << "2: Write" << endl;

	// Input
	int choice;
	scanf("%d", &choice);

	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT); //translates a short integer from host byte order to network byte order

	//convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("Address error\n");
		return -1;
	}

	if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("Connection failed\n");
		return -1;
	}

	send(sock, &choice, sizeof(choice), 0);
	printf("Message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	close(client_fd);
	return 0;
}
