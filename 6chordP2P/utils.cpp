#include "utils.h"

llu hash(string p, int len) { // consistent hashing
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
	return h;
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