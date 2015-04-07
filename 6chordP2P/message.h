#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include "utils.h"

using namespace std;

class message {
public:
	struct sockaddr_in from;
	string type;
	string text;
	string msg;
	vector<string> tokens;
	message(char* buf, int size);
	message(string type,string text): type(type), text(text){};
	void get_buffer(char* buf);
	void print();
};

/**
Allowed types:
	NBR			Search for neighbor
	NBP			Update Predecessor
	NBS 		Update Successor
	RFL			Request file list
	ADD_FILE	Add file to map
	SEARCH 		Search
	UPD_FINGER	Update fingers
*/

#endif