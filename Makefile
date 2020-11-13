all : echo-client echo-server

echo-server :
	g++ -g -Wall -o echo-server echo-server.cpp -pthread -std=c++11

echo-client :
	g++ -g -Wall -o echo-client echo-client.cpp -pthread -std=c++11

clean:
	rm -f echo-server
	rm -f echo-client
	rm -f *.o