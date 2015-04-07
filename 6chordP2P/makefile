redo: clean all

all: client 

client: client.o node.o utils.o message.o finger_table.o location.o
	g++ client.o node.o utils.o message.o finger_table.o location.o -o client

client.o: client.cpp
	g++ -c client.cpp

node.o: node.cpp node.h
	g++ -c node.cpp

finger_table.o: finger_table.cpp finger_table.h
	g++ -c finger_table.cpp 

utils.o: utils.cpp utils.h
	g++ -c utils.cpp

message.o: message.cpp message.h
	g++ -c message.cpp

location.o: location.cpp location.h
	g++ -c location.cpp

clean:
	rm *o