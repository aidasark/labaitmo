build: src/client.o src/server.o

src/client.o: src/client.cpp
	g++ -std=c++17 $^ -o client/client

src/server.o: src/server.cpp
	g++ -std=c++17 $^ -o server/server

clean:
	rm -f *.o client server
