#include <iostream>
#include <string>
#include <map>
// inet_addr
#include <arpa/inet.h>
#include <array>
#include <vector>
#include <thread>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "OrderMatcher.h"
#include <time.h>
#include <chrono>
#include <random>
#include <cmath>


using namespace std;

#define PORT 8888

sem_t x, y;

thread writerthreads[100];
thread readerthreads[100];

int readercount = 0;
map<string, string> orders;
string message = "Message";

array<int, 1000000> firmsC;
array<string, 1000000>  symbolsC; 
array<char, 1000000> sideC; 
array<float, 1000000> pricesC;

void data_TEST() {

	vector<string> symbols = {"APPL", "ETH", "LDOS", "SPY", "QQQ"};
	vector<int> firms = {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010};

	srand((unsigned)time( NULL ));

	for (int i = 0; i < 1000000; i++) { //1 million orders

		int firmRand = rand() % 10;
		std::mt19937 rng(std::random_device{}());
		bool boolRand =  std::uniform_int_distribution<>{0, 1}(rng);

		char side = 'S';
		if (boolRand) {
			side = 'B';
		}

		firmsC[i] = firms[firmRand];
		symbolsC[i] = symbols[i%5];
		sideC[i] = side;
		pricesC[i] = ceil((100.f + (float)rand()/RAND_MAX) * 100.0) / 100.0;
	}
}

void* reader(int new_socket) {
	// Lock the semaphore
	sem_wait(&x); //decrements
	readercount++;
	if (readercount == 1)
		sem_wait(&y);
	// Unlock the semaphore
	sem_post(&x); //increments
	cout << readercount << " reader in semaphore" << endl;
	
	if(send(new_socket, message.c_str(), strlen(message.c_str()), 0) != strlen(message.c_str())) {
		perror("send");
	}
	
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
	/*
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
			readerthreads[i++] = std::thread(reader, newSocket);
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
	*/

	Orderbook orderbook = Orderbook();

	//orderbook.NewOrder(1000, "APPL", 'B', 1021.51);
	//orderbook.NewOrder(1001, "APPL", 'B', 1021.61);
	//orderbook.NewOrder(1002, "APPL", 'S', 1021.45);

	data_TEST();

 	auto start = chrono::steady_clock::now();

	for (int j = 0; j < 1000000; j++) { //1 million orders

		orderbook.NewOrder(firmsC[j], symbolsC[j], sideC[j], pricesC[j]);	
	}

	auto end = chrono::steady_clock::now();

	cout << "Elapsed time in milliseconds: "
        << chrono::duration_cast<chrono::milliseconds>(end - start).count()
        << " ms" << endl;
	

	orderbook.print();

	return 0;
}
