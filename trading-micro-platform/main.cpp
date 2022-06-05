#include <iostream>
// inet_addr
#include <arpa/inet.h>

#include <thread>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define PORT 8888

sem_t x, y;

thread writerthreads[100];
thread readerthreads[100];

int readercount = 0;

void* reader() {
	// Lock the semaphore
	sem_wait(&x);
	readercount++;
	if (readercount == 1)
		sem_wait(&y);
	// Unlock the semaphore
	sem_post(&x);
	cout << readercount << " reader in semaphore" << endl;
	sleep(5);
	// Lock the semaphore
	sem_wait(&x);
	readercount--;
	if (readercount == 0) {
		sem_post(&y);
	}
	// Lock the semaphore
	sem_post(&x);
	cout << (readercount + 1) << " reader exited" << endl;
	pthread_exit(NULL);
}

void* writer() {
	cout << "Writer waiting..." << endl;
	// Lock the semaphore
	sem_wait(&y);
	cout << "Writer in semaphore" << endl;
	// Unlock the semaphore
	sem_post(&y);
	cout << "Writer exited" << endl;
	pthread_exit(NULL);
}

int main() {
	int sockfd, newSocket;
	struct sockaddr_in address;
	struct sockaddr_storage serverStorage;

	socklen_t addr_size;
	sem_init(&x, 0, 1);
	sem_init(&y, 0, 1);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY); //converts the unsigned integer hostlong from host byte order to network byte order
	address.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, 50) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	puts("Listening...");

	int i = 0; 

	while (true) {
		addr_size = sizeof(serverStorage);

		newSocket = accept(sockfd, (struct sockaddr*)&serverStorage, &addr_size);
		int choice = 0;
		recv(newSocket, &choice, sizeof(choice), 0);

		if (choice == 1) {
			readerthreads[i++] = std::thread(reader);
		}
		else if (choice == 2) {
			writerthreads[i++] = std::thread(writer);
		}

		if (i >= 50) {
			i = 0;

			while (i < 50) {
				writerthreads[i++].join();
				readerthreads[i++].join();
			}

			i = 0;
		}
	}

	return 0;
}
