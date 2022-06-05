#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

using namespace std;

#define PORT 8888

string orderStatus() {
	cout << "Order Status:" << endl;
	cout << "--Order ID: ";

	string orderNum;
	scanf("%s", &orderNum);

	return "status|" + orderNum;
}

string newOrder() {
	cout << "New Order:" << endl;
	cout << "--Price: ";
	cout << "--Quantity: ";
	cout << "--Type: ";

	string orderNum;
	scanf("%s", &orderNum);

	return "new|" + orderNum;
}

string cancelOrder() {
	cout << "Cancel:" << endl;
	cout << "--Order ID: ";

	string orderNum;
	scanf("%s", &orderNum);

	return "cancel|" + orderNum;
}

int main(int argc, char* argv[]) {

	string message = "";

	cout << "1: Order Status" << endl;
	cout << "2: New Order" << endl;
	cout << "3: Cancel Order" << endl;

	// Input
	int choice;
	scanf("%d", &choice);

	switch(choice) {
		case 1:
			message = orderStatus()
			break;
		case 2:
			message = newOrder()
			break;
		case 3:
			message = cancelOrder()
			break;
		default:
			cout << "Invalid function" << endl;
	}

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

	send(sock, &message.c_str(), strlen(message.c_str()), 0);
	printf("Message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	close(client_fd);
	return 0;
}
