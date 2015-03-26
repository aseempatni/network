#include <iostream>

using namespace std;

typedef unsigned long long int llu;

class node {
	private:
		node* successor();
		node* predecessor();
};

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

node* search(llu file_id);
llu map_file_to_node (llu file_id);
bool download (string filename, node* server);

int main () {
	
}