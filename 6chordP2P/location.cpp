#include "location.h"

location::location(struct sockaddr_in me) {
	// ip = "127.0.0.1"; // needs to be corrected
	my_sock = me;
	port =  (int) ntohs(me.sin_port);
	ip = inet_ntoa(me.sin_addr);
	string pstr = getaddr();
	id = hash(pstr);
}

location::location (string ip, int port) {
	my_sock.sin_family=AF_INET;
	my_sock.sin_addr.s_addr=inet_addr(ip.c_str());
	my_sock.sin_port=htons(port);
	this->port = port;
	this->ip = ip;
	string pstr = getaddr();
	id = hash(pstr);
}

location::location() {
	
}

string location::getaddr() {
	return ip+":"+tostr(port);
}