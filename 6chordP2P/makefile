redo: clean all

all: client 

client: client.o node.o utils.o message.o
	g++ client.o node.o utils.o message.o -o client

client.o: client.cpp
	g++ -c client.cpp

node.o: node.cpp node.h
	g++ -c node.cpp

utils.o: utils.cpp utils.h
	g++ -c utils.cpp

message.o: message.cpp message.h
	g++ -c message.cpp

clean:
	rm *o