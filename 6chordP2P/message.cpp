#include "message.h"
message::message(char* buf, int size) {
	string s(buf,size);
	msg = s;
	Tokenize(s, tokens);
	type = tokens[0];
	text = tokens[1];
	// text = s.substr(3);
	// type = s.substr(0,3);
	// print();s
}

void message::get_buffer (char* buf) {
	string out = type +":"+ text;
	strcpy(buf, out.c_str());
}

void message::print () {
	cout << "Message type: " << type << endl;
	cout << "Message text: " << text << endl;
}