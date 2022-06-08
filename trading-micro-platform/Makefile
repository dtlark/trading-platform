CC=g++-11
CFLAGS=-pthread -std=c++20 -g -Wall

main: main.o Order.h Orderbook.h
	$(CC) $(CFLAGS) -o main main.o

clean:
	rm *.o