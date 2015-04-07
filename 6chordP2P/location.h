#ifndef LOCATION_H
#define LOCATION_H

#include <arpa/inet.h>
#include <string>
#include "utils.h"
class location {
public:
	// location(string ip, string port);
	location(struct sockaddr_in me);
	location (string ip, int port);
	location();
	llu id;
	string ip;
	int port;
	struct sockaddr_in my_sock; // bound to node
	string getaddr();
};
#endif