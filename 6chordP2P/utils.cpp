#include "utils.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
void Tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters) {
  	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  	// Find first "non-delimiter".
	string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (string::npos != pos || string::npos != lastPos) {
  		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
    	// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
    	// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

string encode_addr(string ip, int port) {
	return ip + ":" + tostr(port);
}
// string encode_addr(struct sockaddr_in addr) {
// 	string out =  inet_ntoa(addr.sin_addr) + ":" + tostr((int) ntohs(addr.sin_port));
// }

int decode_port(string encoded_addr) {
	vector<string> tokens;
	Tokenize(encoded_addr, tokens);
	return atoi(tokens[1].c_str());
}

string decode_ip(string encoded_addr) {
	vector<string> tokens;
	Tokenize(encoded_addr, tokens);
	return tokens[0];
}

llu hash(string p) { // consistent hashing
	int len = p.size();
	llu h = 0;
	int i;
	for (i = 0; i < len; i++) {
		h += p[i];
		h += (h << 10);
		h ^= (h >> 6);
	}
	h += (h << 3);
	h ^= (h >> 11);
	h += (h << 15);
	// return h;
	return h%8;
}

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

string tostr(int x) {
	stringstream ss;
	ss << x;
	return ss.str();
}
void getownip() {

	struct ifaddrs * ifAddrStruct=NULL;
	struct ifaddrs * ifa=NULL;
	void * tmpAddrPtr=NULL;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
        	tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
        	char addressBuffer[INET_ADDRSTRLEN];
        	inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
        	printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
            // is a valid IP6 Address
        	tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
        	char addressBuffer[INET6_ADDRSTRLEN];
        	inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
        	printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } 
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
}